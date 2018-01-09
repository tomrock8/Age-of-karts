#ifndef GESTORCOLISIONES_H
#define GESTORCOLISIONES_H

#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "Caja.hpp"
#include "Item.hpp"
#include "Proyectil.hpp"
#include "Pista.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "MotorFisicas.hpp"
#include <iostream>

using namespace std;

class GestorColisiones
{
public:
  GestorColisiones(){};
  void ComprobarColisiones(Corredor *pj1Col_1);
  bool JugadorCaja(Caja *cajas[]);
  bool JugadorTurbo();
  bool objetoDestruible();
  bool JugadorEstatico();

private:
  Corredor *pj1Col;
  ISceneNode *nodoA;
  ISceneNode *nodoB;
};

#endif