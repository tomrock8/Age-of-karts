#include "GestorColisiones.hpp"

#define TAMANYOCAJAS 10
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

void GestorColisiones::ComprobarColisiones(CorredorJugador **pj)
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	Pista *pista = Pista::getInstancia();
	Caja** cajas = pista->getArrayCaja();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	pj1 = pj;
	int numManifolds = mundo->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold *contactManifold = mundo->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject *obA = contactManifold->getBody0();
		const btCollisionObject *obB = contactManifold->getBody1();

		nodoA = static_cast<ISceneNode *>(obA->getUserPointer());
		nodoB = static_cast<ISceneNode *>(obB->getUserPointer());

		if (nodoA != 0 && nodoB != 0)
		{

			/* if (strcmp("Proyectil", nodoB->getName()) == 0){
			   cout<<"------------------------ID PROYECTIL: "<<nodoB->getID()<<endl;
			   if (item->getNodo() !=NULL){
				   cout<<"------------------------ID PROYECTIL ITEM: "<<item->getNodo()->getID()<<endl;
			   }else{
				   cout<<"------------------------ID PROYECTIL ITEM CORRUPTO: ";
			   }
			}*/
			
			if (JugadorCaja(cajas))continue;
			if (JugadorTurbo())continue;
			if(JugadorWaypoint())continue;
			if (objetoDestruible())continue;
			if (JugadorEstatico())continue;

		}
	}
}



bool GestorColisiones::JugadorWaypoint(){
	
     if (strcmp("Jugador", nodoA->getName()) == 0)
    {
        if (strcmp("Waypoint", nodoB->getName()) == 0)
        {
			for(int i = 0; i< 2; i++){
			if(nodoA->getID() == pj1[i]->getNodo()->getID()){
				pj1[i]->setWaypointActual(nodoB);
				
				return true;	
				}
			}
			
		}
    }
	
	return false;
	
}



//
// Comprobar colisiones entre Jugador y turbo
//
bool GestorColisiones::JugadorTurbo()
{
	Pista *mapa = Pista::getInstancia();
	//cout << TimeStamp << endl;

	if (strcmp("Jugador", nodoA->getName()) == 0)
	{
		if (strcmp("Turbo", nodoB->getName()) == 0)
		{
			for( int i = 0; i< 2; i++)
				if(nodoA->getID() == pj1[i]->getNodo()->getID())
					pj1[i]->setTurbo(true, false);

			//cout << "Jugador - Turbo\n";
			return true;
		}
	}
	return false;
}

//
// Comprobar colisiones entre Jugador y turbo
//
bool GestorColisiones::JugadorEstatico()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	Pista *pista = Pista::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<Item *> items = pista->getItems();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	Pista *mapa = Pista::getInstancia();
	//cout << TimeStamp << endl;

	if (strcmp("Jugador", nodoA->getName()) == 0)
	{
		if (strcmp("Estatico", nodoB->getName()) == 0)
		{
			//Turbo *t = mapa->getTurbo();
			//t->setFrenadaActivo(pj1Col, true);
			int idB = nodoB->getID();
			for (core::list<Item *>::Iterator Iterator = items.begin(); Iterator != items.end(); ++Iterator)
			{
				Item *item = *Iterator;
				if (item->getNodo()->getID() == idB)
				{
					if (item->getColision()) {
						item->Delete();
						Iterator = items.erase(Iterator);
						pista->setItems(items);
					}
					else {
						item->setColision(true);
					}

					return true;
				}
			}

			//cout << "Jugador - Turbo\n";
		}
	}
}

//
// Comprobar colisiones entre Jugador y Caja
//
bool GestorColisiones::JugadorCaja(Caja **cajas)
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	Pista *pista = Pista::getInstancia();
	int tamCajas = pista->getTamCajas();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	bool colision = false;

	if (strcmp("Jugador", nodoA->getName()) == 0)
	{
		if (strcmp("Caja", nodoB->getName()) == 0 && nodoB->isVisible())
		{
			colision = true;
			//cout << "Jug - Caja\n";
			int idB = nodoB->getID();
			//cout << "Tam Cajas: " << tamCajas << "- " << idB << endl;
			for (int i = 0; i < tamCajas; i++)
			{
				if (cajas[i] != NULL)
				{
					if (cajas[i]->getIDCaja() == idB)
					{
						for(int j = 0; j<2; j++)
							if(nodoA->getID()== pj1[j]->getNodo()->getID()){
								cajas[i]->romper(pj1[j]);
							}
					}
				}
				//cout << "Entro " << i << endl;
			}
			pista->setArrayCaja(cajas);
		}
	}

	return colision;
}

//
// Comprobar colisiones entre proyectil y objeto destruible
//
bool GestorColisiones::objetoDestruible()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	Pista *pista = Pista::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<Item *> items = pista->getItems();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	bool colision = false;

	if (strcmp("Destruible", nodoA->getName()) == 0)
	{
		if (strcmp("Proyectil", nodoB->getName()) == 0)
		{
			colision = true;
			//cout << "Destruible - Item\n";
			int idB = nodoB->getID();
			for (core::list<Item *>::Iterator Iterator = items.begin(); Iterator != items.end(); ++Iterator)
			{
				Item *item = *Iterator;
				//cout << "NodoB: " << idB << " == NodoItem: " << item->getNodo()->getID() << endl;
				if (item->getNodo()->getID() == idB)
				{
					//cout << "Entro\n";
					item->Delete();
					Iterator = items.erase(Iterator);
					pista->setItems(items);

					return true;
				}
			}
		}
	}
	return colision;
}
