#ifndef GESTORESCENA_H
#define GESTORESCENA_H

#include "Escena.hpp"
#include "Corredor.hpp"

#include <iostream>
#include <string>
#include <vector>

class GestorEscena {
public:

	~GestorEscena();

	Escena::tipo_escena update(); // Actualizacion del gestor de escenas
	bool cambiaEscena(Escena::tipo_escena escena); // Modifica la escena actual
	bool borraEscena(Escena::tipo_escena tipo); // Limpia y borra la escena del tipo seleccionado

	// METODOS GET

	// Devuelve la instancia del singleton
	static GestorEscena& instancia();
	// Devuelve la escena que esta activa actualmente
	Escena& getEscenaActiva();
	// Devuelve el array de escenas posibles
	Escena** getEscenas();
	// Devuelve la escena con el tipo pedido
	Escena* getEscena(Escena::tipo_escena escena);

	// METODOS SET


private:
	Escena** escenas;	// Array de escenas que tiene el juego

	Escena* escenaActiva; // Escena que esta activa actualmente
	Escena::tipo_escena cambioEscena;	// Indicador para saber si hay que cambiar de escena


	float tiempoAnterior;
	time_t tiempoActual;
	int cont;

	// METODOS
	GestorEscena(); // Constructor privado para el constructor
	bool comprobarInputs(); // Comprueba los inputs
	bool agregaEscena(Escena* escena); // Agrega una escena al gestor
	int indiceEscena(Escena::tipo_escena tipo); // Devuelve el indice de la escena en el array (-1 si no existe)
	int indiceVacio(); // Devuelve la primera posicion donde se pueda guardar una escena nueva
	bool nuevaEscena(Escena::tipo_escena tipo, std::string ipConexion, std::vector<Corredor::tipo_jugador> jugadores); // Metodo para crear una nueva escena
};

#endif /* GESTORESCENA_H */