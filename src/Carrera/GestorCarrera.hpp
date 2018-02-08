#ifndef GESTORCARRERA_H
#define GESTORCARRERA_H

#include "GestorJugadores.hpp"
#include "Corredor.hpp"
#include "Waypoint.hpp"
#include <vector>
using namespace std;

class GestorCarrera {
public:
	GestorCarrera();
	void update();
	//int getCorredorIndex();
	int getCorredorIndexOriginal(int n);
	void setVueltas(int i);
	int getVueltas();
	void comprobarItemTeledirigido(int num);
private:
	vector<Corredor*> pj1;
	vector<Corredor*> pj2;
	Corredor *pj_aux;
	GestorJugadores *jugadores;
	int vueltas;
};

#endif