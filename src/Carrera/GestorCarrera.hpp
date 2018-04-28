#ifndef GESTORCARRERA_H
#define GESTORCARRERA_H

#include "GestorJugadores.hpp"
#include "Corredor.hpp"
#include <vector>

class GestorCarrera {
public:
	GestorCarrera();
	~GestorCarrera();

	bool update();
	void comprobarItemTeledirigido(int num);
	//int getCorredorIndex();

	// METODOS GET
	int getCorredorIndexOriginal(int n);
	int getVueltas();

	// METODOS SET
	void setVueltas(int i);
	
private:
	
	std::vector<Corredor*> pj1;
	std::vector<Corredor*> pj2;
	Corredor *pj_aux;
	GestorJugadores *jugadores;
	int vueltas;
	int acum;
};

#endif