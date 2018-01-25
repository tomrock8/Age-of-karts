#include "CorredorRed.hpp"


CorredorRed::CorredorRed(stringw rutaObj, vector3df pos) : Corredor(rutaObj, pos)
{
	cuboNodo->setName("JugadorRed");
}

void CorredorRed::updateRed(){
    movimiento();
	actualizarRuedas();
	EstadosJugador *estados = this->getEstados();
	estados->update();

}

void CorredorRed::movimiento(){
    //std::cout << "Entro\n";
    EstadosJugador *estados = this->getEstados();
	int estadoMovimiento = estados->getEstadoMovimiento();
	int direccionMovimiento = estados->getDireccionMovimiento();
	//cout <<"Estado del jugador: "<<estadoMovimiento<<endl;
	//cout <<"Direccion del jugador: "<<direccionMovimiento<<endl;
    frenodemano(false);
    switch (estadoMovimiento){

        case 0:
        //std::cout << "Esta quieto\n";
		break;
		case 1:
        acelerar();
		break;
		case 2:
        frenar();
		break;
		case 3:
        frenar();
		break;
		case 4:
		frenodemano(true);
		break;
		case 5:
		desacelerar();
		break;
    }

	
    switch (direccionMovimiento){

		case 0:
		break;
		case 1:
        girarIzquierda();
		break;
		case 2:
        girarDerecha();   
		break;
    }
	
    
}

void CorredorRed::lanzarItemRed(){

	Pista *pista = Pista::getInstancia();
	core::list<Item *> items = pista->getItems();
	int direccionItem = 1;
	vector3df posDisparo(cuboNodo->getPosition().X + orientacion.X * 5, cuboNodo->getPosition().Y+10, cuboNodo->getPosition().Z + orientacion.Z * 5);

	if (tipoObj == 1)
			{
				pro = new Proyectil(posDisparo);
				lanzarItem(pro, direccionItem);
				items.push_back(pro);
			}
				else if (tipoObj == 2)
			{
				posDisparo.X = cuboNodo->getPosition().X - orientacion.X * 5;
				posDisparo.Z = cuboNodo->getPosition().Z - orientacion.Z * 5;
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
				posDisparo.X = cuboNodo->getPosition().X - orientacion.X * 5;
				posDisparo.Z = cuboNodo->getPosition().Z - orientacion.Z * 5;
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
				btVector3 orientacioncentral(orientacion.X, orientacion.Y, orientacion.Z);
				btVector3 centro(cuboNodo->getPosition().X + orientacion.X * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.Z * 5);
				btVector3 orientacionderecha = orientacioncentral.rotate(btVector3(0, 1, 0), 10 * PI / 180);
				btVector3 orientacionizquierda = orientacioncentral.rotate(btVector3(0, 1, 0), -10 * PI / 180);
				vector3df c(centro.getX(), centro.getY(), centro.getZ());
				vector3df iz(cuboNodo->getPosition().X + orientacionizquierda.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionizquierda.getZ() * 5);
				vector3df d(cuboNodo->getPosition().X + orientacionderecha.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionderecha.getZ() * 5);
				for (int i = 0; i < 3; i++) {


					if (i == 0) {
						proX3[i] = new Proyectil(iz);
						proX3[i]->inicializarFisicas();
						proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacionizquierda.getX() * 100, 5.0f, orientacionizquierda.getZ() * 100));
					}
					if (i == 1) {

						proX3[i] = new Proyectil(c);
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
				h->getNodo()->setVisible(true);
				h->setOrientacion(orientacion);
				h->setPadre(this->getNodo());
				h->setPosicion(posDisparo);
				h->lanzarHabilidad();
				items.push_back(h);
				soltarItem();
			}
			pista->setItems(items);
}

void CorredorRed::acelerarRed(){
	acelerar();
}

void CorredorRed::frenarRed(){
    frenar();
}
void CorredorRed::girarDRed(){
    girarDerecha();
}

void CorredorRed::girarIRed(){
    girarIzquierda();
}