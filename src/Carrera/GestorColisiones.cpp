#include "GestorColisiones.hpp"

#define TAMANYOCAJAS 10
//#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
void GestorColisiones::IniciarTimer(){
	tiempoInicio = clock();
}
void GestorColisiones::ComprobarColisiones()
{	
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	jugadores = GestorJugadores::getInstancia();
	Pista *pista = Pista::getInstancia();
	vector<Caja*> cajas = pista->getArrayCaja();
	vector<Item*> items = pista->getItems();
	btDynamicsWorld *mundo = bullet->getMundo();
	pj1 = jugadores->getJugadores();
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
			
			
			if (ItemTeledirigidoWaypoint())continue;
			
			if (JugadorCaja(cajas))continue;
			
			if (JugadorTurbo())continue;
			
			if (JugadorWaypoint())continue;
			
			if (objetoDestruible())continue;
			
			if (HabilidadesJugadores())continue;

			if(Escudoitems())break;

			if(HabilidadesItems())break;

			if (JugadorProyectil())break;
			
			if (JugadorEstatico())break;
			
			//if (JugadorItemTeledirigido())continue;

		}
	}
	jugadores->setJugadores(pj1);
}



bool GestorColisiones::JugadorWaypoint(){
	
     if (strcmp("Jugador", nodoA->getName()) == 0 || strcmp("JugadorIA", nodoA->getName()) == 0 || strcmp("JugadorRed", nodoA->getName()) == 0)
    {
        if (strcmp("Waypoint", nodoB->getName()) == 0)
        {
			for(int i = 0; i< jugadores->getNumJugadores(); i++){
				//if(pj1.at(i)!=NULL)
					if(nodoA->getID() == pj1.at(i)->getNodo()->getID()){
						if (nodoB->getID()-6==0 && pj1.at(i)->getWaypointSiguiente()->getWaypoint()->getID()-6==0){
							clock_t timediff = clock() - tiempoInicio;
							float timediff_sec = ((float)timediff) / CLOCKS_PER_SEC;
							
							///cout<<"timediff_sec: "<<timediff_sec<<endl;
							if (pj1.at(i)->getMaxVuetas()>=pj1.at(i)->getVueltas()){
								pj1.at(i)->setTiempoVuelta(timediff_sec-pj1.at(i)->getTiempoVueltaTotal());
							}
							
							
						}
						pj1.at(i)->setWaypointActual(nodoB);
						
						return true;	
					}
			}

		}
	}

	return false;

}


bool GestorColisiones::ItemTeledirigidoWaypoint(){

	Pista *pista = Pista::getInstancia();
	vector<Item *> items = pista->getItems();

	if (strcmp("Estatico", nodoA->getName()) == 0){
			 if (strcmp("Waypoint", nodoB->getName()) == 0)
        {
			
	int idA = nodoA->getID();		
	for (int i=0;i<items.size();i++){
		if (items.at(i)->getID() == idA){

						if(strcmp("ItemTeledirigido", items.at(i)->getNombre()) == 0){
						items.at(i)->setColision(nodoB->getID());
						
						return true;	
						}			
			
	}
		
	}
}
}

if (strcmp("Estatico", nodoB->getName()) == 0){
			 if (strcmp("Waypoint", nodoA->getName()) == 0)
        {
			
	int idB = nodoB->getID();		
	for (int i=0;i<items.size();i++){
		if (items.at(i)->getID() == idB){

						if(strcmp("ItemTeledirigido", items.at(i)->getNombre()) == 0){
						items.at(i)->setColision(nodoA->getID());
						
						return true;	
						}
					
					
			
	}
		
	}
}
}

	return false;
}

