#ifndef PISTA_H
#define PISTA_H

#include <iostream>
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

	// METODOS SET
	void setMapa(const char* name);
	void createMap(const char * path);
	// CARGA MAPA
	void loadWaypoints(const char *waypoints);
	void setItems(std::vector<Item *> itemMetodo);
	// METODOS GET
	std::vector<Item *> getItems();
	void loadPirateMapElements();
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
	int tamWaypoints;
	std::vector<Waypoint*> arrayWaypoints;
	int tamTurbos;
	std::vector<Turbo*> arrayTurbos;
	int tamCajas;
	std::vector<Caja*> arrayCajas;
	obj3D *Mapa;
	std::vector<Item *> Items;
	//Variables para la deteccion del mapa
	const char *nameMap;
	const char *path;
	const char *wayPoints;
};

#endif /* PISTA_H */
