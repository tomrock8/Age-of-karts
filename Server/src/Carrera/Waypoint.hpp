#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "btBulletDynamicsCommon.h"
#include "obj3D.hpp"

#define PI 3.14159265358979323846  /* pi */
#define RADTODEG 180.0f / PI

using namespace std;

class Waypoint {
public:
	Waypoint();
	~Waypoint();
	void inicializarFisicas();

	// METODOS SET
	void setPosicion(float x, float y, float z);
	void setSiguiente(Waypoint *siguiente);
	void setOrientacion(float grados);
	void setVector1(btVector3 p1);
	void setVector2(btVector3 p2);
	void setVector3(btVector3 p3);
	void setVector4(btVector3 p4);
	void setID(int i);
	
	// METODOS GET
	int getID();
	obj3D *getWaypoint();
	btVector3 getPosicion();
	Waypoint *getNextWaypoint();
	btVector3 getVector1();
	btVector3 getVector2();
	btVector3 getVector3();
	btVector3 getVector4();
	btVector3 getRotation();
	
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