#include "GestorEscena.hpp"
#include <iostream>

GestorEscena::GestorEscena() {
	cambioEscena = Escena::tipo_escena::MENU; // Indicador que tiene que cambiar de escena
	escenas = new Escena*[Escena::nTipos]; // Array que contendra todas las escenas posibles

	// Inicializamos todas las posibles escenas a null
	for (int i = 0; i < Escena::nTipos; i++) {
		escenas[i] = nullptr;
	}
}

Escena::tipo_escena GestorEscena::update() {
	//LIMITAR UPDATE 30
	cambioEscena = escenaActiva->comprobarInputs();
	if (cambioEscena == escenaActiva->getTipoEscena()) {
		escenaActiva->update();
		escenaActiva->dibujar();
	}

	return cambioEscena;
}


bool GestorEscena::cambiaEscena(Escena::tipo_escena tipo) {
	Escena *anterior = escenaActiva; // Guardar escena anterior para tratar con ella si es necesario

	if (anterior) { // Hay una escena activa actualmente
		escenaActiva = nullptr; // Desasignamos la escena activa

		// Proceso de borrado la escena antigua en caso que sea necesario
		if (tipo == Escena::tipo_escena::MENU) {
			// MENU		DESDE LOBBY		-> BORRAR LOBBY
			if (anterior->getTipoEscena() == Escena::tipo_escena::LOBBY) {
				borraEscena(anterior->getTipoEscena());// Borrar LOBBY
			}
			else {
				// MENU		DESDE CARRERA	-> BORRAR CARRERA
				if (anterior->getTipoEscena() == Escena::tipo_escena::CARRERA) {
					borraEscena(anterior->getTipoEscena()); // Borrar Carrera
				}
			}
		}
		else {
			// CARGA	DESDE LOBBY		-> BORRAR LOBBY
			if (tipo == Escena::tipo_escena::CARGA && anterior->getTipoEscena() == Escena::tipo_escena::LOBBY) {
				borraEscena(anterior->getTipoEscena()); // Borrar LOBBY
			}
			else {
				// CARRERA	DESDE CARGA		-> BORRAR CARGA
				if (tipo == Escena::tipo_escena::CARRERA && anterior->getTipoEscena() == Escena::tipo_escena::CARGA) {
					borraEscena(anterior->getTipoEscena()); // Borrar Carga
				}
			}
		}
	}

	// No hay una escena activa, de manera que se crea una nueva
	return nuevaEscena(tipo);
}


GestorEscena& GestorEscena::instancia() {
	static GestorEscena _instancia;  // Instancia de la escena para el singleton
	return _instancia;
}


Escena& GestorEscena::getEscenaActiva() {
	return *escenaActiva;
}


Escena **GestorEscena::getEscenas() {
	return escenas;
}

Escena *GestorEscena::getEscena(Escena::tipo_escena tipo) {
	int indice = indiceEscena(tipo); // Recogemos el indice de la escena donde se encuentra en el array
	if (indice != -1) // Comprobamos si hay alguna escena de ese tipo en el array
		return escenas[indice]; // Hay una escena de ese tipo en el array y se devuelve

	return nullptr; // No se ha encontrado ninguna escena de ese tipo en el array de escenas y se devuelve null
}

bool GestorEscena::agregaEscena(Escena *escena) {
	Escena::tipo_escena tipo = escena->getTipoEscena(); // Tipo de escena en la que trabajamos

	int indice = indiceEscena(tipo); // Escena para comprobar si existe ya una escena de ese tipo

	if (indice != -1) { // Ya existe una escena del tipo
		cout << "Ya hay una escena creada de ese tipo y va a ser borrada";
		borraEscena(tipo);	// Se borra la escena ya existente
		escenas[indice] = escena; // Se almacena en el array de escena la nueva escena
		return true; // La escena ha sido agregada al array y termina el proceso correctamente
	}
	else {
		indice = indiceVacio(); // Cogemos la primera posicion vacia
		escenas[indice] = escena; // Se almacena en el array de escena la nueva escena
		return true; // La escena ha sido agregada al array y termina el proceso correctamente
	}

	return false;
}

bool GestorEscena::nuevaEscena(Escena::tipo_escena tipo) {
	// Crea una escena del tipo especificado
	// Agregar la escena al array

	// Buscar si la nueva escena ya esta creada
	// Crear si hace falta la nueva escena (porque no existe)
	// Asignar la nueva escena a activa
	if (getEscena(tipo))  // Comprobar que no existe ya la escena
		borraEscena(tipo); // Ya hay una escena de ese tipo y es borrada

	switch (tipo) { // Crear la escena dependiendo del tipo
	case Escena::tipo_escena::CARGA:
		//escenaActiva = new EscenaCarga(Escena::tipo_escena::CARGA);
		return false;

	case Escena::tipo_escena::CARRERA:
		escenaActiva = new EscenaJuego(Escena::tipo_escena::CARRERA); // Se crea la nueva escena
		agregaEscena(escenaActiva); // Se agrega la escena nueva al array de escenas
		cambioEscena = Escena::tipo_escena::CARRERA; // Activamos el indicador para cambiar de escena
		return true; // Devolvemos true y terminamos

	case Escena::tipo_escena::CREDITOS:
		//escenaActiva = new EscenaCreditos(Escena::tipo_escena::CREDITOS);
		return false;

	case Escena::tipo_escena::LOBBY:
		//escenaActiva = new EscenaLobby(Escena::tipo_escena::LOBBY);
		return false;

	case Escena::tipo_escena::MENU:
		escenaActiva = new EscenaMenu(Escena::tipo_escena::MENU);
		agregaEscena(escenaActiva);
		cambioEscena = Escena::tipo_escena::MENU;
		return false;

	case Escena::tipo_escena::OPCIONES:
		//escenaActiva = new EscenaOpciones(Escena::tipo_escena::OPCIONES);
		return false;
	}

	return false;
}

bool GestorEscena::borraEscena(Escena::tipo_escena tipo) {
	if (!getEscena(tipo)) // Comprobamos que exista la escena en el array
		return false; // No existe la escena y terminamos

	int indice = indiceEscena(tipo); // Recogemos el indice donde se encuentra el tipo de escena

	if (tipo == Escena::tipo_escena::CARRERA) {
		EscenaJuego *e = static_cast<EscenaJuego *>(escenas[indice]); // Convertimos la escena en su tipo
		delete e; // Eliminamos la escena que hemos recogido del array
	}

	escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
	return true; // Terminamos el proceso correctamente y devolvemos true
}

int GestorEscena::indiceEscena(Escena::tipo_escena tipo) {
	if (escenas) { // Comprobamos que el array no es null
		for (int i = 0; i < Escena::nTipos; i++) {
			if (escenas[i]) { // Comprobamos que el elemento del array no es null
				if (escenas[i]->getTipoEscena() == tipo) { // Comprobamos que es el mismo tipo de escena
					return i; // Devolvemos el indice donde esta el tipo de escena que buscamos
				}
			}
		}
	}

	return -1; // La escena no ha sido encontrada y devolvemos -1
}

int GestorEscena::indiceVacio() {
	if (escenas) { // Comprobamos que el array no es null
		for (int i = 0; i < Escena::nTipos; i++) {
			if (!escenas[i]) // Comprobamos que el elemento del array es null
				return i; // Devolvemos el indice donde esta el hueco
		}
	}

	return -1; // La escena no ha sido encontrada y devolvemos -1
}