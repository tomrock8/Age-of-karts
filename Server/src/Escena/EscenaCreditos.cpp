#include "EscenaCreditos.hpp"

EscenaCreditos::EscenaCreditos() : Escena(Escena::tipo_escena::CREDITOS) {
	/*texto = L"Autores:\n\n";
	texto += "  -  Angel Gonzalez Cobo\n\n";
	texto += "  -  Carlos Jimenez Quinto\n\n";
	texto += "  -  Jordi Serna Valero\n\n";
	texto += "  -  Javier Martinez Arias\n\n";
	texto += "  -  Salvador Mateo Tomas\n\n";
	texto += "  -  Santiago Martinez Gutierrez\n\n";
	texto += "SocketWar 2017\n\n";
	texto += "Grado en ingenieria multimedia. Universidad de Alicante.\n\n";
	texto += "INTRO - SALIR";

	u16 xPos = Motor3d::instancia().getAnchoPantalla() / 3;
	u16 yPos = Motor3d::instancia().getAltoPantalla() / 4;

	fuente = Motor3d::instancia().getGUI()->getFont("assets/fuente.bmp");
	textoUI = Motor3d::instancia().getGUI()->addStaticText(
		texto.c_str(),				 // Texto
		rect<s32>(xPos, 2 * yPos, 2 * xPos, 4* yPos), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,							 // Id del elemento
		false);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);
	
	logoAOK = Motor3d::instancia().getDriver()->getTexture("assets/logoAOK.png");
	Motor3d::instancia().getDriver()->makeColorKeyTexture(logoAOK, core::position2d<s32>(0, 0));*/
}

EscenaCreditos::~EscenaCreditos() {
	cout << "Destructor ESCENA CREDITOS. Entro.";
	limpiar();
	cout << "Salgo.\n";
}

void EscenaCreditos::init() {
}

void EscenaCreditos::dibujar() {
	/*Motor3d::instancia().iniciarDibujado();

	//Motor3d::instancia().getDriver()->draw2DImage(logoAOK, rect<s32>(0, 0, 100, 100), 0, SColor(255, 255, 255, 255), true);
	Motor3d::instancia().getDriver()->draw2DImage(logoAOK, position2d<s32>(Motor3d::instancia().getAnchoPantalla() / 4, 0),
		core::rect<s32>(0, 0, logoAOK->getSize().Width, logoAOK->getSize().Height), 0,
		video::SColor(255, 255, 255, 255), true);

	Motor3d::instancia().dibujar();
	Motor3d::instancia().terminarDibujado();*/
}

void EscenaCreditos::limpiar() {
	/*Motor3d::instancia().getScene()->clear();
	Motor3d::instancia().getGUI()->clear();*/
}

void EscenaCreditos::update() {
}

Escena::tipo_escena EscenaCreditos::comprobarInputs() {

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS){
		return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
	}
	else {
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_KP_1) == GLFW_PRESS){
			cout << "SINGLE PLAYER\n";
		}
		else {
			if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_KP_2) == GLFW_PRESS){
				cout << "MULTI PLAYER\n";
			}
		}
	}

	return Escena::tipo_escena::CREDITOS;
}
