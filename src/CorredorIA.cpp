#include "CorredorIA.hpp"


CorredorIA::CorredorIA(stringw rutaObj, s32 id_colision, Waypoint** arrayWaypoints, int tamanyoArray) :
	Corredor(rutaObj, id_colision) {


	//sacamos el waypoint más cercano
	float diferenciaX = 0;
	float diferenciaZ = 0;
	for (int i = 0; i < tamanyoArray; i++) {

		//puntoActual=arrayWaypoints[i];
		if (i == 0) {
			puntoActual = arrayWaypoints[i];
			diferenciaX = abs(posX - arrayWaypoints[i]->getPosicion().X);
			diferenciaZ = abs(posZ - arrayWaypoints[i]->getPosicion().Z);

		}
		else {

			if ((abs(posX - arrayWaypoints[i]->getPosicion().X)) <= diferenciaX && (abs(posZ - arrayWaypoints[i]->getPosicion().Z)) <= diferenciaZ) {
				puntoActual = arrayWaypoints[i];
				diferenciaX = abs(posX - arrayWaypoints[i]->getPosicion().X);
				diferenciaZ = abs(posZ - arrayWaypoints[i]->getPosicion().Z);
			}

		}
	}
}

//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
void CorredorIA::movimiento()
{

	bool acelerando;
	acelerando = true;
	//Cara A (NORTE) - DELANTE

	if (acelerando)
		acelerar();

	if (norte) {
		//cout << "CONDICION NORTE" << endl;
		if (posZ - (puntoActual->getPosicion().Z) > 0) {
			puntoActual = puntoActual->getNextWaypoint();
		}

		if (posX - (puntoActual->getPosicion().X) < 0) {
			girarDerecha();
		}
		else {
			girarIzquierda();
		}
	}
	else {
		if (sur) {
			//cout << "CONDICION SUR" << endl;
			if (posZ - (puntoActual->getPosicion().Z) < 0) {
				puntoActual = puntoActual->getNextWaypoint();
			}

			if (posX - (puntoActual->getPosicion().X) < 0) {
				girarIzquierda();
			}
			else {
				girarDerecha();
			}
		}
	}

	//Cara B (SUR) - ATRAS
	//Cara C (ESTE) - DRCH
	//Cara D (OESTE) - IZQ



}



std::string CorredorIA::getNombreWaypoint() {
	return puntoActual->getNombre();
}



