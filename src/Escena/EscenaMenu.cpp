#include "EscenaMenu.hpp"

EscenaMenu::EscenaMenu() : Escena(Escena::tipo_escena::MENU) {
	
	
/*	
	texto = L"1 - INCIAR JUEGO SINGLEPLAYER\n\n";
	texto += "2 - INCIAR JUEGO MULTIPLAYER\n\n";
	texto += "3  - OPCIONES (IN DEVELOPMENT)\n\n";
	texto += "4 - CREDITOS\n\n";
	texto += "ESC - SALIR\n\n\n";
	texto += "        SocketWar 2017";

	pressed = true;
	ipConexion="";
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
	Motor3d::instancia().getDriver()->makeColorKeyTexture(logoAOK, core::position2d<s32>(0, 0));*/
}

EscenaMenu::~EscenaMenu() {
	cout << "Destructor ESCENA MENU. Entro.";
	limpiar();
	cout << "Salgo.\n";
}

void EscenaMenu::init() {


}

void EscenaMenu::dibujar() {
	/*Motor3d::instancia().iniciarDibujado();

	Motor3d::instancia().getDriver()->draw2DImage(logoAOK, position2d<s32>(Motor3d::instancia().getAnchoPantalla() / 4, 0),
		core::rect<s32>(0, 0, logoAOK->getSize().Width, logoAOK->getSize().Height), 0,
		video::SColor(255, 255, 255, 255), true);

	Motor3d::instancia().dibujar();
	Motor3d::instancia().terminarDibujado();*/
}

void EscenaMenu::limpiar() {
	/*Motor3d::instancia().getScene()->clear();
	Motor3d::instancia().getGUI()->clear();*/
}

void EscenaMenu::update() {
}

Escena::tipo_escena EscenaMenu::comprobarInputs() {

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS){
		if(!pressed){
			return Escena::tipo_escena::SALIR; // Devuelve el estado de las escenas para que salga
		}
	}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_1) == GLFW_PRESS){
		cout << "SINGLE PLAYER\n";
		ipConexion="offline";
		return Escena::tipo_escena::LOBBY;
	}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_2) == GLFW_PRESS){
		cout << "MULTI PLAYER\n";
		ipConexion="";
		return Escena::tipo_escena::LOBBY;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_3) == GLFW_PRESS){
			cout << "OPCIONES\n";
	}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_4) == GLFW_PRESS){
		cout << "CREDITOS\n";
		return Escena::tipo_escena::CREDITOS;
	}else pressed = false;

	return Escena::tipo_escena::MENU;
}
std::string EscenaMenu::getIpConexion(){
	return ipConexion;
}