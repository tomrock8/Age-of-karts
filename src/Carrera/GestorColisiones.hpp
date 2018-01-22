#ifndef GESTORCOLISIONES_H
#define GESTORCOLISIONES_H

#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "CorredorJugador.hpp"
#include "Caja.hpp"
#include "Item.hpp"
#include "Proyectil.hpp"
#include "Pista.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "MotorFisicas.hpp"
#include "GestorJugadores.hpp"
#include <iostream>

using namespace std;

class GestorColisiones
{
public:
  GestorColisiones(){};
  void ComprobarColisiones();
  bool JugadorCaja(Caja *cajas[]);
  bool JugadorTurbo();
  bool objetoDestruible();
  bool JugadorEstatico();
  bool JugadorWaypoint();


private:
  Corredor **pj1;
  ISceneNode *nodoA;
  ISceneNode *nodoB;
};

#endif