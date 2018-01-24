#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H

#include "Escena.hpp"
#include "EscenaJuego.hpp"
#include "EscenaMenu.hpp"
#include "EscenaCarga.hpp"
#include "EscenaLobby.hpp"

class GestorEscena {
public:

	void update(); // Actualizacion del gestor de escenas
	void cambiaEscena(Escena::tipo_escena escena); // Modifica la escena actual
	bool agregaEscena(Escena *escena); // Agrega una escena al gestor
	bool borraEscena(Escena::tipo_escena tipo); // Limpia y borra la escena del tipo seleccionado

	// METODOS GET
	static GestorEscena& instancia(); // Devuelve la instancia del singleton
	Escena **getEscenas(); // Devuelve el array de escenas posibles
	Escena *getEscena(Escena::tipo_escena escena); // Devuelve la escena con el tipo pedido

	// METODOS SET
	

private:
	static GestorEscena _instancia;  // Instancia de la escena para el singleton

	int nEscenas;		// Numero de escenas maximo que tiene el juego
	Escena **escenas;	// Array de escenas que tiene el juego

	Escena *escenaActiva; // Escena que esta activa actualmente
	bool cambioEscena;	// Indicador para saber si hay que cambiar de escena

	GestorEscena(); // Constructor privado para el constructor
	void comprobarInputs(); // Comprueba los inputs
	int indiceEscena(Escena::tipo_escena tipo); // Devuelve el indice de la escena en el array (-1 si no existe)
};

#endif /* GESTORESCENA_H */