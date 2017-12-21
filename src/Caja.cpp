#include "Caja.hpp"

Caja::Caja(vector3df posicionCaja, int idCaja)
{
	Motor3d *m = Motor3d::getInstancia();
	tamanyo = 1.0f;
	nodo = m->getScene()->addCubeSceneNode(tamanyo);

	escala.X = 5.f;
	escala.Y = 5.f;
	escala.Z = 5.f;
	nodo->setScale(escala);

	posicion = posicionCaja;
	nodo->setPosition(posicion);

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);

	nodo->setMaterialTexture(0, m->getDriver()->getTexture("assets/textures/rust.png"));
	nombre = "Caja";
	nodo->setName(nombre);
	id = idCaja;
	nodo->setID(id);
}

btRigidBody *Caja::inicializarFisicas()
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
	masa = 0;
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

void Caja::comprobarRespawn(){
	Motor3d *mundo = Motor3d::getInstancia();
	if(!nodo->isVisible()){
		if(mundo->getTime()-timer >= 5000){
			cout<<"Visible\n";
			nodo->setVisible(true);
		}
	}
}

void Caja::romper(Corredor *pj1Col){
	Motor3d *mundo = Motor3d::getInstancia();
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	core::list<btRigidBody *> objetos = bullet->getObjetos();

	for (list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator)
	{
		ISceneNode *nodoActual = static_cast<ISceneNode *>(static_cast<btRigidBody *>(*Iterator)->getUserPointer());
		if (nodoActual->getID() == id)
		{
			btRigidBody *Object = *Iterator;

			// Delete irrlicht node
			ISceneNode *Node = static_cast<ISceneNode *>(Object->getUserPointer());
			Node->setVisible(false);
			timer = mundo->getTime();
			if(pj1Col->getTipoObj()==0){
				pj1Col->setTipoObj();
			}

			break;
		}
	}
}

void Caja::Delete()
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
			delete Object->getMotionState();
			delete Object->getCollisionShape();
			delete Object;
			Iterator = objetos.erase(Iterator);
			bullet->setObjetos(objetos);

			break;
		}
	}
}