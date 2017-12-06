#include "CorredorJugador.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj, s32 id_colision) :
	Corredor(rutaObj, id_colision) {

}

/**
	Movimiento del jugador controlado por el teclado
	TODO: Modificar para detectar el input que se recibe.
*/
void CorredorJugador::movimiento() {
	CTeclado *teclado = CTeclado::getInstancia();

	int checkGiro = 0;
	int checkMarchaAtras = 0;
	float checkVelocidad = getVelocidad();

	if (teclado->isKeyDown(KEY_KEY_S))
	{
		frenar();
		checkMarchaAtras = 1;
	}
	else if (teclado->isKeyDown(KEY_KEY_W))
	{
		acelerar();
	}
	else
	{
		desacelerar();
	}
	if (teclado->isKeyDown(KEY_KEY_D))
	{
		if (checkMarchaAtras == 0)
		{
			girarDerecha();
		}
		else
		{
			if (checkVelocidad < 0.5)
			{
				girarIzquierda();
			}
		}
		checkGiro = 1;
	}
	else if (teclado->isKeyDown(KEY_KEY_A))
	{
		if (checkMarchaAtras == 0)
		{
			girarIzquierda();
		}
		else
		{
			if (checkVelocidad < 0.5)
			{
				girarDerecha();
			}
		}
		checkGiro = 1;
	}

	if (checkGiro == 0)
	{
		resetGiro();
	}
}

void CorredorJugador::update() {
	Corredor::update();
	movimiento();
}