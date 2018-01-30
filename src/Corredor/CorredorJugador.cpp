#include "CorredorJugador.hpp"
#include "Client.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj, btVector3 pos) : Corredor(rutaObj, pos)
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
	btVector3 posDisparo(cuboNodo->getPosition().X + orientacion.getX() * 5, cuboNodo->getPosition().Y+5, cuboNodo->getPosition().Z + orientacion.getZ() * 5);
	btVector3 posDisparo2(cuboNodo->getPosition().X, cuboNodo->getPosition().Y-1, cuboNodo->getPosition().Z);
	
	if(teclado->isKeyDown(KEY_KEY_1)) setTipoObj(1);
	if(teclado->isKeyDown(KEY_KEY_2)) setTipoObj(2);
	if(teclado->isKeyDown(KEY_KEY_3)) setTipoObj(3);
	if(teclado->isKeyDown(KEY_KEY_4)) setTipoObj(4);
	if(teclado->isKeyDown(KEY_KEY_5)) setTipoObj(5);//escudo
	if(teclado->isKeyDown(KEY_KEY_6)) setTipoObj(6);//proytectil x3
	if(teclado->isKeyDown(KEY_KEY_7)) setTipoObj(7);//Habilidad
	if(teclado->isKeyDown(KEY_KEY_8)) setTipoObj(8);

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
			if(client->getConnected())
				client->PlayerThrowObject();

			if (tipoObj == 1)
			{
				pro = new Proyectil(posDisparo);
				lanzarItem(pro, direccionItem);
				items.push_back(pro);
				soltarItem();
			}
				else if (tipoObj == 2)
			{
				posDisparo.setX(cuboNodo->getPosition().X - orientacion.getX() * 5);
				posDisparo.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 5);
				CajaFalsa *est = new CajaFalsa(posDisparo);
				est->inicializarFisicas();
				soltarItem();
				items.push_back(est);
			}
			else if (tipoObj == 3)
			{
				setTurbo(true, true,26000);
			}else if (tipoObj == 4)
			{
				posDisparo.setX(cuboNodo->getPosition().X - orientacion.getX() * 5);
				posDisparo.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 5);
				Aceite *est2 = new Aceite(posDisparo);
				est2->inicializarFisicas();
				//est2->getRigidBody()->setCollisionFlags(est2->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
				soltarItem();
				items.push_back(est2);
			}
			else if (tipoObj == 5)
			{
				if(getProteccion()==false) setProteccion(true);
				soltarItem();
			}
			else if (tipoObj == 6)
			{
				proX3 = new Proyectil *[3];
				btVector3 orientacioncentral(orientacion.getX(), orientacion.getY(), orientacion.getZ());
				btVector3 centro(cuboNodo->getPosition().X + orientacion.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 5);
				btVector3 orientacionderecha = orientacioncentral.rotate(btVector3(0, 1, 0), 10 * PI / 180);
				btVector3 orientacionizquierda = orientacioncentral.rotate(btVector3(0, 1, 0), -10 * PI / 180);
				btVector3 iz(cuboNodo->getPosition().X + orientacionizquierda.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionizquierda.getZ() * 5);
				btVector3 d(cuboNodo->getPosition().X + orientacionderecha.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionderecha.getZ() * 5);
				for (int i = 0; i < 3; i++) {


					if (i == 0) {
						proX3[i] = new Proyectil(iz);
						proX3[i]->inicializarFisicas();
						proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacionizquierda.getX() * 100, 5.0f, orientacionizquierda.getZ() * 100));
					}
					if (i == 1) {

						proX3[i] = new Proyectil(centro);
						proX3[i]->inicializarFisicas();
						proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacioncentral.getX() * 100, 5.0f, orientacioncentral.getZ() * 100));
					}
					if (i == 2) {

						proX3[i] = new Proyectil(d);
						proX3[i]->inicializarFisicas();
						proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacionderecha.getX() * 100, 5.0f, orientacionderecha.getZ() * 100));
					}


					items.push_back(proX3[i]);



				}
				soltarItem();
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
					cout << "que mierda estas haciendo , no puedes usar la habilidad si tu limite no es 10 o mas" << endl;
				}
				
			}else if (tipoObj == 8){
				ItemTeledirigido *pt= new ItemTeledirigido(posDisparo2);
				pt->inicializarFisicas();
				pt->getRigidBody()->setCollisionFlags(pt->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
				pt->getNodo()->setParent(cuboNodo);
				tipoObj=0;
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