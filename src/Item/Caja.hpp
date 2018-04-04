#ifndef CAJA_H
#define CAJA_H

#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
//#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "Timer.hpp"
#include "GestorIDs.hpp"
#include <iostream>
#include "TMotor.hpp"

class Caja
{
public:
  Caja(btVector3 posicionCaja);
  btRigidBody *inicializarFisicas();
  obj3D *getNodo();
  btRigidBody *getRigidBody();
  const char *getNombre();
  int getID();
  void romper(Corredor *pj1Col);
  void comprobarRespawn();
  void Delete();
   ~Caja();

private:

  obj3D *nodo;
  btRigidBody *rigidBody;
  btScalar masa;
  btVector3 escala;
  btVector3 posicion;
  const char *nombre;
  int id;
  int timer;
};

#endif