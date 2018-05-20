#include "GestorColisiones.hpp"
#include "Pista.hpp"
#include "Item.hpp"

void GestorColisiones::IniciarTimer() {
	tiempoInicio = glfwGetTime();
}


GestorColisiones::~GestorColisiones() {
	pj1.clear();
	nodoA = nullptr;
	nodoB = nullptr;
}


void GestorColisiones::ComprobarColisiones() {
	btDynamicsWorld *mundo = MotorFisicas::getInstancia()->getMundo();
	pj1 = GestorJugadores::getInstancia()->getJugadores();
	int numManifolds = mundo->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold *contactManifold = mundo->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject *obA = contactManifold->getBody0();
		const btCollisionObject *obB = contactManifold->getBody1();

		nodoA = static_cast<obj3D *>(obA->getUserPointer());
		nodoB = static_cast<obj3D *>(obB->getUserPointer());

		if (nodoA != 0 && nodoB != 0) {
			if (ItemTeledirigidoWaypoint())continue;
			if (JugadorCaja(Pista::getInstancia()->getArrayCaja()))continue;
			if (JugadorTurbo())continue;
			if (JugadorWaypoint())continue;
			if (HabilidadesJugadores())continue;
			if (Escudoitems())break;
			if (HabilidadesItems())break;
			if (JugadorProyectil())break;
			if (JugadorEstatico())break;
			if (habilidadVShabilidad())continue;
			//if (JugadorItemTeledirigido())continue;
		}
	}
	GestorJugadores::getInstancia()->setJugadores(pj1);
}



