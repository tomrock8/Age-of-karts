#include "EscenaMenu.hpp"

EscenaMenu::EscenaMenu(tipo_escena tipo) : Escena(tipo) {
	texto = L"1 - INCIAR JUEGO SINGLEPLAYER\n\n";
	texto += "2 - INCIAR JUEGO MULTIPLAYER\n\n";
	texto += "OPCIONES\n\nCREDITOS\n\nESC - SALIR\n\n\n";
	texto += "        SocketWar 2017";


	u16 xPos = Motor3d::instancia().getAnchoPantalla() / 3;
	u16 yPos = Motor3d::instancia().getAltoPantalla() / 4;

	fuente = Motor3d::instancia().getGUI()->getFont("assets/fuente.bmp");
	textoUI = Motor3d::instancia().getGUI()->addStaticText(
		texto.c_str(),				 // Texto
		rect<s32>(xPos, 2 * yPos, 2 * xPos, 3* yPos), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,							 // Id del elemento
		false);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);
	
	logoAOK = Motor3d::instancia().getDriver()->getTexture("assets/logoAOK.png");
	Motor3d::instancia().getDriver()->makeColorKeyTexture(logoAOK, core::position2d<s32>(0, 0));
}

EscenaMenu::~EscenaMenu() {
}

void EscenaMenu::init() {


}

void EscenaMenu::dibujar() {
	Motor3d::instancia().iniciarDibujado();

	//Motor3d::instancia().getDriver()->draw2DImage(logoAOK, rect<s32>(0, 0, 100, 100), 0, SColor(255, 255, 255, 255), true);
	Motor3d::instancia().getDriver()->draw2DImage(logoAOK, position2d<s32>(Motor3d::instancia().getAnchoPantalla() / 4, 0),
		core::rect<s32>(0, 0, logoAOK->getSize().Width, logoAOK->getSize().Height), 0,
		video::SColor(255, 255, 255, 255), true);

	Motor3d::instancia().dibujar();
	Motor3d::instancia().terminarDibujado();
}

void EscenaMenu::limpiar() {
	Motor3d::instancia().getScene()->clear();
}

void EscenaMenu::update() {
}

Escena::tipo_escena EscenaMenu::comprobarInputs() {
	CTeclado *teclado = CTeclado::getInstancia();

	if (teclado->isKeyDown(KEY_ESCAPE)) {
		return Escena::tipo_escena::SALIR; // Devuelve el estado de las escenas para que salga
	}
	else {
		if (teclado->isKeyDown(KEY_KEY_1)) {
			cout << "SINGLE PLAYER\n";
		}
		else {
			if (teclado->isKeyDown(KEY_KEY_2)) {
				cout << "MULTI PLAYER\n";
			}
		}
	}

	return Escena::tipo_escena::MENU;
}
