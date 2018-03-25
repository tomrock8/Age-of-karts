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
#include <vector>
#include <time.h>
#include "Timer.hpp"
using namespace std;

class GestorColisiones
{
public:
  GestorColisiones(){};
  void ComprobarColisiones();
  bool JugadorCaja(vector<Caja*> cajas);
  bool JugadorTurbo();
  bool JugadorEstatico();
  bool JugadorWaypoint();
  bool JugadorProyectil();
  bool ItemTeledirigidoWaypoint();
  bool Escudoitems();
  bool HabilidadesItems();
  bool HabilidadesJugadores();
  bool habilidadVShabilidad();
  void IniciarTimer();

  //bool JugadorItemTeledirigido();

private:
  vector<Corredor*> pj1;
  GestorJugadores *jugadores;
  ISceneNode *nodoA;
  ISceneNode *nodoB;
  clock_t tiempoLanzado;
  bool control;
  clock_t tiempoInicio;
};

#endif