#include "Item.hpp"



Item::Item(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
float alturaLanzamiento,int idNodo)
{

	colision=false;
	lanzado = false;
	idNodoPadre=idNodo;
	this->masa = masa;
	this->posicion = posicion;
	this->escala=escala;
	this->tiempoDesctruccion=tiempoDesctruccion;
	this->tamanyoNodo=tamanyoNodo;
	this->radio=radio;
	this->fcolision=fcolision;
	this->alturaLanzamiento=alturaLanzamiento;
	


}

void Item::inicializarFisicas()
{

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	vector<btRigidBody *> objetos = bullet->getObjetos();
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(posicion);
	MotionState = new btDefaultMotionState(Transform);
	btVector3 HalfExtents(escala.getX() , escala.getY(), escala.getZ());
	// Create the shape
	switch(fcolision){

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
	
	if (masa != 0)
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	else
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	

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

bool Item::comprobarDestructor(){
	clock_t timediff = clock() - tiempoLanzado;
	float timediff_sec = ((float)timediff) / 100000;
	if (timediff_sec >= tiempoDesctruccion) {
		return true;
	}
	return false;
}


bool Item::update(){
			
	if(comprobarDestructor()){
	Delete();
	return true;
	}else{
	updateHijos();
	return false;
	}
}


void Item::Delete()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	vector<btRigidBody *> objetos = bullet->getObjetos();

	for (int i=0;i<objetos.size();i++)
	{
		ISceneNode *nodoActual = static_cast<ISceneNode *>(static_cast<btRigidBody *>(objetos.at(i))->getUserPointer());
		if (nodoActual->getID() == id)
		{

			btRigidBody *Object = objetos.at(i);

			// Delete irrlicht node
			ISceneNode *Node = static_cast<ISceneNode *>(Object->getUserPointer());

			Node->remove();

			// Remove the object from the world
			mundo->removeRigidBody(Object);

			// Free memory

			delete Object->getCollisionShape();
			delete Object->getMotionState();
			delete Object;

			objetos.erase(objetos.begin()+i);
			bullet->setObjetos(objetos);
			
		}
	}

}

btRigidBody *Item::getRigidBody()
{
	return rigidBody;
}
IMeshSceneNode *Item::getNodo()
{
	return nodo;
	
}
const char* Item::getNombre(){
	return nombre;
}
int Item::getID(){
	return id;
}

int Item::getIDPadre(){
	return idNodoPadre;
}

void Item::setNombre(const char* nombre){
	this->nombre=nombre;
}
void Item::setMasa(btScalar masa){
	this->masa=masa;
}

void Item::setIDPadre(int id){
	idNodoPadre=id;
}

void Item::setColision(int id){

	idwaypoint=id;
	colision=true;

}