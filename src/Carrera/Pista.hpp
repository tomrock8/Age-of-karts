#ifndef PISTA_H
#define PISTA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "MotorFisicas.hpp"
#include "Item.hpp"
#include <string>
#include <fstream>
#include <vector>
#include "Waypoint.hpp"
#include "Turbo.hpp"
#include "Caja.hpp"
#include "BulletWorldImporter/btBulletWorldImporter.h"
#include "TMotor.hpp"


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
	void setItems(std::vector<Item *> itemMetodo);
	// METODOS GET
	std::vector<Item *> getItems();
	std::vector<Turbo*> getArrayTurbo();
	std::vector<Caja*> getArrayCaja();
	void setArrayCaja(std::vector<Caja*> caja);
	int getTamCajas();
	std::vector<Waypoint*> getArrayWaypoints();
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
	std::vector<Waypoint*> arrayWaypoints;

	int tamTurbos;
	std::vector<Turbo*> arrayTurbos;
	
	int tamCajas;
	std::vector<Caja*> arrayCajas;
	
	obj3D *Mapa;

	std::vector<Item *> Items;


	//fisicas del mapa
	btBulletWorldImporter *fisicasMapa;
};

#endif /* PISTA_H */
