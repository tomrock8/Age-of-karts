#include "Caja.hpp"

Caja::Caja(Motor3d *ventana, vector3df posicionCaja, int idCaja)
{
    tamanyo = 1.0f;
    nodo = ventana->getScene()->addCubeSceneNode(tamanyo);

    escala.X = 5.f;
    escala.Y = 5.f;
    escala.Z = 5.f;
	nodo->setScale(escala);

    posicion = posicionCaja;
	nodo->setPosition(posicion);

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, ventana->getDriver()->getTexture("assets/textures/rust.png"));

    nombre = "Caja";
    nodo->setName(nombre);

    id = idCaja;
    nodo->setID(id);

}

btRigidBody* Caja::inicializarFisicas()
{
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
	return rigidBody;
}

void Caja::Delete(core::list<btRigidBody *> &objetosMundo, btDiscreteDynamicsWorld *mundoMetodo, Corredor *pj1Col){
	for (list<btRigidBody *>::Iterator Iterator = objetosMundo.begin(); Iterator != objetosMundo.end(); ++Iterator)
	{
		ISceneNode *nodoActual = static_cast<ISceneNode *>(static_cast<btRigidBody *>(*Iterator)->getUserPointer());
		if (nodoActual->getID() == id)
		{

			btRigidBody *Object = *Iterator;

			// Delete irrlicht node
			ISceneNode *Node = static_cast<ISceneNode *>(Object->getUserPointer());

			if (strcmp("Caja", Node->getName())==0){
				if(pj1Col->getCargador()==0)
				pj1Col->incCargador();
			}
			Node->remove();

			// Remove the object from the world
			mundoMetodo->removeRigidBody(Object);

			// Free memory
			delete Object->getMotionState();
			delete Object->getCollisionShape();
			delete Object;

			Iterator = objetosMundo.erase(Iterator);
			break;
		}
	}
}