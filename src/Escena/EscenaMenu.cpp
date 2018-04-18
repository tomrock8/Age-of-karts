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
	//--LOGO AOK--
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "aok", "assets/HUD/MainMenu/aok_logo.png");
	TMotor::instancia().getActiveHud()->traslateElement("aok", 0.0f, 0.45f);
	//--BOTONES--
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "local", "assets/HUD/MainMenu/btn_local_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("local", -0.01f, -0.15f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "online", "assets/HUD/MainMenu/btn_online_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("online", -0.01f, -0.35f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "opciones", "assets/HUD/MainMenu/btn_opciones_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("opciones", -0.01f, -0.55f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "salir", "assets/HUD/MainMenu/btn_salir_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("salir", -0.01f, -0.75f);
	//--LOGO SW--
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "sw", "assets/HUD/MainMenu/sw_logo.png");
	TMotor::instancia().getActiveHud()->traslateElement("sw", 0.85f, -0.80f);
	TMotor::instancia().getActiveHud()->scaleElement("sw",0.20f,0.20f);

	//Se crea el hud del menu local
	TMotor::instancia().newHud("LocalMenuHUD");

	//Se añaden los distintos elementos del hud y se posicionan correctamente
	//--TEXTO ELEGIR MODO--
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "elegirModo", "assets/HUD/MainMenu/elegirModo.png");
	TMotor::instancia().getActiveHud()->scaleElement("elegirModo",1.0f,0.80f);
	TMotor::instancia().getActiveHud()->traslateElement("elegirModo", 0.0f, 0.50f);
	//--BOTONES--
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "unJugador", "assets/HUD/MainMenu/btn_unJugador_normal.png");
	TMotor::instancia().getActiveHud()->scaleElement("unJugador",1.50f, 1.75f);
	TMotor::instancia().getActiveHud()->traslateElement("unJugador", -0.35f, -0.15f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "pantallaPartida", "assets/HUD/MainMenu/btn_pantallaPartida_normal.png");
	TMotor::instancia().getActiveHud()->scaleElement("pantallaPartida",1.50f, 1.75f);
	TMotor::instancia().getActiveHud()->traslateElement("pantallaPartida", 0.35f, -0.15f);
	//--VOLVER AL MENU--
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "volverAlMenu", "assets/HUD/MainMenu/volverAlMenu.png");
	TMotor::instancia().getActiveHud()->scaleElement("volverAlMenu",2.0f, 2.0f);
	TMotor::instancia().getActiveHud()->traslateElement("volverAlMenu", 0.0f, -0.65f);
	//--LOGO AOK--
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "aok", "assets/HUD/MainMenu/aok_logo.png");
	TMotor::instancia().getActiveHud()->traslateElement("aok", 0.82f, 0.80f);
	TMotor::instancia().getActiveHud()->scaleElement("aok",0.30f,0.30f);


	//Activamos el hud del menu principal, para que se muestre primero
	TMotor::instancia().setActiveHud("MainMenuHUD");

