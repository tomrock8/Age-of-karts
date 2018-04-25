#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <iostream>
#include "MotorFisicas.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "TMotor.hpp"

using namespace std;

class Waypoint
{
public:
	//Constructor
	Waypoint();
	//Destructor
	~Waypoint();


	void inicializarFisicas();
	//auxiliares

	//Update

  //metodos SET
	void setPosicion(float x, float y, float z);
	void setSiguiente(Waypoint *siguiente);
	void setOrientacion(float grados);
	void setVector1(btVector3 p1);
	void setVector2(btVector3 p2);
	void setVector3(btVector3 p3);
	void setVector4(btVector3 p4);
	void setID(int i);
	//metodos GET

	obj3D *getWaypoint();
	btVector3 getPosicion();
	Waypoint *getNextWaypoint();
	btVector3 getVector1();
	btVector3 getVector2();
	btVector3 getVector3();
	btVector3 getVector4();
	btVector3 getRotation();
	int getID();
private:
	obj3D * waypoint;
	Waypoint *siguiente; // Siguiente punto

  //fisicas
	btDefaultMotionState *MotionState;
	btCollisionShape *Shape;
	btRigidBody * rigidBody;

	//vectores
	btVector3 vector1;
	btVector3 vector2;
	btVector3 vector3;
	btVector3 vector4;
	int id;
};

#endif /* WAYPOINT_H */