#include "Item.hpp"


Item::Item(int tipo) {
}

Item::Item(btVector3 pos)
{

	lanzado = false;

	masa = btScalar(50);
	
}

btRigidBody *Item::inicializarFisicas()
{

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
	// Add it to the world
	mundo->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);
	rigidBody->setGravity(btVector3(0, -6, 0));
	return rigidBody;
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
	if (timediff_sec >= 15) {
		return true;
	}
	return false;
}

bool Item::Delete()
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
int Item::getID(){
	return id;
}

void Item::setNombre(const char* name){
	nombre=name;
}
void Item::setMasa(btScalar mass){
	masa=mass;
}