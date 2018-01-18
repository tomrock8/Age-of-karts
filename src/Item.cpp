#include "Item.hpp"

Item::Item(vector3df posicionItem, int idItem)
{
	Motor3d *m = Motor3d::getInstancia();
	tamanyo = 1.0f;
	nodo = m->getScene()->addCubeSceneNode(tamanyo);

	escala.X = 3.f;
	escala.Y = 3.f;
	escala.Z = 3.f;
	nodo->setScale(escala);

	posicion = posicionItem;
	nodo->setPosition(posicion);

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, m->getDriver()->getTexture("assets/textures/gust.png"));

	
	nodo->setName("Item");

	id = idItem;
	nodo->setID(id);

	colision = false;
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

	masa = 1;
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
ISceneNode *Item::getNodo()
{
	return nodo;
}
const char* Item::getNombre(){
	return nombre;
}
void Item::setNombre(const char* name){
	nombre=name;
}