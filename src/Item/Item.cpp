#include "Item.hpp"
Item::Item(btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo)
{
	colision = false;
	lanzado = false;
	idNodoPadre = idNodo;
	this->masa = masa;
	this->posicion = posicion;
	this->escala = escala;
	this->tiempoDesctruccion = tiempoDesctruccion;
	this->tamanyoNodo = tamanyoNodo;
	this->radio = radio;
	this->fcolision = fcolision;
	this->alturaLanzamiento = alturaLanzamiento;
	orientacionItem = btVector3(1, 0, 0);

	subir = false;
	bajar = false;
	indiceAltura = 0;
	diferencia = 0.035;
	alturaItem = 0;
}

Item::~Item() {

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	for (int i = 0; i < objetos.size(); i++) {
		obj3D *nodoActual = static_cast<obj3D *>(static_cast<btRigidBody *>(objetos.at(i))->getUserPointer());
		if (nodoActual->getID() == id && strcmp(nodoActual->getName(), "rueda1") != 0 && strcmp(nodoActual->getName(), "rueda2") != 0 && strcmp(nodoActual->getName(), "rueda3") != 0
			&& strcmp(nodoActual->getName(), "rueda4") != 0) {
			btRigidBody *Object = objetos.at(i);

			// Remove the object from the world
			mundo->removeRigidBody(Object);

			// Free memory

			delete Object->getCollisionShape();
			delete Object->getMotionState();
			delete Object;

			objetos.erase(objetos.begin() + i);
			bullet->setObjetos(objetos);

		}
	}

	delete nodo;
}

void Item::inicializarFisicas() {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(posicion);
	MotionState = new btDefaultMotionState(Transform);
	btVector3 HalfExtents(escala.getX(), escala.getY(), escala.getZ());
	//btVector3 HalfExtents(1, 1, 1);
	// Create the shape
	switch (fcolision) {

	case CUBO:
		Shape = new btBoxShape(HalfExtents);
		break;

	case ESFERA:
		Shape = new btSphereShape(radio);
		break;

	case CILINDRO:
		Shape = new btCylinderShape(HalfExtents);
		break;

	}

	// Add mass
	btVector3 LocalInertia;
	Shape->calculateLocalInertia(masa, LocalInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(masa, MotionState, Shape, LocalInertia);



	rigidBody->setUserPointer((void *)(nodo));

	// Add it to the world
	mundo->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);

}

void Item::setLanzado(bool b) {
	lanzado = b;
	tiempoLanzado = clock();
}

bool Item::getLanzado()
{
	return lanzado;
}

bool Item::comprobarDestructor() {
	clock_t timediff = clock() - tiempoLanzado;
	float timediff_sec = ((float)timediff) / 100000;
	if (timediff_sec >= tiempoDesctruccion) {
		return true;
	}
	return false;
}


bool Item::update() {
	//cout<<nodo->getRotation().y<<endl;
	if (comprobarDestructor()) {

		return true;
	}
	else {

		return false;
	}
}


void Item::Delete() {
	

	
	
}





//COmprobar distancias con el suelo
void Item::ajustarAltura() {

	btTransform posObj = rigidBody->getCenterOfMassTransform();
	float altura = 0;


	if (subir) {
		altura = (alturaItem - indiceAltura) / diferencia;
		posObj.setOrigin(btVector3(posObj.getOrigin().getX(), posObj.getOrigin().getY() + altura, posObj.getOrigin().getZ()));
	}
	else if (bajar) {
		altura = (indiceAltura - alturaItem) / diferencia;
		posObj.setOrigin(btVector3(posObj.getOrigin().getX(), posObj.getOrigin().getY() - altura, posObj.getOrigin().getZ()));
	}


	rigidBody->setCenterOfMassTransform(posObj);

	subir = false;
	bajar = false;


}

void Item::comprobarAltura(float altura) {

	alturaItem = altura;
	MotorFisicas *mun = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = mun->getMundo();
	mundo->updateAabbs();
	mundo->computeOverlappingPairs();

	// Raycast central1
	btVector3 inicio(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z);
	btVector3 fin(nodo->getPosition().x, nodo->getPosition().y - 40, nodo->getPosition().z);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);


	if (RayCast1.hasHit())
	{

		for (int i = 0; i < RayCast1.m_hitFractions.size(); i++)
		{
			if (i < RayCast1.m_hitFractions.size() - 1) {
				indiceAltura = RayCast1.m_hitFractions[i];

				if (RayCast1.m_hitFractions[i] < altura) {
					subir = true;

				}
				if (RayCast1.m_hitFractions[i] > altura) {

				}
				if (RayCast1.m_hitFractions[i] > altura) {

					bajar = true;

				}
			}
		}
	}

}

btRigidBody *Item::getRigidBody()
{
	return rigidBody;
}
obj3D *Item::getNodo()
{
	return nodo;

}
const char* Item::getNombre() {
	return nombre;
}
int Item::getID() {
	return id;
}

int Item::getIDPadre() {
	return idNodoPadre;
}

void Item::setNombre(const char* nombre) {
	this->nombre = nombre;
}
void Item::setMasa(btScalar masa) {
	this->masa = masa;
}

void Item::setIDPadre(int id) {
	idNodoPadre = id;
}

void Item::setColision(int id) {
	idwaypoint = id;
	//cout<<"idwaypoint: "<<idwaypoint-7<<endl;
	colision = true;
}