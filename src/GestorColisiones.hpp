#ifndef GESTORCOLISIONES_H
#define GESTORCOLISIONES_H

#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "Caja.hpp"
#include "Item.hpp"
#include "Proyectil.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "MotorFisicas.hpp"
#include <iostream>

using namespace std;

class GestorColisiones
{
  public:
    GestorColisiones(){};
    void ComprobarColisiones(Corredor *pj1Col_1, Caja *cajas[], Proyectil *item_1);
    void JugadorCaja(Caja *cajas[]);
    void JugadorTurbo();
    void objetoDestruible();

  private:
  Corredor *pj1Col;
  Proyectil *item;
  ISceneNode *nodoA;
  ISceneNode *nodoB;
  
};

#endif