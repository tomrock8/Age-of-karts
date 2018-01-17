#include "CorredorJugador.hpp"

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

	if	(teclado->isKeyDown(KEY_KEY_V)){
		movimientoIA();

	}
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
	vector3df posDisparo(cuboNodo->getPosition().X + orientacion.X * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.Z * 5);
	
	if(teclado->isKeyDown(KEY_KEY_1)) setTipoObj(1);
	if(teclado->isKeyDown(KEY_KEY_2)) setTipoObj(2);
	if(teclado->isKeyDown(KEY_KEY_3)) setTipoObj(3);
	
	Pista *mapa = Pista::getInstancia();
	int idObjMapa = pista->getIdsObjetosCaja();

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
			if (teclado->isKeyDown(KEY_DOWN))
			{
				posDisparo.X = cuboNodo->getPosition().X - orientacion.X * 5;
				posDisparo.Z = cuboNodo->getPosition().Z - orientacion.Z * 5;
				direccionItem = -1;
			}

			if (tipoObj == 1)
			{
				pro = new Proyectil(posDisparo, idObjMapa);
				lanzarItem(pro, direccionItem);
				items.push_back(pro);
			}
			else if (tipoObj == 2)
			{
				posDisparo.X = cuboNodo->getPosition().X - orientacion.X * 5;
				posDisparo.Z = cuboNodo->getPosition().Z - orientacion.Z * 5;
				est = new Estatico(posDisparo, idObjMapa);
				est->inicializarFisicas();
				soltarItem(est);
				items.push_back(est);
			}
			else if (tipoObj == 3)
			{
				setTurbo(true, true);
			}
			pista->setItems(items);
			//Llama a la funcion de la clase padre
			idObjMapa++;
			pista->setIdsObjetosCaja(idObjMapa);
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