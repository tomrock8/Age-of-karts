#include "GestorEscena.hpp"


GestorEscena::GestorEscena() {
	cambioEscena = Escena::tipo_escena::MENU; // Indicador que tiene que cambiar de escena
	escenas = new Escena*[Escena::nTipos]; // Array que contendra todas las escenas posibles

	// Inicializamos todas las posibles escenas a null
	for (int i = 0; i < Escena::nTipos; i++) {
		escenas[i] = nullptr;
	}
}

Escena::tipo_escena GestorEscena::update() {

	//Se comprueban los inputs de la escena activa
	cambioEscena = escenaActiva->comprobarInputs();

	if (cambioEscena == escenaActiva->getTipoEscena()) {
		//Se actualiza la escena
		escenaActiva->update();
		//Y se dibuja la escena
		escenaActiva->dibujar();
	}
	else { //La escena ha cambiado a otra
		if (cambioEscena != Escena::tipo_escena::SALIR)
			cambiaEscena(cambioEscena);
	}

	return cambioEscena; // Por si hay que salir, devolver la escena salir. En cualquier otro caso obviar.
}


bool GestorEscena::cambiaEscena(Escena::tipo_escena tipo) {
	Escena *anterior = escenaActiva; // Guardar escena anterior para tratar con ella si es necesario
	std::string ipConexion = "";
	std::vector<Corredor::tipo_jugador> jugadores;

	if (anterior) { // Hay una escena activa actualmente
		if (anterior->getTipoEscena() == Escena::tipo_escena::LOBBY && tipo == Escena::tipo_escena::ONLINE) {
			ipConexion = static_cast<EscenaLobby*>(anterior)->getIpConexion();
			cout << ipConexion << "\n";
		}
		if (anterior->getTipoEscena() == Escena::tipo_escena::ONLINE && tipo == Escena::tipo_escena::LOBBY) {
			ipConexion = static_cast<EscenaJuego*>(anterior)->getIpConexion();
			cout << ipConexion << "\n";
		}
		if (anterior->getTipoEscena() == Escena::tipo_escena::MENU && tipo == Escena::tipo_escena::LOBBY) {
			ipConexion = static_cast<EscenaMenu*>(anterior)->getIpConexion();
			cout << "De menu a lobby: " << ipConexion << "\n";
		}

		if (tipo == Escena::tipo_escena::PODIO) {
			ipConexion = static_cast<EscenaJuego*>(anterior)->getIpConexion();
			jugadores = static_cast<EscenaJuego*>(anterior)->getJugadores();
			cout << "De menu a lobby: " << ipConexion << "\n";
		}

		escenaActiva->limpiar();
		escenaActiva = nullptr; // Desasignamos la escena activa
		borraEscena(anterior->getTipoEscena());
	}

	// No hay una escena activa, de manera que se crea una nueva
	return nuevaEscena(tipo, ipConexion, jugadores);
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

bool GestorEscena::nuevaEscena(Escena::tipo_escena tipo, std::string ipConexion, std::vector<Corredor::tipo_jugador> jugadores) {
	// Crea una escena del tipo especificado
	// Agregar la escena al array

	// Buscar si la nueva escena ya esta creada
	// Crear si hace falta la nueva escena (porque no existe)
	// Asignar la nueva escena a activa
	if (getEscena(tipo)) {// Comprobar que no existe ya la escena
		borraEscena(tipo); // Ya hay una escena de ese tipo y es borrada
	}

	delete TMotor::instancia().getSceneNode(); // Vacia el arbol completo del motor
	TMotor::instancia().cleanScene();
	TMotor::instancia().nuevaEscenaRaiz(); // Crea la escena raiz de donde cuelgan todos los nodos


	switch (tipo) { // Crear la escena dependiendo del tipo
	case Escena::tipo_escena::CARGA:
		//escenaActiva = new EscenaCarga(Escena::tipo_escena::CARGA);
		return false;

	case Escena::tipo_escena::ONLINE:
		escenaActiva = new EscenaJuego(Escena::tipo_escena::ONLINE, ipConexion); // Se crea la nueva escena
		agregaEscena(escenaActiva); // Se agrega la escena nueva al array de escenas
		cambioEscena = Escena::tipo_escena::ONLINE; // Activamos el indicador para cambiar de escena
		return true; // Devolvemos true y terminamos

	case Escena::tipo_escena::CARRERA:
		escenaActiva = new EscenaJuego(Escena::tipo_escena::CARRERA); // Se crea la nueva escena
		agregaEscena(escenaActiva); // Se agrega la escena nueva al array de escenas
		cambioEscena = Escena::tipo_escena::CARRERA; // Activamos el indicador para cambiar de escena
		return true; // Devolvemos true y terminamos

	case Escena::tipo_escena::CREDITOS:
		escenaActiva = new EscenaCreditos();
		agregaEscena(escenaActiva);
		cambioEscena = Escena::tipo_escena::CREDITOS;
		return true;

	case Escena::tipo_escena::LOBBY:
		escenaActiva = new EscenaLobby(Escena::tipo_escena::LOBBY, ipConexion);
		agregaEscena(escenaActiva);
		cambioEscena = Escena::tipo_escena::LOBBY;

		return true;

	case Escena::tipo_escena::MENU:
		escenaActiva = new EscenaMenu();
		agregaEscena(escenaActiva);
		cambioEscena = Escena::tipo_escena::MENU;
		return true;

	case Escena::tipo_escena::OPCIONES:
		escenaActiva = new EscenaOpciones(Escena::tipo_escena::OPCIONES);
		agregaEscena(escenaActiva);
		cambioEscena = Escena::tipo_escena::OPCIONES;

		return true;

	case Escena::tipo_escena::PODIO:
		escenaActiva = new EscenaPodio(Escena::tipo_escena::PODIO, jugadores);
		agregaEscena(escenaActiva);
		cambioEscena = Escena::tipo_escena::PODIO;

		return true;


	}

	return false;
}

bool GestorEscena::borraEscena(Escena::tipo_escena tipo) {
	if (!getEscena(tipo)) // Comprobamos que exista la escena en el array
		return false; // No existe la escena y terminamos

	
	int indice = indiceEscena(tipo); // Recogemos el indice donde se encuentra el tipo de escena

	if (tipo == Escena::tipo_escena::CARRERA || tipo == Escena::tipo_escena::ONLINE) {
		EscenaJuego *e = static_cast<EscenaJuego *>(escenas[indice]); // Convertimos la escena en su tipo

		delete e; // Eliminamos la escena que hemos recogido del array
		escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
		return true;
	}

	if (tipo == Escena::tipo_escena::MENU) {
		EscenaMenu *e = static_cast<EscenaMenu *>(escenas[indice]);// Convertimos la escena en su tipo
		delete e; // Eliminamos la escena que hemos recogido del array
		escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
		return true;
	}

	if (tipo == Escena::tipo_escena::CREDITOS) {
		EscenaCreditos *e = static_cast<EscenaCreditos *>(escenas[indice]);// Convertimos la escena en su tipo
		escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
		return true;
	}

	if (tipo == Escena::tipo_escena::OPCIONES) {
		EscenaOpciones *e = static_cast<EscenaOpciones *>(escenas[indice]);// Convertimos la escena en su tipo
		delete e; // Eliminamos la escena que hemos recogido del array
		escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
		return true;
	}

	if (tipo == Escena::tipo_escena::LOBBY) {
		EscenaLobby *e = static_cast<EscenaLobby *>(escenas[indice]);// Convertimos la escena en su tipo
		delete e; // Eliminamos la escena que hemos recogido del array
		escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
		return true;
	}

	if (tipo == Escena::tipo_escena::PODIO) {
		EscenaLobby *e = static_cast<EscenaLobby *>(escenas[indice]);// Convertimos la escena en su tipo
		delete e; // Eliminamos la escena que hemos recogido del array
		escenas[indice] = nullptr; // Ponemos el indice del array que hemos borrado a null
		return true;
	}


	return false;
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