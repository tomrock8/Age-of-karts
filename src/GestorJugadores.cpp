#include "GestorJugadores.hpp"

GestorJugadores *GestorJugadores::instancia = NULL;

GestorJugadores::GestorJugadores(){
	//Corredor **pj= new Corredor*[6];
    id = 999;// estaba int id = 0; . Se cambia a 999 para evitar posibles conflictos con ids 0 creadas en mapa 
	vector3df pos(0, 1, 300);
    vector3df fuera(0,0,0);
	pj[0] = new CorredorJugador("assets/coche.obj", pos);
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
}

GestorJugadores::~GestorJugadores(){

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

int GestorJugadores::getID(){
    return id;
}

void GestorJugadores::setID(int idp){
    id=idp;
}
