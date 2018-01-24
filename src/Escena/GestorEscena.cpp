#include "GestorEscena.hpp"

GestorEscena GestorEscena::_instancia;

GestorEscena::GestorEscena() {
	cambioEscena = true; // Indicador que tiene que cambiar de escena
	nEscenas = 6; // Numero total de escenas posibles (una de cada tipo)
	escenas = new Escena*[nEscenas]; // Array que contendra todas las escenas posibles

	// Inicializamos todas las posibles escenas a null
	for (int i = 0; i < nEscenas; i++) {
		escenas[i] = nullptr;
	}
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


GestorEscena& GestorEscena::instancia() {
	return _instancia;
}

Escena **GestorEscena::getEscenas() {
	return escenas;
}

Escena *GestorEscena::getEscena(Escena::tipo_escena escena) {
	if (escenas) {
		for (int i = 0; i < nEscenas; i++) {
			if (escenas[i]) {
				if (escenas[i]->getTipoEscena() == escena) {
					return escenas[i];
				}
			}
		}
	}
	return nullptr;
}

bool GestorEscena::agregaEscena(Escena *escena) {
	Escena::tipo_escena tipo = escena->getTipoEscena(); // Tipo de escena en la que trabajamos
	// Escena para comprobar si existe ya una escena de ese tipo
	if (getEscena(tipo)) { // Ya existe una escena del tipo
		cout << "Ya hay una escena creada de ese tipo y va a ser borrada";
		// Borramos escena ya existente
		borraEscena(tipo);	// Se borra la escena ya existente
	}
	
	// Buscar el indice donde se va a guardar la escena en el array
	for (int i = 0; i < nEscenas; i++) {
		if (!escenas[i]) { // La posicion del array esta vacia
			escenas[i] = escena; // Asignamos al array de escenas el puntero de la nueva escena
			return true; // La escena ha sido agregada al array y termina el proceso correctamente
		}
	}

	return false; // No ha encontrado una posicion donde guardar la escena y termina el proceso
}

bool GestorEscena::borraEscena(Escena::tipo_escena tipo) {
	if (!getEscena(tipo)) // Comprobamos que exista la escena en el array
		return false; // No existe la escena y terminamos

	int indice = indiceEscena(tipo); // Recogemos el indice donde se encuentra el tipo de escena
	delete escenas[indice]; // Eliminamos la escena que hemos recogido del array
	escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
	return true; // Terminamos el proceso correctamente y devolvemos true
}

int GestorEscena::indiceEscena(Escena::tipo_escena tipo) {
	for (int i = 0; i < nEscenas; i++) {
		if (escenas) { // Comprobamos que el array no es null
			if (escenas[i]) { // Comprobamos que el elemento no es null
				if (escenas[i]->getTipoEscena() == tipo) { // Comprobamos que es el mismo tipo de escena
					return i; // Devolvemos el indice donde esta el tipo de escena que buscamos
				}
			}
		}
	}

	return -1; // La escena no ha sido encontrada y devolvemos -1
}