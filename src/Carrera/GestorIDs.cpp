#include "GestorIDs.hpp"

GestorIDs *GestorIDs::instancia = new GestorIDs();

GestorIDs::GestorIDs(){
    //nodoA= new ISceneNode();
    id = 0;
}

void GestorIDs::setIdentifier(ISceneNode *nodo, const char *nombre){
    nodo->setID(id);
    id++;
    nodo->setName(nombre);
}
int GestorIDs::getIDLibre(){
    return id;
}
GestorIDs *GestorIDs::getInstancia(){
    if (instancia == NULL)
    instancia = new GestorIDs();

	return instancia;
}
GestorIDs::~GestorIDs(){
    delete instancia;
}