bool GestorColisiones::HabilidadesJugadores(){
	Pista *pista = Pista::getInstancia();
	vector<Item *> items = pista->getItems();
	int idPadreUlti=0;
	
	if(strcmp("Jugador", nodoA->getName()) == 0 || strcmp("JugadorIA", nodoA->getName()) == 0 || strcmp("JugadorRed", nodoA->getName()) == 0){
	if (strcmp("HabilidadVikingo", nodoB->getName()) == 0 
	|| strcmp("HabilidadPirata", nodoB->getName()) == 0 
	|| strcmp("HabilidadGladiador", nodoB->getName()) == 0 
	|| strcmp("HabilidadChino", nodoB->getName()) == 0){

		int idA = nodoA->getID();
		int idB = nodoB->getID();

			for (int i=0;i<items.size();i++){
				if(items.at(i)->getID() == idB){
						idPadreUlti = items.at(i)->getIDPadre();
				}
			}

			if(idPadreUlti==idA)
			return false;
			else{
				pj1.at(idA)->resetFuerzas();
				return true;
			}

			}

	}


	if(strcmp("Jugador", nodoB->getName()) == 0 || strcmp("JugadorIA", nodoB->getName()) == 0 || strcmp("JugadorRed", nodoB->getName()) == 0){
	if (strcmp("HabilidadVikingo", nodoA->getName()) == 0 
	|| strcmp("HabilidadPirata", nodoA->getName()) == 0 
	|| strcmp("HabilidadGladiador", nodoA->getName()) == 0 
	|| strcmp("HabilidadChino", nodoA->getName()) == 0){

		int idA = nodoA->getID();
		int idB = nodoB->getID();

			for (int i=0;i<items.size();i++){
				if(items.at(i)->getID() == idA){
						idPadreUlti = items.at(i)->getIDPadre();
				}
			}

			if(idPadreUlti==idB)
			return false;
			else{
				pj1.at(idB)->resetFuerzas();
				return true;
			}

			}

	}
	



	return false;
}

bool GestorColisiones::Escudoitems(){


	Pista *pista = Pista::getInstancia();
	vector<Item *> items = pista->getItems();
	
	int idEscudo=0;
	int idObjeto=0;
	int idPadreEscudo=0;
	int idPadreObjeto=0;

	if (strcmp("Escudo", nodoA->getName()) == 0){
		if (strcmp("Estatico", nodoB->getName()) == 0 
		|| strcmp("Proyectil", nodoB->getName()) == 0
		||strcmp("HabilidadVikingo", nodoB->getName()) == 0 
		|| strcmp("HabilidadPirata", nodoB->getName()) == 0 
		|| strcmp("HabilidadGladiador", nodoB->getName()) == 0 
		|| strcmp("HabilidadChino", nodoB->getName()) == 0){
		
			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i=0;i<items.size();i++){

						if(items.at(i)->getID() == idA){
						idPadreEscudo = items.at(i)->getIDPadre();
						idEscudo=i;	
							}
							
						if (items.at(i)->getID() == idB){
						idPadreObjeto= items.at(i)->getIDPadre();
						idObjeto=i;	
						
							}		
	}

			if(idPadreEscudo == idPadreObjeto){
			return false;
		
			}
			else{
				
				items.at(idEscudo)->Delete();
				items.erase(items.begin()+idEscudo);
				

				for (int i=0;i<items.size();i++){
					if(items.at(i)->getID() == idB){
					items.at(i)->Delete();
					items.erase(items.begin()+i);
				
				}
				}
				pista->setItems(items);
				return true;	
			}	
}
	}


if (strcmp("Escudo", nodoB->getName()) == 0){
		if (strcmp("Estatico", nodoA->getName()) == 0 
		|| strcmp("Proyectil", nodoA->getName()) == 0
		|| strcmp("HabilidadVikingo", nodoA->getName()) == 0 
		|| strcmp("HabilidadPirata", nodoA->getName()) == 0 
		|| strcmp("HabilidadGladiador", nodoA->getName()) == 0 
		|| strcmp("HabilidadChino", nodoA->getName()) == 0){
		
			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i=0;i<items.size();i++){

						if(items.at(i)->getID() == idB){
						idPadreEscudo = items.at(i)->getIDPadre();
						idEscudo=i;	
							}
							
						if (items.at(i)->getID() == idA){
						idPadreObjeto= items.at(i)->getIDPadre();
						idObjeto=i;	
						
							}		
	}

			if(idPadreEscudo == idPadreObjeto){
			return false;
		
			}
			else{
				
				items.at(idEscudo)->Delete();
				items.erase(items.begin()+idEscudo);
				
				for (int i=0;i<items.size();i++){
					if(items.at(i)->getID() == idA){
					items.at(i)->Delete();
					items.erase(items.begin()+i);
					
				}
				}
				pista->setItems(items);
				return true;	
			}	
}
	}

	return false;
}


bool GestorColisiones::HabilidadesItems(){

	Pista *pista = Pista::getInstancia();
	vector<Item *> items = pista->getItems();
	int idUlti=0;
	int idObjeto=0;
	int idPadreUlti=0;
	int idPadreObjeto=0;

	if (strcmp("HabilidadVikingo", nodoA->getName()) == 0 
	|| strcmp("HabilidadPirata", nodoA->getName()) == 0 
	|| strcmp("HabilidadGladiador", nodoA->getName()) == 0 
	|| strcmp("HabilidadChino", nodoA->getName()) == 0){
		
		if (strcmp("Estatico", nodoB->getName()) == 0 
		|| strcmp("Proyectil", nodoB->getName()) == 0){

			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i=0;i<items.size();i++){

						if(items.at(i)->getID() == idA){
						idPadreUlti = items.at(i)->getIDPadre();
						idUlti=i;
						
							}
							
						if (items.at(i)->getID() == idB){
						idPadreObjeto= items.at(i)->getIDPadre();
						idObjeto=i;	
							
							}		
	}

			if(idPadreUlti == idPadreObjeto){
			return false;
			}
			else{
				
				items.at(idObjeto)->Delete();
				items.erase(items.begin()+idObjeto);
				pista->setItems(items);
				
				return true;	
			}	
}
	}


