#include "GestorEscena.hpp"

GestorEscena *GestorEscena::instancia = nullptr;

GestorEscena::GestorEscena() {
}


GestorEscena::~GestorEscena() {
}

void GestorEscena::update() {
	comprobarInputs();
	escenaActiva->update();
}

void GestorEscena::cambiaEscena() {
}

GestorEscena *GestorEscena::getInstancia() {
	return instancia;
}

bool GestorEscena::agregaEscena(Escena *escena) {
	return false;
}

void GestorEscena::comprobarInputs() {
	
}
