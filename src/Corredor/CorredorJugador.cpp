#include "CorredorJugador.hpp"
#include "Client.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo) : Corredor(rutaObj, pos,tipo) {
	checkItem = false;
}

/**
	Movimiento del jugador controlado por el teclado
	TODO: Modificar para detectar el input que se recibe.
*/
void CorredorJugador::movimiento() {
	CTeclado *teclado = CTeclado::getInstancia();

	bool comprobadorMovimiento = false;

	//-------ENTRADA TECLADO ----------//
	if (teclado->isKeyDown(KEY_KEY_S)) {
		frenar();
		//vehiculo-> set
		comprobadorMovimiento = true;
	}
	else if (teclado->isKeyDown(KEY_KEY_W)) {
		acelerar();
		comprobadorMovimiento = true;
	}
	if (teclado->isKeyDown(KEY_SPACE)) {
		frenodemano(true, false);
		comprobadorMovimiento = true;
	}
	else {
		frenodemano(false, false);
	}
	if (teclado->isKeyDown(KEY_KEY_D)) {
		girarDerecha();

		comprobadorMovimiento = true;
	}
	else if (teclado->isKeyDown(KEY_KEY_A)) {
		girarIzquierda();
		comprobadorMovimiento = true;
	}

	if (!comprobadorMovimiento) {
		if (!turboActivado) {
			desacelerar();
		}
	}

}

void CorredorJugador::update() {
	Corredor::update();
	movimiento();
}

void CorredorJugador::actualizarItem() {

	CTeclado *teclado = CTeclado::getInstancia();

	int direccionItem = 1;
	setPosDisparo(btVector3(cuboNodo->getPosition().X + orientacion.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 10));
	btVector3 posDisparo2(cuboNodo->getPosition().X, cuboNodo->getPosition().Y - 1, cuboNodo->getPosition().Z);

	if (teclado->isKeyDown(KEY_KEY_1)) setTipoObj(1);
	if (teclado->isKeyDown(KEY_KEY_2)) setTipoObj(2);
	if (teclado->isKeyDown(KEY_KEY_3)) setTipoObj(3);
	if (teclado->isKeyDown(KEY_KEY_4)) setTipoObj(4);
	if (teclado->isKeyDown(KEY_KEY_5)) setTipoObj(5);//escudo
	if (teclado->isKeyDown(KEY_KEY_6)) setTipoObj(6);//proytectil x3
	if (teclado->isKeyDown(KEY_KEY_7)) setTipoObj(7);//FlechaTeledirigida
	if (teclado->isKeyDown(KEY_KEY_8)) setTipoObj(8);//TurboTriple

	if (teclado->isKeyup(KEY_KEY_P))
	{
		// comprobacion del item
		if (getCheckItem() == true) {
			setCheckItem(false);
		}
	}
	else if (teclado->isKeyDown(KEY_KEY_P))
	{
		if (getTipoObj() != 0 && !checkItem)
		{
			setCheckItem(true);
			//Para saber si se ha lanzado el item desde un jugador en red
			Client *client = Client::getInstancia();
			if (client->getConnected())
				client->PlayerThrowObject();
			//Llama a la funcion de la clase padre
			usarObjetos();
		}
	}
	if(teclado->isKeyDown(KEY_KEY_O)){
		lanzarHabilidad();
	}
}

/*
	Comprueba si el jugador pulsa P para crear un item y lanzarlo
*/
std::string CorredorJugador::toString() {
	std::string text = Corredor::toString();
	text += "\n -- CORREDOR JUGADOR -- ";
	return text;
}
bool CorredorJugador::setComprobadorMovimiento(bool s) {
	comprobadorMovimiento = s;
	return comprobadorMovimiento;
}