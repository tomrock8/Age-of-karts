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
	CorredorJugador(stringw rutaObj, vector3df pos);
	Item *actualizarItem(Item *item, int &id);
	void movimiento();
	std::string toString();

	void update();

  private:
	bool checkItem;
	//Item *actualizarItem(int &id, btDiscreteDynamicsWorld *mundo, core::list<btRigidBody *> objetos);
};

#endif /* CORREDORJUGADOR_H */
