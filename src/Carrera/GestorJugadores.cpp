#include "GestorJugadores.hpp"

GestorJugadores *GestorJugadores::instancia = NULL;

GestorJugadores::GestorJugadores(){
	pj= new Corredor*[6];
	numJugadores = 0;
    /*
	id = 999;// estaba int id = 0; . Se cambia a 999 para evitar posibles conflictos con ids 0 creadas en mapa 
	vector3df pos(0, 1, 300);
    vector3df fuera(0,0,0);
	pj[0] = new Corredor("assets/coche.obj", fuera);
	pj[0]->getNodo()->setID(id);
	id++;
    pj[1] = new Corredor("assets/coche.obj", fuera);
	pj[1]->getNodo()->setID(id);
	id++;
    pj[2] = new Corredor("assets/coche.obj", fuera);
	pj[2]->getNodo()->setID(id);
	id++;
    pj[3] = new Corredor("assets/coche.obj", fuera);
	pj[3]->getNodo()->setID(id);
	id++;
    pj[4] = new Corredor("assets/coche.obj", fuera);
	pj[4]->getNodo()->setID(id);
	id++;
    pj[5] = new Corredor("assets/coche.obj", fuera);
	pj[5]->getNodo()->setID(id);
	id++;
	*/



	/*	pj= new Corredor*[6];
   	GestorIDs *ids = GestorIDs::getInstancia();
	vector3df pos(0, 1, 300);
    vector3df fuera(0,0,0);
	pj[0] = new CorredorJugador("assets/coche.obj", pos);
	ids->setIdentifier(pj[0]->getNodo(),"Jugador");
    pj[1] = new Corredor("assets/coche.obj", fuera);
	ids->setIdentifier(pj[1]->getNodo(),"Jugador");
    pj[2] = new Corredor("assets/coche.obj", fuera);
	ids->setIdentifier(pj[2]->getNodo(),"Jugador");
    pj[3] = new Corredor("assets/coche.obj", fuera);
	ids->setIdentifier(pj[3]->getNodo(),"Jugador");
    pj[4] = new Corredor("assets/coche.obj", fuera);
	ids->setIdentifier(pj[4]->getNodo(),"Jugador");
    pj[5] = new Corredor("assets/coche.obj", fuera);
	ids->setIdentifier(pj[5]->getNodo(),"Jugador");*/
}

GestorJugadores::~GestorJugadores(){
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

GestorJugadores *GestorJugadores::getInstancia(){
    if(instancia == NULL)
        instancia = new GestorJugadores();

    return instancia;
}

Corredor **GestorJugadores::getJugadores(){
    return pj;
}

void GestorJugadores::setJugadores(Corredor **pj1){
    pj = pj1;
}

int GestorJugadores::getNumJugadores(){
	return numJugadores;
}

void GestorJugadores::aumentarJugadores(){
	numJugadores++;
}
void GestorJugadores::decrementarJugadores(){
	numJugadores--;
}