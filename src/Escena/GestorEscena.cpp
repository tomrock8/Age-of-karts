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
	if (nueva == Escena::tipo_escena::MENU) {
		// MENU		DESDE LOBBY		-> BORRAR LOBBY
		if (anterior->getTipoEscena() == Escena::tipo_escena::LOBBY) {
			// Borrar LOBBY
		}
		else {
			// MENU		DESDE CARRERA	-> BORRAR CARRERA
			if (anterior->getTipoEscena() == Escena::tipo_escena::CARRERA) {
				// Borrar Carrera
			}
		}
	}
	else {
		// CARGA	DESDE LOBBY		-> BORRAR LOBBY
		if (nueva == Escena::tipo_escena::CARGA && anterior->getTipoEscena() == Escena::tipo_escena::LOBBY) {
			// Borrar LOBBY
		}
		else {
			// CARRERA	DESDE CARGA		-> BORRAR CARGA
			if (nueva == Escena::tipo_escena::CARRERA && anterior->getTipoEscena() == Escena::tipo_escena::CARGA) {
				// Borrar Carga
			}
		}
	}

	// Buscar si la nueva escena ya esta creada
	// Crear si hace falta la nueva escena (porque no existe)
	// Asignar la nueva escena a activa
	if (!getEscena(nueva)) {
		switch (nueva) {
		case Escena::tipo_escena::CARGA:
			//escenaActiva = new EscenaCarga(Escena::tipo_escena::CARGA);
			break;

		case Escena::tipo_escena::CARRERA:
			escenaActiva = new EscenaJuego(Escena::tipo_escena::CARGA);
			break;

		case Escena::tipo_escena::CREDITOS:
			//escenaActiva = new EscenaCreditos(Escena::tipo_escena::CREDITOS);
			break;

		case Escena::tipo_escena::LOBBY:
			//escenaActiva = new EscenaLobby(Escena::tipo_escena::LOBBY);
			break;

		case Escena::tipo_escena::MENU:
			//escenaActiva = new EscenaMenu(Escena::tipo_escena::MENU);
			break;

		case Escena::tipo_escena::OPCIONES:
			//escenaActiva = new EscenaOpciones(Escena::tipo_escena::OPCIONES);
			break;
		}
	}
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
