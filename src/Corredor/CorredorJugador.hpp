#ifndef CORREDORJUGADOR_H
#define CORREDORJUGADOR_H

#include <iostream>
//#include "IrrlichtLib.hpp"
#include "Pista.hpp"
#include "Corredor.hpp"
#include "Turbo.hpp"
#include "Caja.hpp"
#include "Graphics.hpp"
using namespace std;

class CorredorJugador : public Corredor
{
  public:
	CorredorJugador(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo);
	void actualizarItem();
	void movimiento();
	std::string toString();
	bool setComprobadorMovimiento(bool s);
	void update();

  private:
	//objetos estaticos y dinamicos
	bool comprobadorMovimiento;
	//Item *actualizarItem(int &id, btDiscreteDynamicsWorld *mundo, core::list<btRigidBody *> objetos);
};

#endif /* CORREDORJUGADOR_H */
