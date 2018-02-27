#ifndef CAJA_H
#define CAJA_H

#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "Timer.hpp"
#include "GestorIDs.hpp"
#include <iostream>

using namespace std;

class Caja
{
public:
  Caja(btVector3 posicionCaja);
  btRigidBody *inicializarFisicas();
  IMeshSceneNode *getNodo();
  btRigidBody *getRigidBody();
  const char *getNombre();
  void romper(Corredor *pj1Col);
  void comprobarRespawn();
  void Delete();
   ~Caja();

private:

  IMeshSceneNode *nodo;
  btRigidBody *rigidBody;
  btScalar masa;
  btVector3 escala;
  btVector3 posicion;
  const char *nombre;
  int id;
  int timer;
};

#endif