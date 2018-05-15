#ifndef CORREDORJUGADOR_H
#define CORREDORJUGADOR_H

#include <iostream>
#include "Pista.hpp"
#include "Corredor.hpp"
#include "Turbo.hpp"
#include "Caja.hpp"

using namespace std;

class CorredorJugador : public Corredor {
public:
	CorredorJugador(btVector3 pos, Corredor::tipo_jugador tipo);
	virtual void movimiento() override;
	virtual void updateHijos() override;
	virtual void actualizarItem() override;
	std::string toString();
	bool setComprobadorMovimiento(bool s);
	void update();

private:
	//objetos estaticos y dinamicos
	bool comprobadorMovimiento;
	bool pressed;
	//Item *actualizarItem(int &id, btDiscreteDynamicsWorld *mundo, core::list<btRigidBody *> objetos);
	//controlador para las animaciones
	bool giroDer;
	bool giroIzq;
	TNodo *animacionPrevia;
};

#endif /* CORREDORJUGADOR_H */
