#include "CorredorIA.hpp"

CorredorIA::CorredorIA(stringw rutaObj, vector3df pos, Waypoint **arrayWaypoints, int tamanyoArray) : Corredor(rutaObj, pos)
{

	//sacamos el waypoint m�s cercano
	float diferenciaX = 0;
	float diferenciaZ = 0;
	for (int i = 0; i < tamanyoArray; i++)
	{

		//puntoActual=arrayWaypoints[i];
		if (i == 0)
		{
			puntoActual = arrayWaypoints[i];
			diferenciaX = abs(cuboNodo->getPosition().X - arrayWaypoints[i]->getPosicion().X);
			diferenciaZ = abs(cuboNodo->getPosition().Z - arrayWaypoints[i]->getPosicion().Z);
		}
		else
		{

			if ((abs(cuboNodo->getPosition().X - arrayWaypoints[i]->getPosicion().X)) <= diferenciaX && (abs(cuboNodo->getPosition().Z - arrayWaypoints[i]->getPosicion().Z)) <= diferenciaZ)
			{
				puntoActual = arrayWaypoints[i];
				diferenciaX = abs(cuboNodo->getPosition().X - arrayWaypoints[i]->getPosicion().X);
				diferenciaZ = abs(cuboNodo->getPosition().Z - arrayWaypoints[i]->getPosicion().Z);
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

	//if (norte) {
	//cout << "CONDICION NORTE" << endl;
	if (cuboNodo->getPosition().Z - (puntoActual->getPosicion().Z) > 0)
	{
		puntoActual = puntoActual->getNextWaypoint();
	}

	if (cuboNodo->getPosition().X - (puntoActual->getPosicion().X) < 0)
	{
		girarDerecha();
	}
	else
	{
		girarIzquierda();
	}
	/*
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

*/
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

	int aux = 0;

	aux = vehiculo->getForwardAxis();
	text += "\n INDEX: getForwardAxis: " + to_string(aux);

	aux = vehiculo->getUpAxis();
	text += "\n INDEX: getUpAxis: " + to_string(aux);

	aux = vehiculo->getRightAxis();
	text += "\n INDEX: getRightAxis: " + to_string(aux);

	text += "\nVector: X[" + to_string(vehiculo->getForwardVector().getX()) +
			"] , Y[" + to_string(vehiculo->getForwardVector().getY()) +
			"] , Z[" + to_string(vehiculo->getForwardVector().getZ()) + "]";

	return text;
}