if (strcmp("HabilidadVikingo", nodoB->getName()) == 0 
	|| strcmp("HabilidadPirata", nodoB->getName()) == 0 
	|| strcmp("HabilidadGladiador", nodoB->getName()) == 0 
	|| strcmp("HabilidadChino", nodoB->getName()) == 0){
		
		if (strcmp("Estatico", nodoA->getName()) == 0 
		|| strcmp("Proyectil", nodoA->getName()) == 0){

			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i=0;i<items.size();i++){

						if(items.at(i)->getID() == idB){
						idPadreUlti = items.at(i)->getIDPadre();
						idUlti=i;
						
							}
							
						if (items.at(i)->getID() == idA){
						idPadreObjeto= items.at(i)->getIDPadre();
						idObjeto=i;	
							
							}		
	}

			if(idPadreUlti == idPadreObjeto){
			return false;
			}
			else{
				
				items.at(idObjeto)->Delete();
				items.erase(items.begin()+idObjeto);
				pista->setItems(items);
				
				return true;	
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

	if (strcmp("Jugador", nodoA->getName()) == 0 || strcmp("JugadorIA", nodoA->getName()) == 0 || strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Turbo", nodoB->getName()) == 0)
		{
			for( int i = 0; i< jugadores->getNumJugadores(); i++)
				if(nodoA->getID() == pj1.at(i)->getNodo()->getID())
					pj1.at(i)->setTurbo(true, false,26000);
					
			//cout << "Jugador - Turbo\n";
			return true;
		}
	}
	return false;
}

//
// Comprobar colisiones entre Jugador y Estatico
//
bool GestorColisiones::JugadorEstatico()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	Pista *pista = Pista::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	vector<Item *> items = pista->getItems();
	vector<btRigidBody *> objetos = bullet->getObjetos();
	Pista *mapa = Pista::getInstancia();
	bool protegido=false;
	bool aceite = false;

	if (strcmp("Jugador", nodoA->getName()) == 0 || strcmp("JugadorIA", nodoA->getName()) == 0 || strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Estatico", nodoB->getName()) == 0)
		{
			//probando escudo de jugador y que me devuelva si tiene proteccion o no
			for (int j = 0; j < jugadores->getNumJugadores(); j++) {
				if (pj1.at(j) != NULL) {//tengo un personaje, y voy a ver si tiene escudo
					if (nodoA->getID()==pj1.at(j)->getID()){ 
						if (pj1.at(j)->getProteccion()==true) {
							cout << "estoy protegido" << endl;
							pj1.at(j)->setProteccion(false);
							protegido = true;
						}
					}
				}
			}
			//Turbo *t = mapa->getTurbo();
			cout << "Items: " << items.size() << std::endl;
			//t->setFrenadaActivo(pj1Col, true);
			int idB = nodoB->getID();
			for (int i=0;i<items.size();i++){
					cout << "ENTRO 1\n";
				if (items.at(i)->getID() == idB)
				{
					if(!protegido)
					{
					cout << "ENTRO2\n";
						if (strcmp("Aceite", items.at(i)->getNombre()) == 0){	//Si es aceite aplicamos el deslizamiento, sino es caja falsa
							aceite = true;
						}
						for(int j = 0; j< jugadores->getNumJugadores(); j++){
								//if(pj1.at(j)!=NULL)
							if (nodoA->getID()== pj1.at(j)->getID()){
								if(aceite)
								{
									pj1.at(j)->setAceite();
								}
								else
								{
									cout << "Caja Falsa\n";
									pj1.at(j)->resetFuerzas();
								}
							}
						}
					}
					protegido = false;
					items.at(i)->Delete();
					items.erase(items.begin()+i);
					pista->setItems(items);
					cout << "ENTRO\n";
					//}
					//else { 
					//	item->setColision(true);
					//}

					return true;
				}
			}

		}
	}else if (strcmp("Estatico", nodoA->getName()) == 0 && strcmp("Proyectil", nodoB->getName()) == 0 || strcmp("Proyectil", nodoA->getName()) == 0 && strcmp("Estatico", nodoB->getName()) == 0) {

		int idB = nodoB->getID();
		int idA = nodoA->getID();
		bool b=false;
		Item *it;
		for (int i=0;i<items.size();i++)
		{					
			if (items.at(i)->getID() == idA)
			{
				items.at(i)->Delete();
				items.erase(items.begin()+i);
				pista->setItems(items);	
				b=true;
				break;
			}
		}
		if (b==true){
			for (int j=0;j<items.size();j++)
			{
				if (items.at(j)->getID() == idB){
					
					items.at(j)->Delete();
					items.erase(items.begin()+j);
					pista->setItems(items);	
					return true;
				}
			}
		}
                        //encontrar proyectil y estatico con las ids de los nodos (y entonces borrar ambos)
              

		
	}
	return false;
}

