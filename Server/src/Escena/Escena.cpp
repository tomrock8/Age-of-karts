#include "Escena.hpp"

Escena::Escena(tipo_escena tipo) {
	tipoEscena = tipo;
}

Escena::~Escena() { }

Escena::tipo_escena Escena::getTipoEscena() {
	return tipoEscena;
}
