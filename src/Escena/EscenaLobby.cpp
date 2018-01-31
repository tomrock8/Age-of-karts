#include "EscenaLobby.hpp"

EscenaLobby::EscenaLobby() : Escena(Escena::tipo_escena::LOBBY) {
	nElementos = 0;
	texto = L"ESC - SALIR\n\n";
	texto = "Introduce IP para iniciar partida online: 192.168.1.";


	u16 xPos = Motor3d::instancia().getAnchoPantalla() / 3;
	u16 yPos = Motor3d::instancia().getAltoPantalla() / 4;

	fuente = Motor3d::instancia().getGUI()->getFont("assets/fuente.bmp");
	textoUI = Motor3d::instancia().getGUI()->addStaticText(
		texto.c_str(),				 // Texto
		rect<s32>(xPos, 2 * yPos, 2 * xPos, 3 * yPos), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,							 // Id del elemento
		false);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);

	logoAOK = Motor3d::instancia().getDriver()->getTexture("assets/logoAOK.png");
	Motor3d::instancia().getDriver()->makeColorKeyTexture(logoAOK, core::position2d<s32>(0, 0));
}

EscenaLobby::~EscenaLobby() {
	cout << "Destructor ESCENA LOBBY. Entro.";
	limpiar();
	cout << "Salgo.\n";
}

void EscenaLobby::init() {
}

void EscenaLobby::dibujar() {
	Motor3d::instancia().iniciarDibujado();

	//Motor3d::instancia().getDriver()->draw2DImage(logoAOK, rect<s32>(0, 0, 100, 100), 0, SColor(255, 255, 255, 255), true);
	Motor3d::instancia().getDriver()->draw2DImage(logoAOK, position2d<s32>(Motor3d::instancia().getAnchoPantalla() / 4, 0),
		core::rect<s32>(0, 0, logoAOK->getSize().Width, logoAOK->getSize().Height), 0,
		video::SColor(255, 255, 255, 255), true);

	Motor3d::instancia().dibujar();
	Motor3d::instancia().terminarDibujado();
}

void EscenaLobby::limpiar() {
	Motor3d::instancia().getScene()->clear();
	Motor3d::instancia().getGUI()->clear();
}

void EscenaLobby::update() {
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {
	CTeclado *teclado = CTeclado::getInstancia();

	if (teclado->isKeyDown(KEY_ESCAPE)) {
		return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
	}

	if (nElementos < 3) {
		if (teclado->isKeyDown(KEY_KEY_1)) {
			texto += "1";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_2)) {
			texto += "2";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_3)) {
			texto += "3";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_4)) {
			texto += "4";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_4)) {
			texto += "4";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_5)) {
			texto += "5";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_6)) {
			texto += "6";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_7)) {
			texto += "7";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_8)) {
			texto += "8";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_9)) {
			texto += "9";
			nElementos++;
		}
		if (teclado->isKeyDown(KEY_KEY_0)) {
			texto += "0";
			nElementos++;
		}
		textoUI->setText(this->texto.c_str());
		return Escena::tipo_escena::LOBBY;
	}


	return Escena::tipo_escena::CARRERA;
}
