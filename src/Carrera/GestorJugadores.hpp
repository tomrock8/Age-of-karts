
#ifndef GESTORJUGADORES_H
#define GESTORJUGADORES_H

#include <iostream>
#include <string>
#include "Corredor.hpp"
#include "CorredorRed.hpp"
#include "CorredorJugador.hpp"
#include "CorredorIA.hpp"
#include <vector>

using namespace std;

class GestorJugadores {
public:

	~GestorJugadores(void);

	static GestorJugadores *getInstancia();
	std::vector<Corredor*> getJugadores();
	int getNumJugadores();

	void setJugadores(std::vector<Corredor*> pj1);

	void aumentarJugadores();
	void decrementarJugadores();

private:
	GestorJugadores(void);
	static GestorJugadores *instancia;
	std::vector<Corredor*> pj;
	int numJugadores;


};

#endif