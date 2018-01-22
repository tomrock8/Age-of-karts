#include "Estatico.hpp"

Estatico::Estatico(vector3df posicionItem, int idItem) : Item(posicionItem, idItem)
{
	Motor3d *m = Motor3d::getInstancia();
	tamanyo = 1.0f;
	nodo = m->getScene()->addCubeSceneNode(tamanyo);
	nodo->setName("Estatico");

	escala.X = 3.f;
	escala.Y = 3.f;
	escala.Z = 3.f;
	nodo->setScale(escala);

	posicion = posicionItem;
	nodo->setPosition(posicion);

	masa = 10000;

	id = idItem;
	nodo->setID(id);
	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, m->getDriver()->getTexture("assets/textures/gust.png"));
}

btRigidBody *Estatico::inicializarFisicas()
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

void Estatico::soltarItem()
{
	//otro que se fue al corredor bitches :(
}
/*Estatico::~Estatico() {
	delete rigidBody->getMotionState();
	delete rigidBody->getCollisionShape();
	delete rigidBody;
}*/