
#ifndef ESTADOSJUGADOR_H
#define ESTADOSJUGADOR_H

#include "IrrlichtLib.hpp"
#include "TextoPantalla.hpp"
#include <cstdlib>
#include <iostream>


using namespace std;
typedef enum { QUIETO, AVANZA, MARCHA_ATRAS, FRENA, DERRAPA } estado_movimiento;
typedef enum { RECTO, IZQUIERDA, DERECHA } direccion_movimiento;
typedef enum { NADA, FLECHA, CAJA_FALSA, TURBO, ACEITE, ESCUDO, FLECHA_TRIPLE } estado_objeto;
typedef enum { POR_DEFECTO, CON_TURBO, CON_ESCUDO } estado_coche;



class EstadosJugador
{
public:
	EstadosJugador();
	void update();
	
	// METODOS SET
	void setEstadoMovimiento(estado_movimiento estado_mov);
	void setDireccionMovimiento(direccion_movimiento direccion_mov);
	void setEstadoObjeto(estado_objeto estado_obj);
	void setEstadoCoche(estado_coche coche);

	// METODOS GET
	estado_movimiento getEstadoMovimiento();
	direccion_movimiento getDireccionMovimiento();
	estado_objeto getEstadoObjeto();

private:
	estado_movimiento estado;
	direccion_movimiento direccion;
	estado_objeto objeto;
	estado_coche estadoCoche;
};

#endif 