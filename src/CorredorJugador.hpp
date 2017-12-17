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

	void movimiento();
	Proyectil* actualizarItem(int &id,btDiscreteDynamicsWorld *mundo,core::list<btRigidBody *> objetos);
private:
	bool checkItem=false;

};

#endif /* CORREDORJUGADOR_H */