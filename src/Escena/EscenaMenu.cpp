#include "EscenaMenu.hpp"

EscenaMenu::EscenaMenu() : Escena(Escena::tipo_escena::MENU) {
	initHud(); // Inicializacion del hud

	/*
		GestorSonido::getInstacia()->setListenerData();
		fuenteMenu = GestorSonido::cargarFuente();
		if(!GestorSonido::getInstacia()->getFuentes().at(fuenteMenu-1)->isPlaying()){
			GestorSonido::getInstacia()->getFuentes().at(fuenteMenu-1)->volume(0.1f);
			GestorSonido::getInstacia()->getFuentes().at(fuenteMenu-1)->play(SOUND_MENU);
		}

	*/

	fuenteMenu = new AlSource();
	fuenteMenu->volume(TMotor::instancia().getGestorSonido()->getVolMusica());
	if (!fuenteMenu->isPlaying())
		fuenteMenu->play(SOUND_MENU);

	fuenteOpcion = new AlSource();
	fuenteOpcion->volume(30.0f * TMotor::instancia().getGestorSonido()->getVolEfectos());
	Client::getInstancia()->BorrarClientes();
	TMotor::instancia().initDebugWindow();
}

void EscenaMenu::initHud() {
	//Se asigna la opcion del menu al primer boton
	optionMenu = 0;
	//Se crea el hud del menu principal
	TMotor::instancia().newHud("MainMenuHUD");

	//Se añaden los distintos elementos del hud y se posicionan correctamente
	//--FONDO
	TMotor::instancia().getActiveHud()->addElement(2.0f, 2.0f, "fondo", "assets/HUD/MainMenu/HUD_Fondo.png");
	//--LOGO AOK--
	TMotor::instancia().getActiveHud()->addElement(0.7f, 0.7f, "aok", "assets/HUD/MainMenu/aok_logo.png");
	TMotor::instancia().getActiveHud()->traslateElement("aok", 0.60f, 0.45f);
	//--BOTONES--
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "local", "assets/HUD/MainMenu/btn_local_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("local", -0.40f, 0.55f);
	TMotor::instancia().getActiveHud()->changeTransparencyElement("local", true, 0.8);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "online", "assets/HUD/MainMenu/btn_online_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("online", -0.10f, 0.20f);
	TMotor::instancia().getActiveHud()->changeTransparencyElement("online", true, 0.8);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "opciones", "assets/HUD/MainMenu/btn_opciones_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("opciones", 0.20f, -0.15f);
	TMotor::instancia().getActiveHud()->changeTransparencyElement("opciones", true, 0.8);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "salir", "assets/HUD/MainMenu/btn_salir_normal.png");
	TMotor::instancia().getActiveHud()->traslateElement("salir", 0.40f, -0.50f);
	TMotor::instancia().getActiveHud()->changeTransparencyElement("salir", true, 0.8);
	//--LOGO SW--
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "sw", "assets/HUD/MainMenu/sw_logo.png");
	TMotor::instancia().getActiveHud()->traslateElement("sw", -0.75f, -0.80f);
	TMotor::instancia().getActiveHud()->scaleElement("sw", 0.20f, 0.20f);


	//Se crea el hud del menu local
	TMotor::instancia().newHud("LocalMenuHUD");
	
	//Se añaden los distintos elementos del hud y se posicionan correctamente
	//--TEXTO ELEGIR MODO--
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "elegirModo", "assets/HUD/MainMenu/elegirModo.png");
	TMotor::instancia().getActiveHud()->scaleElement("elegirModo", 1.0f, 0.80f);
	TMotor::instancia().getActiveHud()->traslateElement("elegirModo", 0.0f, 0.50f);
	//--BOTONES--
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "unJugador", "assets/HUD/MainMenu/btn_unJugador_normal.png");
	TMotor::instancia().getActiveHud()->scaleElement("unJugador", 1.50f, 1.75f);
	TMotor::instancia().getActiveHud()->traslateElement("unJugador", -0.35f, -0.15f);
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "pantallaPartida", "assets/HUD/MainMenu/btn_pantallaPartida_normal.png");
	TMotor::instancia().getActiveHud()->scaleElement("pantallaPartida", 1.50f, 1.75f);
	TMotor::instancia().getActiveHud()->traslateElement("pantallaPartida", 0.35f, -0.15f);
	//--VOLVER AL MENU--
	TMotor::instancia().getActiveHud()->addElement(0.4f, 0.15f, "volverAlMenu", "assets/HUD/MainMenu/volverAlMenu.png");
	TMotor::instancia().getActiveHud()->scaleElement("volverAlMenu", 2.0f, 2.0f);
	TMotor::instancia().getActiveHud()->traslateElement("volverAlMenu", 0.0f, -0.65f);
	//--LOGO AOK--
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "aok", "assets/HUD/MainMenu/aok_logo.png");
	TMotor::instancia().getActiveHud()->traslateElement("aok", 0.82f, 0.80f);
	TMotor::instancia().getActiveHud()->scaleElement("aok", 0.30f, 0.30f);


	//Activamos el hud del menu principal, para que se muestre primero
	TMotor::instancia().setActiveHud("MainMenuHUD");
}

