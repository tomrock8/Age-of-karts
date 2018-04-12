#include "Caja.hpp"

Caja::Caja(btVector3 posicionCaja) {

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	//nodo = Motor3d::instancia().getScene()->addMeshSceneNode(Motor3d::instancia().getScene()->getMesh("assets/Objetos/caja.obj"));
	nodo = TMotor::instancia().newMeshNode("Caja", "assets/caja/caja.obj", "escena_raiz");
	nombre = "Caja";
	GestorIDs::instancia().setIdentifier(nodo, nombre);
	id = GestorIDs::instancia().getIDLibre() - 1;

	escala.setX(2);
	escala.setY(2);
	escala.setZ(2);
	//nodo->setScale(vector3df(escala.getX(),escala.getY(),escala.getZ()));

	posicion = posicionCaja;
	nodo->setPosition(posicion.getX(), posicion.getY(), posicion.getZ());



	rigidBody = inicializarFisicas();
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mundo->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);
}

btRigidBody *Caja::inicializarFisicas()
{
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(posicion);
	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(escala.getX() * 1.5, escala.getY()* 1.5, escala.getZ() * 1.5);
	btCollisionShape *Shape = new btBoxShape(HalfExtents);

	// Add mass
	btVector3 LocalInertia;
	masa = btScalar(0);
	Shape->calculateLocalInertia(masa, LocalInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(masa, MotionState, Shape, LocalInertia);
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);

	// Store a pointer to the irrlicht node so we can update it later
	rigidBody->setUserPointer((void *)(nodo));
	if (masa != 0)
		rigidBody->setActivationState(DISABLE_DEACTIVATION);
	// Add it to the world
	return rigidBody;
}

void Caja::comprobarRespawn() {
	Timer *time = Timer::getInstancia();
	if (nodo) {
		if (!nodo->isVisible()) {
			if (time->getTimer() - timer >= 4) {
				//cout << "Visible\n";
				nodo->setVisible(true);
			}
		}
	}
}

void Caja::romper(Corredor *pj1Col) {
	Timer *time = Timer::getInstancia();
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	for (int i = 0; i < objetos.size(); i++)
	{
		obj3D *nodoActual = static_cast<obj3D *>(static_cast<btRigidBody *>(objetos.at(i))->getUserPointer());
		if (nodoActual->getID() == id) {
			btRigidBody *Object = objetos.at(i);

			// Delete irrlicht node
			obj3D *Node = static_cast<obj3D *>(Object->getUserPointer());
			Node->setVisible(false);
			timer = time->getTimer();
			if (pj1Col->getTipoObj() == 0 && strcmp(pj1Col->getNodo()->getName(), "JugadorRed") != 0) {
				//std::cout<< "Entro al objeto\n";
				pj1Col->setTipoObj();

			}

			break;
		}
	}
}

void Caja::Delete() {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	for (int i = 0; i < objetos.size(); i++) {
		obj3D *nodoActual = static_cast<obj3D *>(static_cast<btRigidBody *>(objetos.at(i))->getUserPointer());
		if (nodoActual->getID() == id) {

			btRigidBody *Object = objetos.at(i);

			// Delete irrlicht node
			obj3D *Node = static_cast<obj3D *>(Object->getUserPointer());

			//Node->remove();

			// Remove the object from the world
			mundo->removeRigidBody(Object);

			// Free memory
			delete Object->getMotionState();
			delete Object->getCollisionShape();
			delete Object;
			objetos.erase(objetos.begin() + i);
			bullet->setObjetos(objetos);

			break;
		}
	}
}

Caja::~Caja() {}
int Caja::getID() { return id; }
obj3D *Caja::getNodo() { return nodo; }
btRigidBody *Caja::getRigidBody() { return rigidBody; }
const char *Caja::getNombre() { return nombre; }