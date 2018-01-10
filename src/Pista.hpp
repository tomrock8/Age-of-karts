#ifndef PISTA_H
#define PISTA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "Item.hpp"
#include <string>
#include <fstream>
#include "Waypoint.hpp"
#include "Turbo.hpp"
#include "Caja.hpp"

using namespace std;

class Pista
{

public:
	//Destructor
	~Pista(void);
	
	static Pista *getInstancia();
	void InicializarFisicas();
	void BorrarFisicas();
	
	// METODOS SET
	void setMapa(stringw mapa, const char *fisicas, const char *waypoint);
	void setItems(irr::core::list<Item *> itemMetodo);

	// METODOS GET
	core::list<Item *> getItems();
	Turbo **getArrayTurbo();
	Caja **getArrayCaja();
	void setArrayCaja(Caja **cajas);
	int getTamCajas();
	Waypoint **getArrayWaypoints();
	Turbo* getTurbo();
	Turbo* getTurbo(int id);

private:
	Pista(void);
	static Pista *instancia;
	
	//MUNDO
	btRigidBody *CuerpoColisionMapa;
	btCollisionShape *FormaColision;
	btDefaultMotionState *MotionState; // posicion del cuerpo de colision
	
	int tamWaypoints;
	Waypoint **arrayWaypoints;

	int tamTurbos;
	Turbo    **arrayTurbos;
	
	int tamCajas;
	Caja     **arrayCajas;
	
	btScalar Masa;
	ISceneNode *Mapa;

	irr::core::list<Item *> Items;
};

#endif /* PISTA_H */
