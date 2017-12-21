#ifndef PISTA_H
#define PISTA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "Item.hpp"
#include <string>
#include <fstream>
#include "Waypoint.hpp"
#include "Turbo.hpp"

using namespace std;

class Pista
{

public:
  //Destructor
  ~Pista(void);
  //metodos GET
  static Pista *getInstancia();
  void InicializarFisicas();
  //btTriangleMesh *getBulletTriangleMesh(IMesh *const mesh, vector3df escala);
  void BorrarFisicas();
  void setMapa(stringw mapa, const char *fisicas, const char *waypoint);
  Turbo* getTurbo();
  core::list<Item *> getItems();
  void setItems(irr::core::list<Item *> itemMetodo);
  Waypoint **getArrayWaypoints();
private:
  Pista(void);
  static Pista *instancia;
  //mundo
  Motor3d *m;
  ISceneManager *smgr;
  btRigidBody *CuerpoColisionMapa;
  btCollisionShape *FormaColision;
  btDefaultMotionState *MotionState; // posicion del cuerpo de colision
  Waypoint **arrayWaypoints;
  Turbo    **arrayTurbos;
  btScalar Masa;
  ISceneNode *Mapa;
  Turbo *t;
  irr::core::list<Item *> Items;
};

#endif /* PISTA_H */