EscenaMenu::~EscenaMenu() {
	cout << "Destructor ESCENA MENU. Entro.";
	limpiar();
	delete fuenteMenu;
	delete fuenteOpcion;
	cout << "Salgo.\n";
}

void EscenaMenu::init() {}

void EscenaMenu::dibujar() {
	TMotor::instancia().draw(getTipoEscena());
}

void EscenaMenu::limpiar() {}

void EscenaMenu::update() {}

std::string EscenaMenu::getIpConexion() { return ipConexion; }

Escena::tipo_escena EscenaMenu::comprobarInputs() {
	//Si se pulsa ESCAPE se sale directamente del juego - para poder salir de todos los sitios, despues se quitara
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (!pressed) {
			return Escena::tipo_escena::SALIR;
		}
	}else if (strcmp(TMotor::instancia().getActiveHud()->getName(), "MainMenuHUD") == 0) { //Inputs para el menu principal
		//Segun se toque arriba o abajo, el usuario se va moviendo entre los distintos botones del menu
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (!pressed) {
				fuenteOpcion->play(SOUND_OPCION); //Reproducimos sonido opcion;
				if (optionMenu == 3) {
					optionMenu = 0;
				}
				else {
					optionMenu++;
				}
				pressed = true;
			}
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS) {
			if (!pressed) {
				fuenteOpcion->play(SOUND_OPCION); //Reproducimos sonido opcion;
				if (optionMenu == 0) {
					optionMenu = 3;
				}
				else {
					optionMenu--;
				}
				pressed = true;
			}
			//Si se pulsa intro se comprueba en que opcion del menu se encuentra el usuario y se cambia a la escena correspondiente
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS) {
			fuenteOpcion->play(SOUND_OPCION); //Reproducimos sonido opcion;
			if (optionMenu == 0) {
				cout << "LOCAL\n";
				TMotor::instancia().setActiveHud("LocalMenuHUD");
				pressed = true;
			}
			else if (optionMenu == 1) {
				cout << "MULTIPLAYER\n";
				ipConexion = "";

				return Escena::tipo_escena::LOBBY;
			}
			else if (optionMenu == 2) {
				cout << "OPCIONES\n";
				return Escena::tipo_escena::OPCIONES;
			}
			else {
				cout << "SALIR\n";

				return Escena::tipo_escena::SALIR;
			}
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_M) == GLFW_PRESS) {
			if (!pressed) {

				if (fuenteMenu->isPlaying()) {
					cout << "Musica desactivada.\n";
					fuenteMenu->stop(SOUND_MENU);
				}
				else {
					cout << "Musica activada.\n";
					fuenteMenu->play(SOUND_MENU);
				}


				pressed = true;
			}
		}
		else pressed = false;
	}

	//Inputs para el menu de juego local
	if (strcmp(TMotor::instancia().getActiveHud()->getName(), "LocalMenuHUD") == 0) {
		//Si se pulsa ESCAPE se sale al menu principal
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
			if (!pressed) {
				TMotor::instancia().setActiveHud("MainMenuHUD");
				optionMenu = 0;
			}
		}

		//Segun se toque derecha o izquierda, el usuario se va moviendo entre los dos botones del menu local
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
			if (!pressed) {
				optionMenu = 1;
				pressed = true;
			}
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS) {
			if (!pressed) {
				optionMenu = 0;
				pressed = true;
			}
			//Si se pulsa intro se comprueba en que opcion del menu se encuentra el usuario y se cambia a la escena correspondiente
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS) {
			if (optionMenu == 0) {
				if (!pressed) {
					cout << "UN JUGADOR\n";
					ipConexion = "offline";
					return Escena::tipo_escena::LOBBY;
				}
			}
			else if (optionMenu == 1) {
				cout << "PANTALLA PARTIDA\n";
			}
		}

		else pressed = false;

	}

		//MODIFICAMOS LA INTERFAZ EN FUNCION DE LAS ACCIONES DEL USUARIO

	//Si esta activo el menu principal, se tienen en cuenta los siguientes inputs
	if (strcmp(TMotor::instancia().getActiveHud()->getName(), "MainMenuHUD") == 0) {
		TMotor::instancia().getActiveHud()->changeTextureElement("local", "assets/HUD/MainMenu/btn_local_normal.png");
		TMotor::instancia().getActiveHud()->changeTextureElement("online", "assets/HUD/MainMenu/btn_online_normal.png");
		TMotor::instancia().getActiveHud()->changeTextureElement("opciones", "assets/HUD/MainMenu/btn_opciones_normal.png");
		TMotor::instancia().getActiveHud()->changeTextureElement("salir", "assets/HUD/MainMenu/btn_salir_normal.png");

		TMotor::instancia().getActiveHud()->scaleElement("local", 1.0f, 1.0f);
		TMotor::instancia().getActiveHud()->scaleElement("online", 1.0f, 1.0f);
		TMotor::instancia().getActiveHud()->scaleElement("opciones", 1.0f, 1.0f);
		TMotor::instancia().getActiveHud()->scaleElement("salir", 1.0f, 1.0f);

		if (optionMenu == 0) {
			TMotor::instancia().getActiveHud()->changeTextureElement("local", "assets/HUD/MainMenu/btn_local_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("local", 1.2f, 1.2f);
		}
		else if (optionMenu == 1) {
			TMotor::instancia().getActiveHud()->changeTextureElement("online", "assets/HUD/MainMenu/btn_online_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("online", 1.2f, 1.2f);
		}
		else if (optionMenu == 2) {
			TMotor::instancia().getActiveHud()->changeTextureElement("opciones", "assets/HUD/MainMenu/btn_opciones_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("opciones", 1.2f, 1.2f);
		}
		else {
			TMotor::instancia().getActiveHud()->changeTextureElement("salir", "assets/HUD/MainMenu/btn_salir_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("salir", 1.2f, 1.2f);
		}
	}

	//Si esta activa el menu de juego local, se realizan los siguientes cambios
	if (strcmp(TMotor::instancia().getActiveHud()->getName(), "LocalMenuHUD") == 0) {
		TMotor::instancia().getActiveHud()->changeTextureElement("unJugador", "assets/HUD/MainMenu/btn_unJugador_normal.png");
		TMotor::instancia().getActiveHud()->changeTextureElement("pantallaPartida", "assets/HUD/MainMenu/btn_pantallaPartida_normal.png");

		TMotor::instancia().getActiveHud()->scaleElement("unJugador", 1.50f, 1.75f);
		TMotor::instancia().getActiveHud()->scaleElement("pantallaPartida", 1.50f, 1.75f);

		if (optionMenu == 0) {
			TMotor::instancia().getActiveHud()->changeTextureElement("unJugador", "assets/HUD/MainMenu/btn_unJugador_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("unJugador", 1.75f, 2.0f);
		}
		else {
			TMotor::instancia().getActiveHud()->changeTextureElement("pantallaPartida", "assets/HUD/MainMenu/btn_pantallaPartida_pulsado.png");
			TMotor::instancia().getActiveHud()->scaleElement("pantallaPartida", 1.75f, 2.0f);
		}
	}
	
	//Se retorna la escena Menu (donde estamos) en caso de que no se haya cambiado de pantalla
	return Escena::tipo_escena::MENU;

}



