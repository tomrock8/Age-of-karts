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
  void setNombre(std::string nombre);
  void setOrientacion(float grados);
  void setDireccion();

  //metodos GET
  IMeshSceneNode *getWaypoint();
  btVector3 getPosicion();
  Waypoint *getNextWaypoint();
  std::string getNombre();
  btVector3 getDireccion();

private:
  IMeshSceneNode *waypoint;
  std::string nombre;
  Waypoint *siguiente; // Siguiente punto
  btVector3 direccion;

  //fisicas
  btDefaultMotionState *MotionState;
  btCollisionShape *Shape;
  btRigidBody * rigidBody;
};

#endif /* WAYPOINT_H */