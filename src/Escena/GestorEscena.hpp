#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H

#include "Escena.hpp"
#include "EscenaJuego.hpp"
#include "EscenaMenu.hpp"
#include "EscenaCarga.hpp"
#include "EscenaLobby.hpp"

class GestorEscena {
public:

	~GestorEscena(); // Destructor de la clase

	void update(); // Actualizacion del gestor de escenas
	void cambiaEscena(Escena::tipo_escena escena); // Modifica la escena actual

	// METODOS GET
	static GestorEscena *getInstancia(); // Devuelve la instancia del singleton
	Escena *getEscena(Escena::tipo_escena escena); // Devuelve la escena con el tipo pedido

	// METODOS SET
	bool agregaEscena(Escena *escena); // Agrega una escena al gestor

private:
	static GestorEscena *instancia;  // Instancia de la escena para el singleton

	int nEscenas;		// Numero de escenas maximo que tiene el juego
	Escena **escenas;	// Array de escenas que tiene el juego

	Escena *escenaActiva; // Escena que esta activa actualmente
	bool cambioEscena;

	GestorEscena(); // Constructor privado para el constructor
	void comprobarInputs(); // Comprueba los inputs

};

#endif /* GESTORESCENA_H */