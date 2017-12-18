#include "Item.hpp"

Item::Item(Motor3d *ventana, vector3df posicionItem, int idItem)
{
    tamanyo = 1.0f;
    nodo = ventana->getScene()->addCubeSceneNode(tamanyo);

    escala.X = 3.f;
    escala.Y = 3.f;
    escala.Z = 3.f;
	nodo->setScale(escala);

    posicion = posicionItem;
	nodo->setPosition(posicion);

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, ventana->getDriver()->getTexture("assets/textures/rust.png"));

    nombre = "Item";
    nodo->setName(nombre);

    id = idItem;
    nodo->setID(id);

}

btRigidBody* Item::inicializarFisicas()
{

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
    // Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(posicion.X, posicion.Y, posicion.Z));
	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(escala.X * 0.5f, escala.Y * 0.5f, escala.Z * 0.5f);
	btCollisionShape *Shape = new btBoxShape(HalfExtents);

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

void Item::Delete(){
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
			delete Object->getMotionState();
			delete Object->getCollisionShape();
			delete Object;

			Iterator = objetos.erase(Iterator);
			break;
		}
	}
}
btRigidBody *Item::getRigidBody(){
	return rigidBody;
}