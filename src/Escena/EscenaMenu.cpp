#include "EscenaMenu.hpp"

EscenaMenu::EscenaMenu(tipo_escena tipo) : Escena(tipo) {
}

EscenaMenu::~EscenaMenu() {
}

void EscenaMenu::init() {

	
}

void EscenaMenu::dibujar() {
	Motor3d::instancia().dibujar();

}

void EscenaMenu::limpiar() {
	Motor3d::instancia().getScene()->clear();
}

void EscenaMenu::update() {
}

bool EscenaMenu::comprobarInputs() {
	CTeclado *teclado = CTeclado::getInstancia();

	if (teclado->isKeyDown(KEY_ESCAPE)) {
		return true; // Devuelve true para cambiar de escena
	}

	return false;
}