bool GestorColisiones::JugadorWaypoint() {

	if (strcmp("Jugador", nodoA->getName()) == 0 ||
		strcmp("JugadorIA", nodoA->getName()) == 0 ||
		strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Waypoint", nodoB->getName()) == 0) {
			for (int i = 0; i < GestorJugadores::getInstancia()->getNumJugadores(); i++) {
				//if(pj1.at(i)!=NULL)
				if (nodoA->getID() == pj1.at(i)->getNodo()->getID()) {
					if (nodoB->getID() - 7 == 0 && pj1.at(i)->getWaypointSiguiente()->getWaypoint()->getID() - 7 == 0) {
						float timediff_sec = glfwGetTime() - tiempoInicio;
						if (pj1.at(i)->getMaxVuetas() >= pj1.at(i)->getVueltas()) {
							pj1.at(i)->setTiempoVuelta(timediff_sec - pj1.at(i)->getTiempoVueltaTotal());
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


bool GestorColisiones::ItemTeledirigidoWaypoint() {
	std::vector<Item *> items = Pista::getInstancia()->getItems();

	if (strcmp("Estatico", nodoA->getName()) == 0) {
		if (strcmp("Waypoint", nodoB->getName()) == 0) {
			int idA = nodoA->getID();
			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idA) {

					if (strcmp("ItemTeledirigido", items.at(i)->getNombre()) == 0) {
						items.at(i)->setColision(nodoB->getID());

						return true;
					}

				}

			}
		}
	}

	if (strcmp("Estatico", nodoB->getName()) == 0) {
		if (strcmp("Waypoint", nodoA->getName()) == 0) {
			int idB = nodoB->getID();
			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					if (strcmp("ItemTeledirigido", items.at(i)->getNombre()) == 0) {
						items.at(i)->setColision(nodoA->getID());
						return true;
					}
				}

			}
		}
	}

	return false;
}



bool GestorColisiones::habilidadVShabilidad() {
	std::vector<Item *> items = Pista::getInstancia()->getItems();

	if (strcmp("HabilidadPirata", nodoA->getName()) == 0) {
		if (strcmp("HabilidadVikingo", nodoB->getName()) == 0
			|| strcmp("HabilidadGladiador", nodoB->getName()) == 0
			|| strcmp("HabilidadChino", nodoB->getName()) == 0)
		{
			int idA = nodoA->getID();
			int idB = nodoB->getID();

			for (int j = 0; j < items.size(); j++) {
				if (items.at(j)->getID() == idA) {
					delete items.at(j);
					items.erase(items.begin() + j);
				}
			}

			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					delete items.at(i);
					items.erase(items.begin() + i);

				}
			}
			Pista::getInstancia()->setItems(items);
			return true;
		}
	}

	if (strcmp("HabilidadVikingo", nodoA->getName()) == 0) {
		if (strcmp("HabilidadPirata", nodoB->getName()) == 0
			|| strcmp("HabilidadGladiador", nodoB->getName()) == 0
			|| strcmp("HabilidadChino", nodoB->getName()) == 0)
		{
			int idA = nodoA->getID();
			int idB = nodoB->getID();

			for (int j = 0; j < items.size(); j++) {
				if (items.at(j)->getID() == idA) {
					delete items.at(j);
					items.erase(items.begin() + j);
				}
			}

			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					delete items.at(i);
					items.erase(items.begin() + i);

				}
			}

			Pista::getInstancia()->setItems(items);
			return true;
		}
	}

	if (strcmp("HabilidadGladiador", nodoA->getName()) == 0) {
		if (strcmp("HabilidadPirata", nodoB->getName()) == 0
			|| strcmp("HabilidadVikingo", nodoB->getName()) == 0
			|| strcmp("HabilidadChino", nodoB->getName()) == 0)
		{
			int idA = nodoA->getID();
			int idB = nodoB->getID();

			for (int j = 0; j < items.size(); j++) {
				if (items.at(j)->getID() == idA) {
					delete items.at(j);
					items.erase(items.begin() + j);
				}
			}

			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					delete items.at(i);
					items.erase(items.begin() + i);

				}
			}
			Pista::getInstancia()->setItems(items);
			return true;
		}
	}

	if (strcmp("HabilidadChino", nodoA->getName()) == 0) {
		if (strcmp("HabilidadPirata", nodoB->getName()) == 0
			|| strcmp("HabilidadVikingo", nodoB->getName()) == 0
			|| strcmp("HabilidadGladiador", nodoB->getName()) == 0)
		{
			int idA = nodoA->getID();
			int idB = nodoB->getID();

			for (int j = 0; j < items.size(); j++) {
				if (items.at(j)->getID() == idA) {
					delete items.at(j);
					items.erase(items.begin() + j);
				}
			}

			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					delete items.at(i);
					items.erase(items.begin() + i);

				}
			}

			Pista::getInstancia()->setItems(items);
			return true;
		}
	}

	return false;
}


bool GestorColisiones::HabilidadesJugadores() {
	std::vector<Item *> items = Pista::getInstancia()->getItems();
	int idPadreUlti = 0;

	if (strcmp("Jugador", nodoA->getName()) == 0 ||
		strcmp("JugadorIA", nodoA->getName()) == 0 ||
		strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("HabilidadVikingo", nodoB->getName()) == 0
			|| strcmp("HabilidadPirata", nodoB->getName()) == 0
			|| strcmp("HabilidadGladiador", nodoB->getName()) == 0
			|| strcmp("HabilidadChino", nodoB->getName()) == 0) {

			int idA = nodoA->getID();
			int idB = nodoB->getID();

			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					idPadreUlti = items.at(i)->getIDPadre();
				}
			}

			if (idPadreUlti == idA)
				return false;
			else {
				if (!pj1.at(idA)->getInmunidad()) {
					pj1.at(idA)->resetFuerzas();
					pj1.at(idA)->setInmunidad(true);
				}
				return true;
			}
		}
	}


	if (strcmp("Jugador", nodoB->getName()) == 0 ||
		strcmp("JugadorIA", nodoB->getName()) == 0 ||
		strcmp("JugadorRed", nodoB->getName()) == 0)
	{
		if (strcmp("HabilidadVikingo", nodoA->getName()) == 0
			|| strcmp("HabilidadPirata", nodoA->getName()) == 0
			|| strcmp("HabilidadGladiador", nodoA->getName()) == 0
			|| strcmp("HabilidadChino", nodoA->getName()) == 0) {

			int idA = nodoA->getID();
			int idB = nodoB->getID();

			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idA) {
					idPadreUlti = items.at(i)->getIDPadre();
				}
			}

			if (idPadreUlti == idB)
				return false;
			else {
				if (!pj1.at(idB)->getInmunidad()) {
					pj1.at(idB)->resetFuerzas();
					pj1.at(idB)->setInmunidad(true);
				}
				return true;
			}
		}
	}

	return false;
}

