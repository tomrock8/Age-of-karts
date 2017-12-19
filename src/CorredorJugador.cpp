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

	bool comprobadorMovimiento = false;

	//-------ENTRADA TECLADO ----------//
	if (teclado->isKeyDown(KEY_ESCAPE))
	{
		m->getDevice()->closeDevice();
	}
	if (teclado->isKeyDown(KEY_KEY_S))
	{
		frenar();
		//vehiculo-> set
		comprobadorMovimiento = true;
	}
	else if (teclado->isKeyDown(KEY_KEY_W))
	{
		acelerar();
		comprobadorMovimiento = true;
	}
	if (teclado->isKeyDown(KEY_KEY_D))
	{
		girarDerecha();

		comprobadorMovimiento = true;
	}
	else if (teclado->isKeyDown(KEY_KEY_A))
	{

		girarIzquierda();

		comprobadorMovimiento = true;
	}

	if (teclado->isKeyDown(KEY_SPACE))
	{

		frenodemano();

		comprobadorMovimiento = true;
	}

	if (!comprobadorMovimiento)
	{

		desacelerar();
	}
}

/*
	Comprueba si el jugador pulsa P para crear un item y lanzarlo
*/
Proyectil *CorredorJugador::actualizarItem(int &id, btDiscreteDynamicsWorld *mundo, core::list<btRigidBody *> objetos)
{
	CTeclado *teclado = CTeclado::getInstancia();
	Motor3d *m = Motor3d::getInstancia();
	if (teclado->isKeyup(KEY_KEY_P))
	{
		vector3df cam(cuboNodo->getPosition().X, cuboNodo->getPosition().Y + 10, cuboNodo->getPosition().Z);
		//if P is pressed, shoot a box
		if (checkItem == true)
		{
			Proyectil *item = new Proyectil(cam, id);
			id++;
			//Llama a la funcion de la clase padre
			lanzarItem(item, mundo, objetos);

			checkItem = false;
		}
	}
	else if (teclado->isKeyDown(KEY_KEY_P))
	{
		if (cargador > 0)
		{
			checkItem = true;
		}
	}
}

std::string CorredorJugador::toString()
{
	std::string text = Corredor::toString();
 	text += "\n -- CORREDOR JUGADOR -- ";	
	return text;
}