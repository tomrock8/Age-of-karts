
#ifndef ESTADOSJUGADOR_H
#define ESTADOSJUGADOR_H

#include <cstdlib>
#include <iostream>

class EstadosJugador
{

public:
	enum estado_movimiento { QUIETO, AVANZA, MARCHA_ATRAS, FRENA, DERRAPA, DESACELERA };
	enum direccion_movimiento { RECTO, IZQUIERDA, DERECHA };
	enum estado_objeto { NADA, FLECHA, CAJA_FALSA, TURBO, ACEITE, ESCUDO, FLECHA_TRIPLE, ITEM_TELEDIRIGIDO, TURBO_TRIPLE, HABILIDAD };
	enum estado_coche { POR_DEFECTO, CON_TURBO, CON_ESCUDO };
	enum estado_carrera { PARRILLA, CARRERA, FIN };
	enum estado_inmunidad { NORMAL, INMUNIDAD };
	enum estado_habilidad { SINHABILIDAD, CONHABILIDAD };

	EstadosJugador();
	void update();

	// Movimiento
	void setEstadoMovimiento(estado_movimiento estado_mov);
	void setEstadoMovimiento(int estado_mov);
	estado_movimiento getEstadoMovimiento();

	// Direccion
	void setDireccionMovimiento(direccion_movimiento direccion_mov);
	void setDireccionMovimiento(int direccion_mov);
	direccion_movimiento getDireccionMovimiento();

	// Objetos
	void setEstadoObjeto(estado_objeto estado_obj);
	void setEstadoObjeto(int estado_obj);
	estado_objeto getEstadoObjeto();

	// Coche
	void setEstadoCoche(estado_coche coche);
	void setEstadoCoche(int coche);
	estado_coche getEstadoCoche();

	// Estado de la carrera
	void setEstadoCarrera(estado_carrera carrera);
	void setEstadoCarrera(int carrera);
	estado_carrera getEstadoCarrera();

	// Inmunidad
	void setEstadoInmunidad(estado_inmunidad inmunidad);
	void setEstadoInmunidad(int inmunidad);
	estado_inmunidad getEstadoInmunidad();

	// Habilidad especial de cada corredor
	void setEstadoHabilidad(estado_habilidad Habilidad);
	void setEstadoHabilidad(int Habilidad);
	estado_habilidad getEstadoHabilidad();

private:
	estado_movimiento estado;
	direccion_movimiento direccion;
	estado_objeto objeto;
	estado_coche estadoCoche;
	estado_carrera estadoCarrera;
	estado_inmunidad Inmunidad;
	estado_habilidad habilidad;
};

#endif 