/*
	GestorSonido::getInstacia()->setListenerData();
	fuenteMenu = GestorSonido::cargarFuente();
	if(!GestorSonido::getInstacia()->getFuentes().at(fuenteMenu-1)->isPlaying()){
		GestorSonido::getInstacia()->getFuentes().at(fuenteMenu-1)->volume(0.1f);
		GestorSonido::getInstacia()->getFuentes().at(fuenteMenu-1)->play(SOUND_MENU);
	}
	fuenteOpcion = GestorSonido::cargarFuente();
*/
	/*
	GestorSonido::getInstacia()->setListenerData();
	fuenteMenu = new AlSource();
	fuenteMenu->volume(0.1f);
	if(fuenteMenu->isPlaying());
		fuenteMenu->play(SOUND_MENU);
	fuenteOpcion->volume(1.0f);
	fuenteOpcion = new AlSource();*/
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

	//Si se pulsa ESCAPE se sale directamente del juego - para poder salir de todos los sitios, despues se quitara
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS){
		if(!pressed){
			//delete fuenteMenu;
			//delete fuenteOpcion;
			return Escena::tipo_escena::SALIR; // Devuelve el estado de las escenas para que salga
		}
	}

	//Inputs para el menu principal
	if (strcmp(TMotor::instancia().getActiveHud()->getName(), "MainMenuHUD") == 0){
		
		//Segun se toque arriba o abajo, el usuario se va moviendo entre los distintos botones del menu
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS){
			if (!pressed){
				//fuenteOpcion->play(SOUND_OPCION); //Reproducimos sonido opcion;
				if (optionMenu == 3){
					optionMenu = 0;
				}else{
					optionMenu++;
				}
				pressed = true;
			}
		}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS){
			if (!pressed){
				//fuenteOpcion->play(SOUND_OPCION); //Reproducimos sonido opcion;
				if (optionMenu == 0){
					optionMenu = 3;
				}else{
					optionMenu--;
				}
				pressed = true;
			}
		//Si se pulsa intro se comprueba en que opcion del menu se encuentra el usuario y se cambia a la escena correspondiente
		}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS){
			//fuenteOpcion->play(SOUND_OPCION); //Reproducimos sonido opcion;
			if (optionMenu == 0){
				cout << "LOCAL\n";
				TMotor::instancia().setActiveHud("LocalMenuHUD");
				pressed = true;
			}else if (optionMenu == 1){
				cout << "MULTIPLAYER\n";
				ipConexion="";
					
				//delete fuenteMenu;
				//delete fuenteOpcion;
				return Escena::tipo_escena::LOBBY;
			}else if (optionMenu == 2){
				cout << "OPCIONES\n";
			}else{
				cout << "SALIR\n";
					
				//delete fuenteMenu;
				//delete fuenteOpcion;
				return Escena::tipo_escena::SALIR; 
			}
		}else pressed = false;
	}

	//Inputs para el menu de juego local
	if (strcmp(TMotor::instancia().getActiveHud()->getName(), "LocalMenuHUD") == 0){
		//Si se pulsa ESCAPE se sale al menu principal
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_BACKSPACE) == GLFW_PRESS){
			if(!pressed){
				TMotor::instancia().setActiveHud("MainMenuHUD"); 
				optionMenu = 0;
			}
		}

		//Segun se toque derecha o izquierda, el usuario se va moviendo entre los dos botones del menu local
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS){
			if (!pressed){
				optionMenu = 1;
				pressed = true;
			}
		}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS){
			if (!pressed){
				optionMenu = 0;
				pressed = true;
			}
		//Si se pulsa intro se comprueba en que opcion del menu se encuentra el usuario y se cambia a la escena correspondiente
		}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS){
			if (optionMenu == 0){
				if (!pressed){
				cout << "UN JUGADOR\n";
				ipConexion="offline";
				return Escena::tipo_escena::LOBBY;
				}
			}else if (optionMenu == 1){
				cout << "PANTALLA PARTIDA\n";
			}
		}else pressed = false;
		
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

	//MODIFICAMOS LA INTERFAZ EN FUNCION DE LAS ACCIONES DEL USUARIO

	//Si esta activo el menu principal, se tienen en cuenta los siguientes inputs
	if (strcmp(TMotor::instancia().getActiveHud()->getName(), "MainMenuHUD") == 0){
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
	
	//Si esta activa el menu de juego local, se realizan los siguientes cambios
	if (strcmp(TMotor::instancia().getActiveHud()->getName(), "LocalMenuHUD") == 0){
		TMotor::instancia().getActiveHud()->changeTextureElement("unJugador", "assets/HUD/MainMenu/btn_unJugador_normal.png");
		TMotor::instancia().getActiveHud()->changeTextureElement("pantallaPartida", "assets/HUD/MainMenu/btn_pantallaPartida_normal.png");

		TMotor::instancia().getActiveHud()->scaleElement("unJugador",1.50f, 1.75f);
		TMotor::instancia().getActiveHud()->scaleElement("pantallaPartida",1.50f, 1.75f);

		if (optionMenu == 0){
			TMotor::instancia().getActiveHud()->changeTextureElement("unJugador", "assets/HUD/MainMenu/btn_unJugador_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("unJugador",1.75f,2.0f);
		}else{
			TMotor::instancia().getActiveHud()->changeTextureElement("pantallaPartida", "assets/HUD/MainMenu/btn_pantallaPartida_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("pantallaPartida",1.75f,2.0f);
		}
	}

	//Se retorna la escena Menu (donde estamos) en caso de que no se haya cambiado de pantalla
	return Escena::tipo_escena::MENU;
}
std::string EscenaMenu::getIpConexion(){
	return ipConexion;
}