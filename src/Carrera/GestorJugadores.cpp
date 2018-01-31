#include "GestorJugadores.hpp"

GestorJugadores *GestorJugadores::instancia = NULL;

GestorJugadores::GestorJugadores() {
	pj = new Corredor*[6];
	numJugadores = 0;
}

GestorJugadores::~GestorJugadores() {
	cout << "\nENTRO DESTRUCTOR DE GESTOR JUGADORES";
	cout << "\nPoco a poco, que son " << numJugadores << "\n";
	for (int i = 0; i < numJugadores; i++) {
		cout << "A por el " << i;
		if (pj[i]) {
			if (strcmp(pj[i]->getNodo()->getName(), "Jugador") == 0) {
				CorredorJugador *cj = static_cast<CorredorJugador *>(pj[i]);
				delete cj;
			}
			else {
				if (strcmp(pj[i]->getNodo()->getName(), "JugadorIA") == 0) {
					CorredorIA *cia = static_cast<CorredorIA *>(pj[i]);
					delete cia;
				}
				else {
					if (strcmp(pj[i]->getNodo()->getName(), "JugadorRed") == 0) {
						CorredorRed *cred = static_cast<CorredorRed *>(pj[i]);
						delete cred;
					}
				}
			}

		}
		cout << "! ";
	}

	instancia = nullptr;
	cout << "\nSALGO DESTRUCTOR DE GESTOR JUGADORES";
}

GestorJugadores *GestorJugadores::getInstancia() {
	if (instancia == NULL)
		instancia = new GestorJugadores();

	return instancia;
}

Corredor **GestorJugadores::getJugadores() {
	return pj;
}

void GestorJugadores::setJugadores(Corredor **pj1) {
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