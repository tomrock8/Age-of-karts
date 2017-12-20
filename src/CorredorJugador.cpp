#include "CorredorJugador.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj,vector3df pos) :
	Corredor(rutaObj,pos) {
			checkItem = false;
}

/**
	Movimiento del jugador controlado por el teclado
	TODO: Modificar para detectar el input que se recibe.
*/
void CorredorJugador::movimiento() {
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
			}
	

void CorredorJugador::update() {
	
	movimiento();
}

Proyectil *CorredorJugador::actualizarItem(Proyectil *item,int &id){

	CTeclado *teclado = CTeclado::getInstancia();
	Pista *pista = Pista::getInstancia();
	core::list<Item *> items=pista->getItems();

	if (teclado->isKeyup(KEY_KEY_P))
	{
		
		vector3df posDisparo(cuboNodo->getPosition().X, cuboNodo->getPosition().Y , cuboNodo->getPosition().Z+5);
		//if P is pressed, shoot a box
		if (checkItem == true)
		{
			item = new Proyectil(posDisparo,id);
			items.push_back(item);
			pista->setItems(items);
			//Llama a la funcion de la clase padre
			lanzarItem(item);
			id++;
			checkItem = false;

		}
	}else if (teclado->isKeyDown(KEY_KEY_P)){
		if (cargador > 0)
		{
		checkItem = true;
		}
	}
	return item;

}

/*
	Comprueba si el jugador pulsa P para crear un item y lanzarlo
*/
std::string CorredorJugador::toString()
{
	std::string text = Corredor::toString();
 	text += "\n -- CORREDOR JUGADOR -- ";	
	return text;
}