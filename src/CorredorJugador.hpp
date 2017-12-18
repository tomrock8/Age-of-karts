#ifndef CORREDORJUGADOR_H
#define CORREDORJUGADOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
using namespace std;

class CorredorJugador : public Corredor
{
public:

	CorredorJugador(stringw rutaObj,vector3df pos);
	void actualizarItem(Proyectil *item,int &id);
	void movimiento();
	void update();

private:

	bool checkItem;
};

#endif /* CORREDORJUGADOR_H */
