#include "GestorJugadores.hpp"
#include "CorredorJugador.hpp"
#include "CorredorIA.hpp"
#include "CorredorRed.hpp"
#include "Corredor.hpp"

GestorJugadores *GestorJugadores::instancia = NULL;

GestorJugadores::GestorJugadores() {
	numJugadores = 0;
}

GestorJugadores::~GestorJugadores() {
//	cout << "ENTRO DESTRUCTOR DE GESTOR JUGADORES\n";
//	cout << "Poco a poco, que son " << numJugadores << "\n";
	for (int i = 0; i < numJugadores; i++) {
		cout << i << endl;
		//if (pj.at(i)) {
		if (strcmp(pj.at(i)->getNombre(), "Jugador") == 0) {
			CorredorJugador *cj = static_cast<CorredorJugador *>(pj.at(i));
			delete cj;
		}
		else {
			if (strcmp(pj.at(i)->getNombre(), "JugadorIA") == 0) {
				CorredorIA *cia = static_cast<CorredorIA *>(pj.at(i));
				cia->deleteArbol();
				delete cia;
			}
			else {
				if (strcmp(pj.at(i)->getNombre(), "JugadorRed") == 0) {
					CorredorRed *cred = static_cast<CorredorRed *>(pj.at(i));
					delete cred;
				}
			}
		}
	}
	pj.clear();
	numJugadores = 0;
	instancia = nullptr;
//	cout << "SALGO DESTRUCTOR DE GESTOR JUGADORES\n";
}

GestorJugadores *GestorJugadores::getInstancia() {
	if (instancia == NULL)
		instancia = new GestorJugadores();

	return instancia;
}

std::vector<Corredor*> GestorJugadores::getJugadores() {
	return pj;
}

void GestorJugadores::setJugadores(std::vector<Corredor*> pj1) {
	pj = pj1;
}

int GestorJugadores::getNumJugadores() {
	return numJugadores;
}

void GestorJugadores::aumentarJugadores() {
	numJugadores++;
}

void GestorJugadores::decrementarJugadores() {
	numJugadores--;
}