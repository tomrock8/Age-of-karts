#include "Estatico.hpp"

Estatico::Estatico(btVector3 posicionItem) {
	/*
	tamanyo = 1.0f;
	nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyo);
	GestorIDs::instancia().setIdentifier(nodo,"Estatico");
	id=nodo->getID();

	escala.setX(3);
	escala.setY(3);
	escala.setZ(3);
	nodo->setScale(vector3df(escala.getX(),escala.getY(),escala.getZ()));
	posicionItem.setY(posicionItem.getY()-1);
	posicion = posicionItem;
	nodo->setPosition(vector3df(posicion.getX(),posicion.getY(),posicion.getZ()));

	masa = 0;

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/gust.png"));
*/
}
/*
btRigidBody *Estatico::inicializarFisicas() {

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	vector<btRigidBody *> objetos = bullet->getObjetos();
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(posicion);
	MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(escala.getX() * 0.5f, escala.getY() * 0.5f, escala.getZ() * 0.5f);
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
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// Add it to the world
	mundo->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);

	return rigidBody;
}

void Estatico::soltarItem() {
	//otro que se fue al corredor bitches :(
}
*/