
#ifndef ESTADOSJUGADOR_H
#define ESTADOSJUGADOR_H

#include "IrrlichtLib.hpp"
#include "TextoPantalla.hpp"
#include <cstdlib>
#include <iostream>


using namespace std;
typedef enum {QUIETO, AVANZA, MARCHA_ATRAS, FRENA, DERRAPA} estado_movimiento;
typedef enum {AVANZANDO, RETROCEDIENDO, PUNTO_MUERTO} estado_motor;
typedef enum {RECTO,IZQUIERDA, DERECHA} direccion_movimiento;
typedef enum {NADA, FLECHA, CAJA_FALSA, TURBO, ACEITE, ESCUDO, FLECHA_TRIPLE} estado_objeto;
typedef enum {POR_DEFECTO, CON_TURBO, CON_ESCUDO} estado_coche;



class EstadosJugador
{
  private:
  estado_movimiento estado;
  direccion_movimiento direccion;
  estado_objeto objeto;
  estado_coche estadoCoche;
  public:
  EstadosJugador();
  void update();
  void setEstadoMovimiento(estado_movimiento estado_mov);
  void setEstadoMovimiento(int estado_mov);
  estado_movimiento getEstadoMovimiento();
  void setDireccionMovimiento(direccion_movimiento direccion_mov);
  void setDireccionMovimiento(int direccion_mov);
  direccion_movimiento getDireccionMovimiento();
  void setEstadoObjeto(estado_objeto estado_obj);
  void setEstadoObjeto(int estado_obj);
  estado_objeto getEstadoObjeto();
  void setEstadoCoche(estado_coche coche);
  void setEstadoCoche(int coche);
  estado_coche getEstadoCoche();
};

#endif 