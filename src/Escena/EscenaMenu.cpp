#include "EscenaMenu.hpp"

EscenaMenu::EscenaMenu(tipo_escena tipo) : Escena(tipo) {
	texto = L"AGE OF KARTS\n\n\n";
	texto += "1 - INCIAR JUEGO SINGLEPLAYER\n\n";
	texto += "2 - INCIAR JUEGO MULTIPLAYER\n\n";
	texto += "OPCIONES\n\nCREDITOS\n\nESC - SALIR\n\n\n";
	texto += "        SocketWar 2017";


	u16 xPos = Motor3d::instancia().getAnchoPantalla() / 3;
	u16 yPos = Motor3d::instancia().getAltoPantalla() / 3;

	fuente = Motor3d::instancia().getGUI()->getFont("assets/fuente.bmp");
	textoUI = Motor3d::instancia().getGUI()->addStaticText(
		texto.c_str(),				 // Texto
		rect<s32>(xPos, yPos, 2 * xPos, 2 * yPos), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,							 // Id del elemento
		false);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);
}

EscenaMenu::~EscenaMenu() {
}

void EscenaMenu::init() {


}

void EscenaMenu::dibujar() {
	Motor3d::instancia().iniciarDibujado();
	Motor3d::instancia().dibujar();
	Motor3d::instancia().terminarDibujado();
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
