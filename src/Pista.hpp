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
#include "Caja.hpp"

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
  Turbo **getArrayTurbo();
  Caja **getArrayCaja();
  void setArrayCaja(Caja **cajas);
  int getTamCajas();
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
  Caja     **arrayCajas;
  int tamCajas;
  btScalar Masa;
  ISceneNode *Mapa;

  irr::core::list<Item *> Items;
};

#endif /* PISTA_H */
