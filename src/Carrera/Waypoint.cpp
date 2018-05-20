#include "Waypoint.hpp"
#include "TMotor.hpp"
#include "MotorFisicas.hpp"
#include "math.h"
#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  
#endif

Waypoint::Waypoint() {
	waypoint = TMotor::instancia().newMeshNode("Waypoint", "assets/wall/wall.obj", "escena_raiz", false);
	waypoint->setVisible(false);
	waypoint->setName("Waypoint");
	siguiente = nullptr; // Referenciar despues con la funcion
}

void Waypoint::inicializarFisicas() {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	//posicion origem 
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(waypoint->getPosition().x, waypoint->getPosition().y, waypoint->getPosition().z));
	btQuaternion quaternion;
	quaternion.setEulerZYX(waypoint->getRotation().z* M_PI / 180, waypoint->getRotation().y * M_PI / 180, waypoint->getRotation().x* M_PI / 180);
	transform.setRotation(quaternion);

	// Create the shape
	btCollisionShape *Shape = new btBoxShape(btVector3(950 * 0.05f, 150 * 0.05f, 5 * 0.05f));

	// sin masa
	btVector3 localInertia;
	Shape->calculateLocalInertia(0, localInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(0, new btDefaultMotionState(transform), Shape, localInertia);

	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	//ACTIVA LA COLISION SIN COLISIONAR CON EL OBJETO
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// Store a pointer to the irrlicht node so we can update it later
	rigidBody->setUserPointer((void *)(waypoint));

	// Add it to the world
	bullet->getMundo()->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);

	Shape = nullptr;
}
Waypoint::~Waypoint() {
	siguiente = nullptr;

	delete rigidBody->getCollisionShape();
	delete rigidBody->getMotionState();

	MotorFisicas::getInstancia()->getMundo()->removeCollisionObject(rigidBody);
	delete rigidBody;

	delete waypoint; // Modelo 3D
}

// MEtodos SET
void Waypoint::setPosicion(float x, float y, float z) { waypoint->setPosition(x, y, z); }
void Waypoint::setSiguiente(Waypoint *siguiente) { this->siguiente = siguiente; }
void Waypoint::setOrientacion(float grado) { waypoint->setRotation(0.0f, grado, 0.0f); }
void Waypoint::setVector1(btVector3 p) { vector1 = p; }
void Waypoint::setVector2(btVector3 p2) { vector2 = p2; }
void Waypoint::setVector3(btVector3 p3) { vector3 = p3; }
void Waypoint::setVector4(btVector3 p4) { vector4 = p4; }
void Waypoint::setID(int i) { id = i; }

// Metodos GET
int Waypoint::getID() { return id; }
obj3D *Waypoint::getWaypoint() { return waypoint; }
Waypoint *Waypoint::getNextWaypoint() { return siguiente; }
btVector3 Waypoint::getVector1() { return vector1; }
btVector3 Waypoint::getVector2() { return vector2; }
btVector3 Waypoint::getVector3() { return vector3; }
btVector3 Waypoint::getVector4() { return vector4; }
btVector3 Waypoint::getPosicion() {
	return btVector3(waypoint->getPosition().x, waypoint->getPosition().y, waypoint->getPosition().z);
}
btVector3 Waypoint::getRotation() {
	return btVector3(waypoint->getRotation().x, waypoint->getRotation().y, waypoint->getRotation().z);
}