//
// Comprobar colisiones entre Jugador y Proyectil
//
bool GestorColisiones::JugadorProyectil()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	Pista *pista = Pista::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	vector<Item *> items = pista->getItems();
	vector<btRigidBody *> objetos = bullet->getObjetos();
	Pista *mapa = Pista::getInstancia();
	bool protegido=false;

	if (strcmp("Jugador", nodoA->getName()) == 0 || strcmp("JugadorIA", nodoA->getName()) == 0 || strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Proyectil", nodoB->getName()) == 0)
		{
			//probando escudo de jugador y que me devuelva si tiene proteccion o no
			for (int j = 0; j < jugadores->getNumJugadores(); j++) {
				if (pj1.at(j) != NULL) {//tengo un personaje, y voy a ver si tiene escudo
					if (nodoA->getID()==pj1.at(j)->getID()){ 
						if (pj1.at(j)->getProteccion()==true) {
							cout << "estoy protegido " << endl;
							pj1.at(j)->setProteccion(false);
							protegido = true;
							break;
						}
					}
				}
			}

			int idB = nodoB->getID();
            for (int i=0;i<items.size();i++)
			{
				if (items.at(i)->getID() == idB)
				{
					cout << "Colisiono\n";
					if(!protegido)
					{
						for(int j = 0; j< jugadores->getNumJugadores(); j++){
							//if(pj1.at(j)!=NULL)
							if (nodoA->getID()== pj1.at(j)->getID()){
								pj1.at(j)->resetFuerzas();
							}
						}
					}
					protegido = false;
					items.at(i)->Delete();
					items.erase(items.begin()+i);
					pista->setItems(items);	

					return true;
				}
			}

			//cout << "Jugador - Turbo\n";
		}
	}
	return false;
}

//
// Comprobar colisiones entre Jugador y Caja
//
bool GestorColisiones::JugadorCaja(vector<Caja*> cajas)
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	Pista *pista = Pista::getInstancia();
	int tamCajas = pista->getTamCajas();
	bool colision = false;

	if (strcmp("Jugador", nodoA->getName()) == 0 || strcmp("JugadorIA", nodoA->getName()) == 0 || strcmp("JugadorRed", nodoA->getName()) == 0 )
	{
		if (strcmp("Caja", nodoB->getName()) == 0 && nodoB->isVisible())
		{
			colision = true;
			//cout << "Jug - Caja\n";
			int idB = nodoB->getID();
			//cout << "Tam Cajas: " << tamCajas << "- " << idB << endl;
			for (int i = 0; i < tamCajas; i++)
			{
				if (cajas.at(i) != NULL)
				{
					if (cajas.at(i)->getID() == idB)
					{
						for(int j = 0; j< jugadores->getNumJugadores(); j++)
							//if(pj1.at(j)!=NULL)
							if(nodoA->getID()== pj1.at(j)->getID()){
								cajas.at(i)->romper(pj1.at(j));
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
	vector<Item *> items = pista->getItems();
	vector<btRigidBody *> objetos = bullet->getObjetos();
	bool colision = false;

	if (strcmp("Destruible", nodoA->getName()) == 0)
	{
		if (strcmp("Proyectil", nodoB->getName()) == 0)
		{
			colision = true;
			//cout << "Destruible - Item\n";
			int idB = nodoB->getID();
			for (int i=0;i<items.size();i++)
			{
				//cout << "NodoB: " << idB << " == NodoItem: " << item->getNodo()->getID() << endl;
				if (items.at(i)->getID() == idB)
				{
					//cout << "Entro\n";
					items.at(i)->Delete();
					items.erase(items.begin()+i);
					pista->setItems(items);	

					return true;
				}
			}
		}
	}
	return colision;
}