bool GestorColisiones::Escudoitems() {
	std::vector<Item *> items = Pista::getInstancia()->getItems();

	int idEscudo = 0;
	int idObjeto = 0;
	int idPadreEscudo = 0;
	int idPadreObjeto = 0;

	if (strcmp("Escudo", nodoA->getName()) == 0) {
		if (strcmp("Estatico", nodoB->getName()) == 0
			|| strcmp("Proyectil", nodoB->getName()) == 0
			|| strcmp("HabilidadVikingo", nodoB->getName()) == 0
			|| strcmp("HabilidadPirata", nodoB->getName()) == 0
			|| strcmp("HabilidadGladiador", nodoB->getName()) == 0
			|| strcmp("HabilidadChino", nodoB->getName()) == 0) {

			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i = 0; i < items.size(); i++) {

				if (items.at(i)->getID() == idA) {
					idPadreEscudo = items.at(i)->getIDPadre();
					idEscudo = i;
				}

				if (items.at(i)->getID() == idB) {
					idPadreObjeto = items.at(i)->getIDPadre();
					idObjeto = i;

				}
			}

			if (idPadreEscudo == idPadreObjeto) {
				return false;

			}
			else {

				delete items.at(idEscudo);
				items.erase(items.begin() + idEscudo);

				if (strcmp("Estatico", nodoB->getName()) == 0 || strcmp("Proyectil", nodoB->getName()) == 0) {
					for (int i = 0; i < items.size(); i++) {
						if (items.at(i)->getID() == idB) {
							delete items.at(i);
							items.erase(items.begin() + i);

						}
					}
				}
				Pista::getInstancia()->setItems(items);
				return true;
			}
		}
	}


	if (strcmp("Escudo", nodoB->getName()) == 0) {
		if (strcmp("Estatico", nodoA->getName()) == 0
			|| strcmp("Proyectil", nodoA->getName()) == 0
			|| strcmp("HabilidadVikingo", nodoA->getName()) == 0
			|| strcmp("HabilidadPirata", nodoA->getName()) == 0
			|| strcmp("HabilidadGladiador", nodoA->getName()) == 0
			|| strcmp("HabilidadChino", nodoA->getName()) == 0) {

			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i = 0; i < items.size(); i++) {

				if (items.at(i)->getID() == idB) {
					idPadreEscudo = items.at(i)->getIDPadre();
					idEscudo = i;
				}

				if (items.at(i)->getID() == idA) {
					idPadreObjeto = items.at(i)->getIDPadre();
					idObjeto = i;

				}
			}

			if (idPadreEscudo == idPadreObjeto) {
				return false;

			}
			else {
				delete items.at(idEscudo);
				items.erase(items.begin() + idEscudo);
				if (strcmp("Estatico", nodoA->getName()) == 0 || strcmp("Proyectil", nodoA->getName()) == 0) {
					for (int i = 0; i < items.size(); i++) {
						if (items.at(i)->getID() == idA) {
							delete items.at(i);
							items.erase(items.begin() + i);
						}
					}
				}
				Pista::getInstancia()->setItems(items);
				return true;
			}
		}
	}

	return false;
}


