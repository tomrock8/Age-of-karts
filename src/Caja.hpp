#ifndef CAJA_H
#define CAJA_H

#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Motor3d.hpp"
#include <iostream>

using namespace std;

class Caja
{
public:
  Caja(Motor3d *ventana, vector3df posicionCaja, int idCaja);
  btRigidBody *inicializarFisicas();
  ISceneNode *getNodo() { return nodo; };
  btRigidBody *getRigidBody() { return rigidBody; };
  char *getNombre() { return nombre; };
  int getIDCaja() { return id; };
  void Delete(core::list<btRigidBody *> &objetosMundo, btDiscreteDynamicsWorld *mundoMetodo, Corredor *pj1Col);

private:
  ISceneNode *nodo;
  btRigidBody *rigidBody;
  btScalar masa;
  f32 tamanyo;
  vector3df escala;
  vector3df posicion;
  char *nombre;
  int id;
};

#endif