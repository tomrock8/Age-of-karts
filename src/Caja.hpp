#ifndef CAJA_H
#define CAJA_H

#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include <ctime>
#include <iostream>

using namespace std;

class Caja
{
public:
  Caja(vector3df posicionCaja, int idCaja);
  btRigidBody *inicializarFisicas();
  ISceneNode *getNodo() { return nodo; };
  btRigidBody *getRigidBody() { return rigidBody; };
  const char *getNombre() { return nombre; };
  int getIDCaja();
  void romper(Corredor *pj1Col);
  void comprobarRespawn();
  void Delete();

private:
  ISceneNode *nodo;
  btRigidBody *rigidBody;
  btScalar masa;
  f32 tamanyo;
  vector3df escala;
  vector3df posicion;
  const char *nombre;
  int id;
  int timer;
};

#endif