bool GestorColisiones::HabilidadesItems() {
	std::vector<Item *> items = Pista::getInstancia()->getItems();
	int idUlti = 0;
	int idObjeto = 0;
	int idPadreUlti = 0;
	int idPadreObjeto = 0;

	if (strcmp("HabilidadVikingo", nodoA->getName()) == 0
		|| strcmp("HabilidadPirata", nodoA->getName()) == 0
		|| strcmp("HabilidadGladiador", nodoA->getName()) == 0
		|| strcmp("HabilidadChino", nodoA->getName()) == 0) {

		if (strcmp("Estatico", nodoB->getName()) == 0
			|| strcmp("Proyectil", nodoB->getName()) == 0) {

			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idA) {
					idPadreUlti = items.at(i)->getIDPadre();
					idUlti = i;
				}

				if (items.at(i)->getID() == idB) {
					idPadreObjeto = items.at(i)->getIDPadre();
					idObjeto = i;
				}
			}

			if (idPadreUlti == idPadreObjeto) {
				return false;
			}
			else {
				delete items.at(idObjeto);
				items.erase(items.begin() + idObjeto);
				Pista::getInstancia()->setItems(items);
				return true;
			}
		}
	}


	if (strcmp("HabilidadVikingo", nodoB->getName()) == 0
		|| strcmp("HabilidadPirata", nodoB->getName()) == 0
		|| strcmp("HabilidadGladiador", nodoB->getName()) == 0
		|| strcmp("HabilidadChino", nodoB->getName()) == 0) {

		if (strcmp("Estatico", nodoA->getName()) == 0
			|| strcmp("Proyectil", nodoA->getName()) == 0) {

			int idA = nodoA->getID();
			int idB = nodoB->getID();
			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					idPadreUlti = items.at(i)->getIDPadre();
					idUlti = i;
				}

				if (items.at(i)->getID() == idA) {
					idPadreObjeto = items.at(i)->getIDPadre();
					idObjeto = i;
				}
			}

			if (idPadreUlti == idPadreObjeto) {
				return false;
			}
			else {
				delete items.at(idObjeto);
				items.erase(items.begin() + idObjeto);
				Pista::getInstancia()->setItems(items);
				return true;
			}
		}
	}
	return false;
}

// Comprobar colisiones entre Jugador y turbo
bool GestorColisiones::JugadorTurbo() {
	if (strcmp("Jugador", nodoA->getName()) == 0 ||
		strcmp("JugadorIA", nodoA->getName()) == 0 ||
		strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Turbo", nodoB->getName()) == 0) {
			for (int i = 0; i < GestorJugadores::getInstancia()->getNumJugadores(); i++) {
				if (nodoA->getID() == pj1.at(i)->getNodo()->getID()) {
					pj1.at(i)->setTurbo(true, false, 10000, 2);

				}
			}
			return true;
		}
	}
	return false;
}

// Comprobar colisiones entre Jugador y Estatico
bool GestorColisiones::JugadorEstatico() {
	std::vector<Item *> items = Pista::getInstancia()->getItems();
	std::vector<btRigidBody *> objetos = MotorFisicas::getInstancia()->getObjetos();

	bool protegido = false;
	bool aceite = false;

	if (strcmp("Jugador", nodoA->getName()) == 0 ||
		strcmp("JugadorIA", nodoA->getName()) == 0 ||
		strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Estatico", nodoB->getName()) == 0) {
			//probando escudo de jugador y que me devuelva si tiene proteccion o no
			for (int j = 0; j < GestorJugadores::getInstancia()->getNumJugadores(); j++) {
				if (pj1.at(j) != NULL) {//tengo un personaje, y voy a ver si tiene escudo
					if (nodoA->getID() == pj1.at(j)->getID()) {
						if (pj1.at(j)->getInmunidad()) {
							protegido = true;
						}
					}
				}
			}
			//cout << "Items: " << items.size() << std::endl;

			int idB = nodoB->getID();
			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {
					if (!protegido) {
						if (strcmp("Aceite", items.at(i)->getNombre()) == 0) {	//Si es aceite aplicamos el deslizamiento, sino es caja falsa
							aceite = true;
						}
						for (int j = 0; j < GestorJugadores::getInstancia()->getNumJugadores(); j++) {
							if (nodoA->getID() == pj1.at(j)->getID()) {
								if (aceite) {
									pj1.at(j)->setAceite();
								}
								else {
									pj1.at(j)->resetFuerzas();
								}
								pj1.at(j)->setInmunidad(true);
							}
						}
					}
					protegido = false;
					delete items.at(i);
					items.erase(items.begin() + i);
					Pista::getInstancia()->setItems(items);
					return true;
				}
			}
		}
	}
	else if (strcmp("Estatico", nodoA->getName()) == 0 &&
		strcmp("Proyectil", nodoB->getName()) == 0 ||
		strcmp("Proyectil", nodoA->getName()) == 0 &&
		strcmp("Estatico", nodoB->getName()) == 0)
	{

		int idB = nodoB->getID();
		int idA = nodoA->getID();
		bool b = false;
		Item *it = NULL;
		for (int i = 0; i < items.size(); i++) {
			if (items.at(i)->getID() == idA) {
				delete items.at(i);
				items.erase(items.begin() + i);
				Pista::getInstancia()->setItems(items);
				b = true;
				break;
			}
		}
		if (b == true) {
			for (int j = 0; j < items.size(); j++) {
				if (items.at(j)->getID() == idB) {
					delete items.at(j);
					items.erase(items.begin() + j);
					Pista::getInstancia()->setItems(items);
					return true;
				}
			}
		}
	}
	return false;
}

