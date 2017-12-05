#ifndef CORREDORIA_H
#define CORREDORIA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"

using namespace std;

class CorredorIA : public Corredor
{
public:
	CorredorIA(stringw rutaObj, s32 id_colision, Waypoint** arrayWaypoints, int tamanyoArray);

	void movimiento();

	std::string getNombreWaypoint();
private:
	Waypoint * puntoActual;
};

#endif /* CORREDORIA_H */