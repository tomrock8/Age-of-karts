#ifndef GESTORCARRERA_H
#define GESTORCARRERA_H

#include "GestorJugadores.hpp"
#include "Corredor.hpp"
#include "Waypoint.hpp"
using namespace std;

class GestorCarrera {
public:
	GestorCarrera();
	void update();
	//int getCorredorIndex();
	int getCorredorIndexOriginal(int n);
	void setVueltas(int i);
	int getVueltas();
	bool comprobarItemTeledirigido(int num);
private:
	Corredor **pj1;
	Corredor **pj2;
	Corredor **pj3;
	Corredor *pj_aux;
	GestorJugadores *jugadores;
	int vueltas;
};

#endif