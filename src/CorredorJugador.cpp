#include "CorredorJugador.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj, vector3df pos) : Corredor(rutaObj, pos)
{
}

/**
	Movimiento del jugador controlado por el teclado
	TODO: Modificar para detectar el input que se recibe.
*/
void CorredorJugador::movimiento()
{
	CTeclado *teclado = CTeclado::getInstancia();
	Motor3d *m = Motor3d::getInstancia();
	bool mierda = false;

	//-------ENTRADA TECLADO ----------//
	if (teclado->isKeyDown(KEY_ESCAPE))
	{
		m->getDevice()->closeDevice();
	}
	if (teclado->isKeyDown(KEY_KEY_S))
	{
		frenar();
		//vehiculo-> set
		mierda = true;
	}
	if (teclado->isKeyDown(KEY_KEY_W))
	{
		acelerar();
		mierda = true;
	}
	if (teclado->isKeyDown(KEY_KEY_D))
	{
		girarDerecha();

		mierda = true;
	}
	if (teclado->isKeyDown(KEY_KEY_A))
	{

		girarIzquierda();

		mierda = true;
	}

	if (teclado->isKeyDown(KEY_SPACE))
	{

		frenodemano();

		mierda = true;
	}

	if (!mierda)
	{

		desacelerar();
	}
}

std::string CorredorJugador::toString()
{

	std::string text = Corredor::toString();
	text += "\n -- CORREDOR JUGADOR -- ";

	return text;
}
