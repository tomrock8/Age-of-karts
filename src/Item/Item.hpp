#ifndef ITEM_H
#define ITEM_H

#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "GestorIDs.hpp"
#include <iostream>

using namespace std;

class Item
{
public:
  Item(int tipo);
  Item(vector3df posicionItem);
  btRigidBody *inicializarFisicas();
  void lanzarItem();
  bool Delete();
  IMeshSceneNode *getNodo();
  btRigidBody *getRigidBody();
  void setColision(bool b);
  bool getColision();
  void setNombre(const char* name);
  const char* getNombre();
  void setMasa(btScalar mass);

protected:
  IMeshSceneNode *nodo;
  btRigidBody *rigidBody;
  btScalar masa;
  f32 tamanyo;
  vector3df escala;
  vector3df posicion;
  const char *nombre;
  int id;
  bool colision;
  btDefaultMotionState *MotionState;
  btCollisionShape *Shape;
};

#endif