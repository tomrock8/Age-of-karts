#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

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
  void setID(int i);
  //metodos GET
  IMeshSceneNode *getWaypoint();
  btVector3 getPosicion();
  Waypoint *getNextWaypoint();
  btVector3 getVector1();
  btVector3 getVector2();
  btVector3 getRotation();
  int getID();
private:
	IMeshSceneNode * waypoint;
	Waypoint *siguiente; // Siguiente punto

  //fisicas
  btDefaultMotionState *MotionState;
  btCollisionShape *Shape;
  btRigidBody * rigidBody;

  //vectores
  btVector3 vector1;
  btVector3 vector2;
  int id;
};

#endif /* WAYPOINT_H */