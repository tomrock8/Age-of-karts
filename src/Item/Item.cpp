#include "Item.hpp"



Item::Item(vector3df posicionItem)
{
	Motor3d *m = Motor3d::getInstancia();
	colision = false;

	masa = 1;
	
}

btRigidBody *Item::inicializarFisicas()
{

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(posicion.X, posicion.Y, posicion.Z));
	MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(escala.X * 0.5f, escala.Y * 0.5f, escala.Z * 0.5f);
	Shape = new btBoxShape(HalfExtents);

	// Add mass
	btVector3 LocalInertia;

	
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
	mundo->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);

	return rigidBody;
}

void Item::setColision(bool b) {
	colision = b;
}

bool Item::getColision()
{
	return colision;
}

bool Item::Delete()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();

	for (list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator)
	{
		ISceneNode *nodoActual = static_cast<ISceneNode *>(static_cast<btRigidBody *>(*Iterator)->getUserPointer());
		if (nodoActual->getID() == id)
		{

			btRigidBody *Object = *Iterator;

			// Delete irrlicht node
			ISceneNode *Node = static_cast<ISceneNode *>(Object->getUserPointer());

			Node->remove();

			// Remove the object from the world
			mundo->removeRigidBody(Object);

			// Free memory

			delete Object->getCollisionShape();
			delete Object->getMotionState();
			delete Object;

			Iterator = objetos.erase(Iterator);
			bullet->setObjetos(objetos);
			return true;
		}
	}
	return false;
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
void Item::setNombre(const char* name){
	nombre=name;
}
void Item::setMasa(btScalar mass){
	masa=mass;
}