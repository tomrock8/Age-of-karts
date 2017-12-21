#include "CorredorIA.hpp"

CorredorIA::CorredorIA(stringw rutaObj, vector3df pos, Waypoint **arrayWaypoints, int tamanyoArray) : Corredor(rutaObj, pos)
{

	
	//sacamos el waypoint m�s cercano
	float diferenciaX = 0;
	float diferenciaZ = 0;
	
	for (int i = 0; i < tamanyoArray; i++) {

		//puntoActual=arrayWaypoints[i];
		if (i == 0) {
			puntoActual = arrayWaypoints[i];
			diferenciaX = abs(posicion.getX() - arrayWaypoints[i]->getPosicion().X);
			diferenciaZ = abs(posicion.getZ() - arrayWaypoints[i]->getPosicion().Z);

		}else {

			if ((abs(posicion.getX() - arrayWaypoints[i]->getPosicion().X)) <= diferenciaX && (abs(posicion.getZ() - arrayWaypoints[i]->getPosicion().Z)) <= diferenciaZ) {
				puntoActual = arrayWaypoints[i];
				diferenciaX = abs(posicion.getX() - arrayWaypoints[i]->getPosicion().X);
				diferenciaZ = abs(posicion.getZ() - arrayWaypoints[i]->getPosicion().Z);
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
		if (posicion.getZ() - (puntoActual->getPosicion().Z) > 0) {	//si la posicion en Z del waypoint es menor que posz cambiamos al siguiente waypoint
			puntoActual = puntoActual->getNextWaypoint();
		}

		if (posicion.getX() - (puntoActual->getPosicion().X) < 0) {
			girarDerecha();
		}else if (posicion.getX() - (puntoActual->getPosicion().X) > 0){
			girarIzquierda();
		}
	}else if (sur) {
		//cout << "CONDICION SUR" << endl;
		if (posicion.getZ() - (puntoActual->getPosicion().Z) < 0) {	//si la posicion en Z del waypoint es mayor que posz cambiamos al siguiente waypoint
			puntoActual = puntoActual->getNextWaypoint();
		}

		if (posicion.getX() - (puntoActual->getPosicion().X) > 0) {
			girarIzquierda();
		}else if (posicion.getX() - (puntoActual->getPosicion().X) < 0){
			girarDerecha();
		}

	}else if (este) {
		//cout << "CONDICION ESTE" << endl;
		if (posicion.getX() - (puntoActual->getPosicion().X) > 0) {	//si la posicion en X del waypoint es menor que pozx cambiamos al siguiente waypoint
			puntoActual = puntoActual->getNextWaypoint();
		}
		if (posicion.getZ() - (puntoActual->getPosicion().Z) < 0) {
			girarIzquierda();
		}else if (posicion.getZ() - (puntoActual->getPosicion().Z) > 0){
			girarDerecha();
		}

	}else if (oeste) {
		//cout << "CONDICION OESTE" << endl;
		if (posicion.getX() - (puntoActual->getPosicion().X) < 0) {	//si la posicion en X del waypoint es mayor que posx cambiamos al siguiente waypoint
			puntoActual = puntoActual->getNextWaypoint();
		}
		if (posicion.getZ() - (puntoActual->getPosicion().Z) > 0) {
			girarIzquierda();
		}else if (posicion.getZ() - (puntoActual->getPosicion().Z) < 0){
			girarDerecha();
		}
	}

	//Cara B (SUR) - ATRAS
	//Cara C (ESTE) - DRCH
	//Cara D (OESTE) - IZQ


}

std::string CorredorIA::getNombreWaypoint()
{
	return puntoActual->getNombre();
}

std::string CorredorIA::toString()
{

	std::string text = Corredor::toString();
	text += "\n -- CORREDOR IA --";

	text += "\n Waypoint siguiente: ";
	text += getNombreWaypoint().c_str();

	return text;
}