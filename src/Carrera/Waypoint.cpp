#include "Waypoint.hpp"

Waypoint::Waypoint() {
	//waypoint = Motor3d::instancia().getScene()->addCubeSceneNode(0.1f);
	waypoint = TMotor::instancia().newMeshNode("Waypoint","assets/wall/wall.obj","escena_raiz");
	//waypoint->setMaterialFlag(EMF_LIGHTING, false); //Desactivar iluminacion
	//Motor3d::instancia().getScene()->getMeshManipulator()->setVertexColors(waypoint->getMesh(), SColor(255, 200, 100, 100));
	//waypoint->setVisible(false);
	//waypoint->setScale(vector3df(1500, 150, 05));
	waypoint->setName("Waypoint");
	siguiente = NULL; // Referenciar despues con la funcion
}

void Waypoint::inicializarFisicas() {

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	//posicion origem 
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(waypoint->getPosition().x, waypoint->getPosition().y, waypoint->getPosition().z));
	btQuaternion quaternion;
	quaternion.setEulerZYX(waypoint->getRotation().z* PI / 180, waypoint->getRotation().y * PI / 180, waypoint->getRotation().x* PI / 180);
	transform.setRotation(quaternion);

	MotionState = new btDefaultMotionState(transform);

	// Create the shape

	Shape = new btBoxShape(btVector3(1500*0.05f, 150*0.05f, 5*0.05f));

	// sin masa
	btVector3 localInertia;
	Shape->calculateLocalInertia(0, localInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(0, MotionState, Shape, localInertia);

	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	//ACTIVA LA COLISION SIN COLISIONAR CON EL OBJETO
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// Store a pointer to the irrlicht node so we can update it later
	rigidBody->setUserPointer((void *)(waypoint));

	// Add it to the world
	bullet->getMundo()->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);

}


// MEtodos SET
void Waypoint::setPosicion(float x, float y, float z) {
	waypoint->setPosition(x, y, z);
}

void Waypoint::setSiguiente(Waypoint *siguiente) {
	this->siguiente = siguiente;
}

void Waypoint::setOrientacion(float grado) {
	waypoint->setRotation(0.0f, grado, 0.0f);
}

// Metodos GET
btVector3 Waypoint::getPosicion() {
	return btVector3(waypoint->getPosition().x, waypoint->getPosition().y, waypoint->getPosition().z);
}
btVector3 Waypoint::getRotation() {
	return btVector3(waypoint->getRotation().x, waypoint->getRotation().y, waypoint->getRotation().z);
}
Waypoint *Waypoint::getNextWaypoint() {
	return siguiente;
}

obj3D *Waypoint::getWaypoint() {
	return waypoint;
}
int Waypoint::getID(){
	return id;
}

Waypoint::~Waypoint() {
	cout << "ENTRO DELETE WAYPOINT " <<endl;
	siguiente = nullptr;
	cout << " SALGO DELETE WAYPOINT \n" << endl;
}
void Waypoint::setVector1(btVector3 p) {
	vector1 = p;
}
void Waypoint::setVector2(btVector3 p2) {
	vector2 = p2;
}
btVector3 Waypoint::getVector1() {
	return vector1;
}
btVector3 Waypoint::getVector2() {
	return vector2;
}
void Waypoint::setID(int i){
	id=i;
}
