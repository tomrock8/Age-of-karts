
#ifndef GESTORJUGADORES_H
#define GESTORJUGADORES_H

#include <iostream>
#include <string>
#include "Corredor.hpp"
#include "CorredorJugador.hpp"
#include "CorredorIA.hpp"
#include "CorredorRed.hpp"
#include <vector>

using namespace std;

class GestorJugadores {
public:

	~GestorJugadores(void);

	static GestorJugadores *getInstancia();
	vector<Corredor*> getJugadores();
	int getNumJugadores();

	void setJugadores(vector<Corredor*> pj1);

	void aumentarJugadores();
	void decrementarJugadores();

private:
	GestorJugadores(void);
	static GestorJugadores *instancia;
	vector<Corredor*> pj;
	int numJugadores;


};

#endif