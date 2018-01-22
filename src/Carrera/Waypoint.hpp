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

  //metodos GET
  IMeshSceneNode *getWaypoint();
  btVector3 getPosicion();
  Waypoint *getNextWaypoint();

private:
  IMeshSceneNode *waypoint;
  Waypoint *siguiente; // Siguiente punto

  //fisicas
  btDefaultMotionState *MotionState;
  btCollisionShape *Shape;
  btRigidBody * rigidBody;
};

#endif /* WAYPOINT_H */