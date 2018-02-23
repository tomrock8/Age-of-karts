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

	bool comprobadorMovimiento = false;

	//-------ENTRADA TECLADO ----------//
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		frenar();
		//vehiculo-> set
		comprobadorMovimiento = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		acelerar();
		comprobadorMovimiento = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		frenodemano(true, false);
		comprobadorMovimiento = true;
	}
	else {
		frenodemano(false, false);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		girarDerecha();

		comprobadorMovimiento = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		girarIzquierda();
		comprobadorMovimiento = true;
	}

	if (!comprobadorMovimiento) {
		if (!turboActivado) {
			desacelerar();
		}
	}

}



void CorredorJugador::updateHijos(){

}



void CorredorJugador::actualizarItem() {

	int direccionItem = 1;
	setPosDisparo(btVector3(cuboNodo->getPosition().X + orientacion.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 10));
	btVector3 posDisparo2(cuboNodo->getPosition().X, cuboNodo->getPosition().Y - 1, cuboNodo->getPosition().Z);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) setTipoObj(1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) setTipoObj(2);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) setTipoObj(3);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) setTipoObj(4);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) setTipoObj(5);//escudo
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) setTipoObj(6);//proytectil x3
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) setTipoObj(7);//FlechaTeledirigida
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) setTipoObj(8);//TurboTriple

	/*if (sf::Keyboard::isKeyReleased(sf::Keyboard::P))
	{
		// comprobacion del item
		if (getCheckItem() == true) {
			setCheckItem(false);
		}
	}
	else*/ if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
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
	}else{
		if (getCheckItem() == true) {
			setCheckItem(false);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
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