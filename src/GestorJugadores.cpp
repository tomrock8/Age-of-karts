#include "GestorJugadores.hpp"

GestorJugadores *GestorJugadores::instancia = NULL;

GestorJugadores::GestorJugadores(){

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
