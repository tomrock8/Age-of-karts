#ifndef PISTA_H
#define PISTA_H

#include <vector>

#include "Item.hpp"
#include "Caja.hpp"
#include "Turbo.hpp"

class Pista {

public:

	//Destructor
	~Pista();
	void createMap(const char * path);
	// CARGA MAPA
	void loadWaypoints(const char *waypoints);
	void loadPirateMapElements();


	// METODOS SET
	void setMapa();
	void setItems(std::vector<Item *> itemMetodo);
	void setArrayCaja(std::vector<Caja*> caja);
	void setNumVueltas(int n);
	void setNombreMapa(const char *name);

	// METODOS GET
	static Pista *getInstancia();

	int getTamCajas();
	int getTamArrayWaypoints();

	std::vector<Item *> getItems();
	std::vector<Turbo*> getArrayTurbo();
	std::vector<Caja*> getArrayCaja();
	std::vector<Waypoint*> getArrayWaypoints();
	std::vector<glm::vec3> getParrilla();
	Turbo* getTurbo();
	Turbo* getTurbo(int id);
	int getNumVueltas();



private:
	Pista(void);
	static Pista *instancia;

	obj3D *Mapa;

	int tamWaypoints;
	int tamTurbos;
	int tamCajas;

	std::vector<Waypoint*> arrayWaypoints;
	std::vector<Turbo*> arrayTurbos;
	std::vector<Caja*> arrayCajas;
	std::vector<Item *> Items;
	std::vector<glm::vec3> parrilla;
	//Variables para la deteccion del mapa
	const char *nameMap;
	const char *path;
	const char *wayPoints;
	int vueltas;
};

#endif /* PISTA_H */