// Comprobar colisiones entre Jugador y Proyectil
bool GestorColisiones::JugadorProyectil() {
	std::vector<Item *> items = Pista::getInstancia()->getItems();
	std::vector<btRigidBody *> objetos = MotorFisicas::getInstancia()->getObjetos();
	bool protegido = false;

	if (strcmp("Jugador", nodoA->getName()) == 0 ||
		strcmp("JugadorIA", nodoA->getName()) == 0 ||
		strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Proyectil", nodoB->getName()) == 0) {
			//probando escudo de jugador y que me devuelva si tiene proteccion o no
			for (int j = 0; j < GestorJugadores::getInstancia()->getNumJugadores(); j++) {
				if (pj1.at(j) != NULL) {//tengo un personaje, y voy a ver si tiene escudo
					if (nodoA->getID() == pj1.at(j)->getID()) {
						if (pj1.at(j)->getInmunidad()) {
							protegido = true;
							break;
						}
					}
				}
			}

			int idB = nodoB->getID();
			for (int i = 0; i < items.size(); i++) {
				if (items.at(i)->getID() == idB) {

					if (!protegido) {
						for (int j = 0; j < GestorJugadores::getInstancia()->getNumJugadores(); j++) {
							//if(pj1.at(j)!=NULL)
							if (nodoA->getID() == pj1.at(j)->getID()) {
								pj1.at(j)->resetFuerzas();
								pj1.at(j)->setInmunidad(true);
							}
						}
					}
					protegido = false;
					delete items.at(i);
					items.erase(items.begin() + i);
					Pista::getInstancia()->setItems(items);

					return true;
				}
			}
		}
	}
	return false;
}

// Comprobar colisiones entre Jugador y Caja
bool GestorColisiones::JugadorCaja(std::vector<Caja*> cajas) {
	int tamCajas = Pista::getInstancia()->getTamCajas();
	bool colision = false;

	if (strcmp("Jugador", nodoA->getName()) == 0 ||
		strcmp("JugadorIA", nodoA->getName()) == 0 ||
		strcmp("JugadorRed", nodoA->getName()) == 0)
	{
		if (strcmp("Caja", nodoB->getName()) == 0 && nodoB->isVisible()) {
			colision = true;
			int idB = nodoB->getID();
			for (int i = 0; i < tamCajas; i++) {
				if (cajas.at(i) != NULL) {
					if (cajas.at(i)->getID() == idB) {
						for (int j = 0; j < GestorJugadores::getInstancia()->getNumJugadores(); j++) {
							if (nodoA->getID() == pj1.at(j)->getID()) {
								cajas.at(i)->romper(pj1.at(j));
							}
						}
					}
				}
			}
			Pista::getInstancia()->setArrayCaja(cajas);
		}
	}
	return colision;
}