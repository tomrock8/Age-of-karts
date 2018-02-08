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
#include "BulletWorldImporter/btBulletWorldImporter.h"

using namespace std;

class Pista
{

public:
	//Destructor
	~Pista();
	
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
	int getTamArrayWaypoints();

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
	
	ISceneNode *Mapa;

	irr::core::list<Item *> Items;


	//fisicas del mapa
	btBulletWorldImporter *fisicasMapa;
};

#endif /* PISTA_H */
