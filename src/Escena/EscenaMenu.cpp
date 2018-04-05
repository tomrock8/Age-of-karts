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

	//Se asigna la opcion del menu al primer boton
	optionMenu = 0;

	//Se crea el hud del menu principal
	TMotor::instancia().newHud("MainMenuHUD");
	//Se añaden los distintos elementos del hud y se posicionan correctamente
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "logo", "assets/HUD/MainMenu/aok_logo.png");
	//--LOGO--
	TMotor::instancia().getHud("MainMenuHUD")->traslateElement("logo", 0.0f, 0.45f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "local", "assets/HUD/MainMenu/btn_local_normal.png");
	//--BOTONES--
	TMotor::instancia().getHud("MainMenuHUD")->traslateElement("local", -0.01f, -0.1f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "online", "assets/HUD/MainMenu/btn_online_normal.png");
	TMotor::instancia().getHud("MainMenuHUD")->traslateElement("online", -0.01f, -0.3f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "opciones", "assets/HUD/MainMenu/btn_opciones_normal.png");
	TMotor::instancia().getHud("MainMenuHUD")->traslateElement("opciones", -0.01f, -0.5f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "salir", "assets/HUD/MainMenu/btn_salir_normal.png");
	TMotor::instancia().getHud("MainMenuHUD")->traslateElement("salir", -0.01f, -0.7f);

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

	//Modificamos los botones para responder a las acciones del usuario
	TMotor::instancia().getActiveHud()->changeTextureElement("local", "assets/HUD/MainMenu/btn_local_normal.png");
	TMotor::instancia().getActiveHud()->changeTextureElement("online", "assets/HUD/MainMenu/btn_online_normal.png");
	TMotor::instancia().getActiveHud()->changeTextureElement("opciones", "assets/HUD/MainMenu/btn_opciones_normal.png");
	TMotor::instancia().getActiveHud()->changeTextureElement("salir", "assets/HUD/MainMenu/btn_salir_normal.png");

	TMotor::instancia().getActiveHud()->scaleElement("local",1.0f,1.0f);
	TMotor::instancia().getActiveHud()->scaleElement("online",1.0f,1.0f);
	TMotor::instancia().getActiveHud()->scaleElement("opciones",1.0f,1.0f);
	TMotor::instancia().getActiveHud()->scaleElement("salir",1.0f,1.0f);

	if (optionMenu == 0){
		TMotor::instancia().getActiveHud()->changeTextureElement("local", "assets/HUD/MainMenu/btn_local_pulsado.png");
		TMotor::instancia().getActiveHud()->scaleElement("local",1.2f,1.2f);
	}else if (optionMenu == 1){
		TMotor::instancia().getActiveHud()->changeTextureElement("online", "assets/HUD/MainMenu/btn_online_pulsado.png");
		TMotor::instancia().getActiveHud()->scaleElement("online",1.2f,1.2f);
	}else if (optionMenu == 2){
		TMotor::instancia().getActiveHud()->changeTextureElement("opciones", "assets/HUD/MainMenu/btn_opciones_pulsado.png");
		TMotor::instancia().getActiveHud()->scaleElement("opciones",1.2f,1.2f);
	}else{
		TMotor::instancia().getActiveHud()->changeTextureElement("salir", "assets/HUD/MainMenu/btn_salir_pulsado.png");
		TMotor::instancia().getActiveHud()->scaleElement("salir",1.2f,1.2f);
	}
}

Escena::tipo_escena EscenaMenu::comprobarInputs() {

	//Si se pulsa ESCAPE se sale directamente del juego
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS){
		if(!pressed){
			return Escena::tipo_escena::SALIR; // Devuelve el estado de las escenas para que salga
		}
	}
	
	//Segun se toque arriba o abajo, el usuario se va moviendo entre los distintos botones del menu
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS){
		if (!pressed){
			if (optionMenu == 3){
				optionMenu = 0;
			}else{
				optionMenu++;
			}
			pressed = true;
		}
	}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS){
		if (!pressed){
			if (optionMenu == 0){
				optionMenu = 3;
			}else{
				optionMenu--;
			}
			pressed = true;
		}
	}else pressed = false;

	//Si se pulsa intro se comprueba en que opcion del menu se encuentra el usuario y se cambia a la escena correspondiente
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS){
		if (optionMenu == 0){
			cout << "SINGLEPLAYER\n";
			ipConexion="offline";
			return Escena::tipo_escena::LOBBY;
		}else if (optionMenu == 1){
			cout << "MULTIPLAYER\n";
			ipConexion="";
			return Escena::tipo_escena::LOBBY;
		}else if (optionMenu == 2){
			cout << "OPCIONES\n";
		}else{
			cout << "SALIR\n";
			return Escena::tipo_escena::SALIR; 
		}
	}


	/*else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_1) == GLFW_PRESS){
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
	*/

	//Se retorna la escena Menu (donde estamos) en caso de que no se haya cambiado de pantalla
	return Escena::tipo_escena::MENU;
}
std::string EscenaMenu::getIpConexion(){
	return ipConexion;
}