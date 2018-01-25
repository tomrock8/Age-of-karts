#include "CorredorJugador.hpp"
#include "Client.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj, vector3df pos) : Corredor(rutaObj, pos)
{
	checkItem = false;
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
	if (teclado->isKeyDown(KEY_SPACE))
	{
		frenodemano(true);
		comprobadorMovimiento = true;
	}else{
		frenodemano(false);
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

	if (!comprobadorMovimiento)
		if (!turboActivado)
		desacelerar();

}

void CorredorJugador::update()
{
	Corredor::update();
	movimiento();
}

void CorredorJugador::actualizarItem()
{

	CTeclado *teclado = CTeclado::getInstancia();
	Pista *pista = Pista::getInstancia();
	core::list<Item *> items = pista->getItems();
	int direccionItem = 1;
	vector3df posDisparo(cuboNodo->getPosition().X + orientacion.X * 5, cuboNodo->getPosition().Y+10, cuboNodo->getPosition().Z + orientacion.Z * 5);
	
	if(teclado->isKeyDown(KEY_KEY_1)) setTipoObj(1);
	if(teclado->isKeyDown(KEY_KEY_2)) setTipoObj(2);
	if(teclado->isKeyDown(KEY_KEY_3)) setTipoObj(3);
	if(teclado->isKeyDown(KEY_KEY_4)) setTipoObj(4);
	if(teclado->isKeyDown(KEY_KEY_5)) setTipoObj(5);//escudo
	if(teclado->isKeyDown(KEY_KEY_6)) setTipoObj(6);//proytectil x3
	if(teclado->isKeyDown(KEY_KEY_7)) setTipoObj(7);//Habilidad
	

	if (teclado->isKeyup(KEY_KEY_P))
	{

		//if P is pressed, shoot a box
		if (checkItem == true)
		{
			checkItem = false;
		}
	}
	else if (teclado->isKeyDown(KEY_KEY_P))
	{
		if (tipoObj != 0 && !checkItem)
		{
			checkItem = true;

			Client *client = Client::getInstancia();
			client->PlayerThrowObject();

			if (tipoObj == 1)
			{
				lanzarFlecha(posDisparo);
			}
			else if (tipoObj == 2)
			{
				lanzarCajaFalsa(posDisparo);
			}
			else if (tipoObj == 3)
			{
				lanzarTurbo();
			}else if (tipoObj == 4)
			{
				lanzarAceite(posDisparo);
			}
			else if (tipoObj == 5)
			{
				lanzarEscudo();
			}
			else if (tipoObj == 6)
			{
				lanzarFlechaTriple(posDisparo);
			}
			else if (tipoObj == 7)
			{
				if (getLimite() >= 10) {//puedo lanzar la habilidad
					h->getNodo()->setVisible(true);
					h->setOrientacion(orientacion);
					h->setPadre(this->getNodo());
					h->setPosicion(posDisparo);
					h->lanzarHabilidad();
					items.push_back(h);
					soltarItem();
				}
				else {
					cout << "No puedes usar la habilidad si tu limite no es 10 o mas" << endl;
				}
				
			}
			pista->setItems(items);
			//Llama a la funcion de la clase padre

		}
	}
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
bool CorredorJugador::setComprobadorMovimiento(bool s) {
	comprobadorMovimiento = s;
	return comprobadorMovimiento;
}