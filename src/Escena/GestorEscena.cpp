#include "GestorEscena.hpp"

GestorEscena *GestorEscena::instancia = nullptr;

GestorEscena::GestorEscena() {
	cambioEscena = true;
}


GestorEscena::~GestorEscena() {
}

void GestorEscena::update() {
	escenaActiva->update();
}

void GestorEscena::cambiaEscena(Escena::tipo_escena nueva) {
	// Guardar escena anterior
	Escena *anterior = escenaActiva;

	// Limpiar escena anterior
	anterior->limpiar();

	// Borrar la escena si es necesario
	// NUEVA         ANTERIOR
	// MENU		DESDE LOBBY		-> BORRAR LOBBY
	// MENU		DESDE CARRERA	-> BORRAR CARRERA
	// CARGA	DESDE LOBBY		-> BORRAR LOBBY
	// CARRERA	DESDE CARGA		-> BORRAR CARGA

	if (nueva == Escena::tipo_escena::MENU) {
		if (anterior->getTipoEscena() == Escena::tipo_escena::LOBBY) {
			// Borrar LOBBY
		}
		else {
			if (anterior->getTipoEscena() == Escena::tipo_escena::CARRERA) {
				// Borrar Carrera
			}
		}
	}
	else {
		if (nueva == Escena::tipo_escena::CARGA && anterior->getTipoEscena() == Escena::tipo_escena::LOBBY) {
			// Borrar LOBBY
		}
		else {
			if (nueva == Escena::tipo_escena::CARRERA && anterior->getTipoEscena() == Escena::tipo_escena::CARGA) {
				// Borrar Carga
			}
		}
	}

	// Buscar si la nueva escena ya esta creada

	// Crear si hace falta la nueva escena (porque no existe)

	// Asignar la nueva escena a activa

}

GestorEscena *GestorEscena::getInstancia() {
	if (!instancia) {
		instancia = new GestorEscena();
		//EscenaMenu
		instancia->cambiaEscena(Escena::tipo_escena::MENU);
	}

	return instancia;
}

Escena *GestorEscena::getEscena(Escena::tipo_escena escena) {
	return nullptr;
}

bool GestorEscena::agregaEscena(Escena *escena) {

}

void GestorEscena::comprobarInputs() {

}
