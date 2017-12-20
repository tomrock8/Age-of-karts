#ifndef CORREDORJUGADOR_H
#define CORREDORJUGADOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Pista.hpp"
#include "Corredor.hpp"
using namespace std;

class CorredorJugador : public Corredor
{
public:

	CorredorJugador(stringw rutaObj,vector3df pos);
	Proyectil *actualizarItem(Proyectil *item,int &id);
	void movimiento();
	void update();

private:

	bool checkItem;
};

#endif /* CORREDORJUGADOR_H */
