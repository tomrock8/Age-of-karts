#include "Waypoint.hpp"

Waypoint::Waypoint()
{
	Motor3d *m = Motor3d::getInstancia();
	waypoint = m->getScene()->addCubeSceneNode(0.1f);
	waypoint->setMaterialFlag(EMF_LIGHTING, false); //Desactivar iluminacion
	m->getScene()->getMeshManipulator()->setVertexColors(waypoint->getMesh(), SColor(255, 200, 100, 100));
	waypoint->setVisible(false);
	waypoint->setScale(vector3df(950, 150, 05));
	waypoint->setName("Waypoint");
	siguiente = NULL; // Referenciar despues con la funcion
	

}

void Waypoint::inicializarFisicas(){

	MotorFisicas *bullet = MotorFisicas::getInstancia();
    irr:core::list<btRigidBody *> objetos = bullet->getObjetos();
    Motor3d *m = Motor3d::getInstancia();
    

	//posicion origem 
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(waypoint->getPosition().X,waypoint->getPosition().Y,waypoint->getPosition().Z));
	btQuaternion quaternion;
	quaternion.setEulerZYX(waypoint->getRotation().Z* PI/180,waypoint->getRotation().Y * PI/180,waypoint->getRotation().X* PI/180);
	transform.setRotation(quaternion);

	MotionState = new btDefaultMotionState(transform);

    // Create the shape

	Shape = new btBoxShape(btVector3(waypoint->getScale().X*0.05f,waypoint->getScale().Y*0.05f,waypoint->getScale().Z*0.05f));

	// sin masa
	btVector3 localInertia;
	Shape->calculateLocalInertia(0, localInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(0, MotionState, Shape,localInertia);

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
void Waypoint::setPosicion(float x, float y, float z)
{
	waypoint->setPosition(vector3df(x, y, z));
}

void Waypoint::setSiguiente(Waypoint *siguiente)
{
	this->siguiente = siguiente;
}

void Waypoint::setOrientacion(float grado)
{

	waypoint->setRotation(vector3df(0.0f,grado,0.0f));

}

// Metodos GET
btVector3 Waypoint::getPosicion()
{
	return btVector3(waypoint->getPosition().X,waypoint->getPosition().Y,waypoint->getPosition().Z);
}

Waypoint *Waypoint::getNextWaypoint()
{

	return siguiente;
}

IMeshSceneNode *Waypoint::getWaypoint(){

	return waypoint;

}
Waypoint::~Waypoint() {
	delete rigidBody->getMotionState();
	delete rigidBody->getCollisionShape();
	delete rigidBody;
}