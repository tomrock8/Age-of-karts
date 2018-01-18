#include "GestorJugadores.hpp"

GestorJugadores *GestorJugadores::instancia = NULL;

GestorJugadores::GestorJugadores(){
	Corredor **pj= new Corredor*[6];
    pj[0] = NULL;
    pj[1] = NULL;
    pj[2] = NULL;
    pj[3] = NULL;
    pj[4] = NULL;
    pj[5] = NULL;
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
