#include "EscenaJuego.hpp"
#include "CorredorJugador.hpp"
#include "CorredorIA.hpp"
#include "CorredorRed.hpp"
#include "Server.hpp"

EscenaJuego::EscenaJuego(tipo_escena tipo) : Escena(tipo) {
	end = false;
	ipConexion = "";
	TMotor::instancia().newHud("OnGameHUD");
	TMotor::instancia().getActiveHud()->addElement(0.2f, 0.2f, "puesto", "assets/HUD/juego/puesto_6.png");
	TMotor::instancia().getActiveHud()->traslateElement("puesto", -0.85f, 0.85f);
	TMotor::instancia().getActiveHud()->addElement(0.35f, 0.35f, "vueltas", "assets/HUD/juego/lap_1_3.png");
	TMotor::instancia().getActiveHud()->traslateElement("vueltas", -0.83f, 0.68f);

	tipoEscena=tipo;
	puesto = 6;
	vueltas = 1;
	vueltas_aux=1;
	waitingPlayers = false;
	init();
}

EscenaJuego::EscenaJuego(tipo_escena tipo, std::string ipConexion) : Escena(tipo) {
	end = false;
	TMotor::instancia().newHud("OnGameHUD");
	TMotor::instancia().getActiveHud()->addElement(0.2f, 0.2f, "puesto", "assets/HUD/juego/puesto_6.png");
	TMotor::instancia().getActiveHud()->traslateElement("puesto", -0.85f, 0.85f);
	TMotor::instancia().getActiveHud()->addElement(0.35f, 0.35f, "vueltas", "assets/HUD/juego/lap_1_3.png");
	TMotor::instancia().getActiveHud()->traslateElement("vueltas", -0.83f, 0.68f);

	puesto = 6;
	vueltas = 1;
	vueltas_aux=1;
	tipoEscena=tipo;
	this->ipConexion = ipConexion;
	waitingPlayers = false;
	init();
}

EscenaJuego::~EscenaJuego() {
	//----------------------------------//
	//-----------DESTRUCTORES-----------//
	//----------------------------------//
	limpiar();
	delete MotorFisicas::getInstancia();
	delete GestorJugadores::getInstancia();
	delete Pista::getInstancia();
}

void EscenaJuego::init() {

	// LUCES PUNTUALES
	obj3D * luzPuntual_0 = TMotor::instancia().newLightNode("light_0", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, true, "escena_raiz");
	luzPuntual_0->translate(glm::vec3(100.0f, 700.0f, 0.0f));

	obj3D * luzPuntual_1 = TMotor::instancia().newLightNode("light_1", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzPuntual_1->translate(glm::vec3(0.0f, 1500.0f, 0.0f));

	obj3D * luzPuntual_2 = TMotor::instancia().newLightNode("light_2", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzPuntual_2->translate(glm::vec3(-150.0f, 12.0f, -300.0f));

	obj3D * luzPuntual_3 = TMotor::instancia().newLightNode("light_3", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzPuntual_3->translate(glm::vec3(0.0f, 12.0f, 300.0f));

	//LUCES DIRIGIDAS
	/*
	obj3D * luzDirigida_0 = TMotor::instancia().newLightNode("light_0", glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), 0.001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzDirigida_0->translate(glm::vec3(1.0f, 12.0f, 0.0f));

	obj3D * luzDirigida_1 = TMotor::instancia().newLightNode("light_1", glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), 0.001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzDirigida_1->translate(glm::vec3(0.0f, 12.0f, 0.0f));

	obj3D * luzDirigida_2 = TMotor::instancia().newLightNode("light_2", glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), 0.001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzDirigida_2->translate(glm::vec3(0.0f, 12.0f, 300.0f));

	obj3D * luzDirigida_3 = TMotor::instancia().newLightNode("light_3", glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), 0.001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzDirigida_3->translate(glm::vec3(0.0f, 12.0f, 300.0f));*/

	//ARGUMENTOS MAIN
	debug = false;
	fin_carrera = false;
	t = Timer::getInstancia();

	GestorIDs::instancia().restartID();

	
	controlPlayer = 0;

	// Gravedad
	gravedad = -10.0f;
	MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0.0, gravedad, 0.0));

	//----------------------------
	//	Debug Bullet
	//----------------------------

	debugDraw = new DebugDraw(TMotor::instancia().getVentana());
	debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	MotorFisicas::getInstancia()->getMundo()->setDebugDrawer(debugDraw);

	//-----------------------------
	//	ESCENARIO MAPA
	//-----------------------------

	Pista::getInstancia()->setMapa();

	//-----------------------------
	//	JUGADORES
	//-----------------------------
	//Posicion del nodo y el bloque de colisiones centralizado:

	btVector3 pos2[6];
	pos2[0].setX(Pista::getInstancia()->getParrilla().at(0).x);
	pos2[0].setY(Pista::getInstancia()->getParrilla().at(0).y);
	pos2[0].setZ(Pista::getInstancia()->getParrilla().at(0).z);
	pos2[1].setX(Pista::getInstancia()->getParrilla().at(1).x);
	pos2[1].setY(Pista::getInstancia()->getParrilla().at(1).y);
	pos2[1].setZ(Pista::getInstancia()->getParrilla().at(1).z);
	pos2[2].setX(Pista::getInstancia()->getParrilla().at(2).x);
	pos2[2].setY(Pista::getInstancia()->getParrilla().at(2).y);
	pos2[2].setZ(Pista::getInstancia()->getParrilla().at(2).z);
	pos2[3].setX(Pista::getInstancia()->getParrilla().at(3).x);
	pos2[3].setY(Pista::getInstancia()->getParrilla().at(3).y);
	pos2[3].setZ(Pista::getInstancia()->getParrilla().at(3).z);
	pos2[4].setX(Pista::getInstancia()->getParrilla().at(4).x);
	pos2[4].setY(Pista::getInstancia()->getParrilla().at(4).y);
	pos2[4].setZ(Pista::getInstancia()->getParrilla().at(4).z);
	pos2[5].setX(Pista::getInstancia()->getParrilla().at(5).x);
	pos2[5].setY(Pista::getInstancia()->getParrilla().at(5).y);
	pos2[5].setZ(Pista::getInstancia()->getParrilla().at(5).z);

	Server::getInstancia()->RaceStart();
	numPantallas = 1;

	gc = new GestorCarrera();

	//-----------------------------
	//	CAMARA
	//-----------------------------
	camera = new cameraThird("camara_jugador3apersona", "escena_raiz");

	//-----------------------------
	//	GESTOR COLISIONES
	//-----------------------------
	colisiones = new GestorColisiones();

	//-----------------------------
	//	TIME
	//-----------------------------
	lastFPS = -1;
	TimeStamp = glfwGetTime();
	DeltaTime = 0;
	tiempoRefresco = glfwGetTime();
	t->restartTimer();

	// -----------------------
	//	IMGUI
	// -----------------------
	debug_Jugador = false;
	muestraDebug = true;
	show_another_window=false;
	muestraDebugIA=false;
	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",30.0f);
	if (GestorJugadores::getInstancia()->getJugadores().size()>0)
		Server::getInstancia()->setStarted(false);
}

void EscenaJuego::dibujar() {

	//------- RENDER ----------

	//Limpiamos el dibujado anterior asignando un color de fondo
	TMotor::instancia().clean(0.16f, 0.533f, 0.698f, 0.0f);
	for (int i = 0; i < numPantallas; i++){
		//Establecemos la zona de renderizado
		if (numPantallas > 1){
			if (i == 0){
				TMotor::instancia().setViewport(0, TMotor::instancia().getHeight()/2, TMotor::instancia().getWidth()/2, TMotor::instancia().getHeight()/2); //Arriba-izquierda
			}else if (i == 1){
				TMotor::instancia().setViewport(TMotor::instancia().getWidth()/2, TMotor::instancia().getHeight()/2, TMotor::instancia().getWidth()/2, TMotor::instancia().getHeight()/2); //Arriba-derecha
			}else if (i == 2){
				TMotor::instancia().setViewport(0, 0, TMotor::instancia().getWidth()/2, TMotor::instancia().getHeight()/2); //Abajo-izquierda				
			}else{
				TMotor::instancia().setViewport(TMotor::instancia().getWidth()/2, 0, TMotor::instancia().getWidth()/2, TMotor::instancia().getHeight()/2); //Abajo-derecha				
			}
		}else{
			TMotor::instancia().setViewport(0, 0, TMotor::instancia().getWidth(), TMotor::instancia().getHeight()); //Pantalla completa
		}
		//Especificamos la camara activa
		TMotor::instancia().setActiveCamera(TMotor::instancia().getCameraByIndex(i));
		//Dibujamos el skybox
		TMotor::instancia().drawSkybox();
		//Dibujamos los objetos 3D creados en la escena
		TMotor::instancia().draw();
		//Dibujamos el debug de bullet
		TMotor::instancia().drawDebugBullet();
		//Dibujamos las particulas
		TMotor::instancia().drawParticles();
		//Dibujamos los billboards
		TMotor::instancia().drawBillboards();
		//Dibujamos el menu 
		TMotor::instancia().drawHudMenus();
		//Dibujamos IMGUI
		TMotor::instancia().drawIMGUI();
	}

	//if (tipoEscena != Escena::tipo_escena::ONLINE) 
		//renderDebug();
}

void EscenaJuego::renderDebug() {
	
	// ------------------------------
	// -------- IMGUI ---------------
	// ------------------------------
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	Corredor *jugador = GestorJugadores::getInstancia()->getJugadores().at(controlPlayer);
	
	// Mostrar ventanas

	ImGui_ImplGlfwGL3_NewFrame();
	if (show_another_window){
		//ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImGui::StyleColorsLight(); 
		ImGuiIO& io = ImGui::GetIO();
/*
		if (io.Fonts->Fonts.Size>1){
			ImGui::PushFont(io.Fonts->Fonts[1]);
		}else{
			ImGui::PushFont(io.Fonts->Fonts[0]);

		}*/
	}else{

		ImGui::StyleColorsClassic();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	}

		if (muestraDebug){		
			
			ImGui::Text("Renderizado: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			debugPlot(controlPlayer,ImGui::GetIO().Framerate,"Frames");
		
			ImGui::Text("Debug del Juego!");
			ImGui::Text("Pulsa 9 para activar - 0 desactivar");
			ImGui::Text("Jugadores: %i", GestorJugadores::getInstancia()->getNumJugadores());
			ImGui::Text("Elementos de fisicas: %i", MotorFisicas::getInstancia()->getMundo()->getNumCollisionObjects());

			if (ImGui::SliderFloat("Gravedad", &gravedad, -100.0f, 0.0f, "%.2f", 1.0f))
				MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0.0, gravedad, 0.0));

			if (ImGui::Checkbox("Debug Fisicas", &debug))
				TMotor::instancia().setDebugBullet(debug);

			ImGui::Checkbox("Debug Jugador", &debug_Jugador);

			ImGui::Checkbox("Debug IA", &muestraDebugIA);
			if (debug_Jugador) {
				
				ImGui::Begin("Datos del Corredor Jugador", &debug_Jugador);
				ImGui::Text(jugador->toString().c_str());
				debugPlot(controlPlayer,jugador->getVehiculo()->getCurrentSpeedKmHour(),"Velocidad");
				static float fuerza, velocidadMedia, velocidadMaximaTurbo, velocidadMaxima, masa, indiceGiroAlto, indiceGiroBajo, velocidadLimiteGiro;
				jugador->getParametrosDebug(&fuerza, &velocidadMedia, &velocidadMaximaTurbo, &velocidadMaxima, &masa, &indiceGiroAlto, &indiceGiroBajo, &velocidadLimiteGiro);

				ImGui::SliderFloat("fuerza", &fuerza, 1000.0f, 10000.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("velocidadMedia", &velocidadMedia, 100.0f, 800.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("velocidadMaximaTurbo", &velocidadMaximaTurbo, 100.0f, 800.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("velocidadMaxima", &velocidadMaxima, 100.0f, 800.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("Masa", &masa, 0.0f, 8000.0f, "%.1f", 10.0f);
				ImGui::SliderFloat("VelocidadLimiteGiro", &velocidadLimiteGiro, 0.0f, 8000.0f, "%.1f", 100.0f);
				ImGui::SliderFloat("indiceGiroAlto", &indiceGiroAlto, 0.0f, 1.0f, "%.4f", 0.01f);
				ImGui::SliderFloat("indiceGiroBajo", &indiceGiroBajo, 0.0f, 1.0f, "%.4f", 0.01f);
				jugador->setParametrosDebug(fuerza, velocidadMedia, velocidadMaximaTurbo, velocidadMaxima, masa, indiceGiroAlto, indiceGiroBajo, velocidadLimiteGiro);

				static float *posicion = new float[3];
				float *resetOri = new float[3];
				resetOri[0] = jugador->getNodo()->getRotation().z;
				resetOri[1] = jugador->getNodo()->getRotation().y;
				resetOri[2] = jugador->getNodo()->getRotation().x;


				ImGui::SliderFloat3("Posicion", posicion, -100, 100, "%.2f", 1.0f);
				ImGui::SameLine();
				if (ImGui::Button("Set position"))
					jugador->setPosicion(posicion, resetOri);// Hay que pasarle solo la posicion al jugador, no al nodo

				if (ImGui::CollapsingHeader("Ruedas")) {
					static float suspensionStiffness, DampingCompression, DampingRelaxation, frictionSlip, rollInfluence, suspForce, suspTravelCm;
					jugador->getParametrosRuedasDebug(&suspensionStiffness, &DampingCompression, &DampingRelaxation, &frictionSlip, &rollInfluence, &suspForce, &suspTravelCm);
					ImGui::SliderFloat("suspensionStiffness", &suspensionStiffness, 0.0f, 50.0f);
					ImGui::SliderFloat("frictionSlip", &frictionSlip, 1000.0f, 50000.0f, "%.1f", 100.0f);
					ImGui::SliderFloat("rollInfluence", &rollInfluence, 0.0f, 0.1f, "%.3f", 0.001f);
					ImGui::SliderFloat("suspForce", &suspForce, 1000.0f, 50000.0f, "%.1f", 100.0f);
					ImGui::SliderFloat("suspTravelCm", &suspTravelCm, 1000.0f, 50000.0f, "%.1f", 100.0f);
					jugador->setParametrosRuedasDebug(suspensionStiffness, DampingCompression, DampingRelaxation, frictionSlip, rollInfluence, suspForce, suspTravelCm);
					ImGui::Text("DampingCompression: %.3f", DampingCompression);
					ImGui::SameLine();
					ImGui::Text("DampingRelaxation: %.3f", DampingRelaxation);
				}
				
				if (ImGui::Button("Cerrar"))
					debug_Jugador = false;
				ImGui::End();
			}
		}
	
	
	if (vueltas_aux!=vueltas){
		show_another_window=true;
		muestra_tiempo=t->getTimer();
	}
	if (show_another_window){
			int display_w,display_h;
			glfwGetFramebufferSize( TMotor::instancia().getVentana() , &display_w , &display_h );
			ImGui::SetNextWindowPos(ImVec2((display_w-300)/2, (display_h-500)/2));
			if (vueltas<=3){
				ImGui::SetNextWindowSize( ImVec2( (float)302 , (float)80 ) );
			}else{
				ImGui::SetNextWindowSize( ImVec2( (float)290 , (float)40 ) );
			}
			ImGui::SetNextWindowBgAlpha(0.6f); 
            ImGui::Begin("Another Window", &show_another_window,  ImGuiWindowFlags_NoResize 
			| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
			| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal);
			if (vueltas<=3){
				ImGui::Text("Tiempo vuelta: ");
				ImGui::Text(to_string(jugador->getTiempoVuelta()).c_str());
			}else{
				ImGui::Text("Has quedado: ");
				TMotor::instancia().getActiveHud()->traslateElement("puesto", 0.0f, 0.3f);
				muestra_tiempo=t->getTimer();
				//ImGui::Text(to_string(jugador->getPosicionCarrera()).c_str());
			}
			vueltas_aux=vueltas;
			if (t->getTimer()-muestra_tiempo>=4){
				show_another_window = false;
			}
           
            ImGui::End();
			
        }
		ImGui::PopFont();
}

void EscenaJuego::limpiar() {
	muestraDebug = false;
	TMotor::instancia().closeDebugWindow();
}

void EscenaJuego::update() {
	Server::getInstancia()->ReceivePackets();
	Pista *pistaca = Pista::getInstancia();
	std::vector<Item *> items = pistaca->getItems();


	DeltaTime = glfwGetTime() * 1000 - TimeStamp;
	TimeStamp = glfwGetTime() * 1000;
	UpdatePhysics(DeltaTime);

	for (int i = 0; i < pistaca->getTamCajas(); i++) {
		pistaca->getArrayCaja()[i]->comprobarRespawn(); // TODO: MOVER AL UPDATE DE PISTACA
	}

	for (int i = 0; i < items.size(); i++)
	{
		if (items.at(i)->getLanzado()) {

			if (items.at(i)->update()) {

				if (strcmp("Escudo", items.at(i)->getNombre()) == 0) {
					GestorJugadores::getInstancia()->getJugadores().at(items.at(i)->getIDPadre())->setProteccion(false);
				}

				if (strcmp("HabilidadPirata", items.at(i)->getNombre()) == 0
					|| strcmp("HabilidadVikingo", items.at(i)->getNombre()) == 0
					|| strcmp("HabilidadGladiador", items.at(i)->getNombre()) == 0
					|| strcmp("HabilidadChino", items.at(i)->getNombre()) == 0)
				{
					GestorJugadores::getInstancia()->getJugadores().at(items.at(i)->getIDPadre())->setHabilidad(false);

				}

				delete items.at(i);
				items.erase(items.begin() + i);
				pistaca->setItems(items);
				break;
			}
			else {
				items.at(i)->updateHijos();
			}
		}
	}

	if(!Server::getInstancia()->getPlayersLoaded()){
		t->restartTimer();
	}

	if (t->getTimer() <= 4 && t->getTimer() >= 0) {
		if(t->getTimer() == 0){	
			if(!waitingPlayers){
				TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "cuentaAtras", "assets/HUD/juego/CuentaAtrasNotLoad.png");
				TMotor::instancia().getActiveHud()->traslateElement("cuentaAtras", 0.0f, 0.80f);
				waitingPlayers = true;
			}else{
				TMotor::instancia().getActiveHud()->changeTextureElement("cuentaAtras", "assets/HUD/juego/CuentaAtrasNotLoad.png");
			}
		} else if (t->getTimer() == 1) {
			if(!waitingPlayers){
				TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "cuentaAtras", "assets/HUD/juego/CuentaAtras3.png");
				TMotor::instancia().getActiveHud()->traslateElement("cuentaAtras", 0.0f, 0.35f);
			}else{
				TMotor::instancia().getActiveHud()->changeTextureElement("cuentaAtras", "assets/HUD/juego/CuentaAtras3.png");
			}
		}
		else if (t->getTimer() == 2) {
			TMotor::instancia().getActiveHud()->changeTextureElement("cuentaAtras", "assets/HUD/juego/CuentaAtras2.png");
		}
		else if (t->getTimer() == 3) {
			TMotor::instancia().getActiveHud()->changeTextureElement("cuentaAtras", "assets/HUD/juego/CuentaAtras1.png");
		}
		else if (t->getTimer() == 4) {
			TMotor::instancia().getActiveHud()->changeTextureElement("cuentaAtras", "assets/HUD/juego/CuentaAtrasGo.png");
		}
	}

	if (t->getTimer() >= 5) {
		TMotor::instancia().getActiveHud()->deleteElement("cuentaAtras");
		for (int i = 0; i < GestorJugadores::getInstancia()->getNumJugadores(); i++) {
			GestorJugadores::getInstancia()->getJugadores().at(i)->getEstados()->setEstadoCarrera(CARRERA);
		}
		if (t->getTimer() >= 5 && t->getTimer() < 6)
		colisiones->IniciarTimer();
	}

	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->actualizarItem();
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
		updateHUD();
		if (GestorJugadores::getInstancia()->getNumJugadores() > 0) {
			for (int i = 0; i < GestorJugadores::getInstancia()->getNumJugadores(); i++) {
				GestorJugadores::getInstancia()->getJugadores().at(i)->update();
			}
		}
	}
	else {
		if (GestorJugadores::getInstancia()->getNumJugadores() > 0){
			GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->actualizarItem();
			updateHUD();
			colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
											//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED
			Server::getInstancia()->aumentarTimestamp();
			for (int i = 0; i < GestorJugadores::getInstancia()->getNumJugadores(); i++) {
				GestorJugadores::getInstancia()->getJugadores().at(i)->update();
			}

			if (GestorJugadores::getInstancia()->getNumJugadores() != 0) {
				float tiempoActual = glfwGetTime();
				float timediff_sec = tiempoActual - tiempoRefresco;

				//if (timediff_sec >= 0) {
					Server::getInstancia()->refreshServer();
					tiempoRefresco = glfwGetTime();
				//}

			}
		}

		if (GestorJugadores::getInstancia()->getNumJugadores() > 0)
			if (gc->update())
				fin_carrera = true;
	}



	camera->setPosition(GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getNodo()->getPosition(), GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getNodo()->getRotation(), GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getVectorDireccion());
	camera->lookAt(GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getNodo()->getPosition());
}

Escena::tipo_escena EscenaJuego::comprobarInputs() {
	btVector3 pos(-10, 0, 310);
	int i = 0;

	//------- ENTRADA TECLADO ----------
	if (fin_carrera == true && glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_F) == GLFW_PRESS) {
		if (tipoEscena == Escena::tipo_escena::ONLINE && controlPlayer == 0) {
			for (int i = 0; i < GestorJugadores::getInstancia()->getJugadores().size(); i++) {
				GestorJugadores::getInstancia()->getJugadores().at(i)->setVueltas(1);
			}
			//client->FinalizarCarrera();
		}
		else if (tipoEscena != Escena::tipo_escena::ONLINE) {
			return Escena::tipo_escena::SALIR;
		}
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_C) == GLFW_PRESS) {
		if (!cambioCamara) {
			tipoCamara++;
			if (tipoCamara == 4) tipoCamara = 0;
			cambioCamara = true;
		}
	}
	else {
		if (cambioCamara)
			cambioCamara = false;
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_T) == GLFW_PRESS) {
		float *resetPos = new float[3];
		resetPos[0] = pos.getX();
		resetPos[1] = pos.getY() + 30;
		resetPos[2] = pos.getZ();

		float *resetOri = new float[3];
		resetOri[0] = GestorJugadores::getInstancia()->getJugadores().at(0)->getNodo()->getRotation().z;
		resetOri[1] = GestorJugadores::getInstancia()->getJugadores().at(0)->getNodo()->getRotation().y;
		resetOri[2] = GestorJugadores::getInstancia()->getJugadores().at(0)->getNodo()->getRotation().x;

		GestorJugadores::getInstancia()->getJugadores().at(0)->setPosicion(resetPos, resetOri);
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		end = true;
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && end == true) {

		return Escena::tipo_escena::SALIR; // Esto deberia cargar la escena de carga - menu
	}

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_9) == GLFW_PRESS) {
		muestraDebug = true;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_0) == GLFW_PRESS) {
		TMotor::instancia().closeDebugWindow();
		TMotor::instancia().initDebugWindow();
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",30.0f);
		muestraDebug = false;
	}


	return tipoEscena; // Significa que debe seguir ejecutando
}

void EscenaJuego::UpdatePhysics(unsigned int TDeltaTime) {
	//TDeltaTime= TDeltaTime*2;
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();
	mundo->stepSimulation(TDeltaTime * 0.001f, 30);
	int c = 0;
	for (int i = 0; i < objetos.size(); i++) {
		c++;
		UpdateRender(objetos.at(i));
	}
}

// Passes bullet's orientation to irrlicht
void EscenaJuego::UpdateRender(btRigidBody *TObject) {
	obj3D *Node = static_cast<obj3D *>(TObject->getUserPointer());
	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();

	Pista *mapa = Pista::getInstancia();

	//btTransform t;
	//TObject->getMotionState()->getWorldTransform(t);	
	//Node->setPosition(vector3df(t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ()));
	if (strcmp(Node->getName(), "Jugador") == 0 || strcmp(Node->getName(), "JugadorIA") == 0 || strcmp(Node->getName(), "JugadorRed") == 0) {
		//cout << "POSICION: " <<Point[0]<<","<< Point[1]<<","<< Point[2]<<endl; 
		Node->setPosition((float)Point[0], (float)Point[1] + 2, (float)Point[2]);
	}else{

		if(strcmp(Node->getName(), "rueda1") == 0 && strcmp(Node->getName(), "rueda2") == 0 && strcmp(Node->getName(), "rueda3") == 0 &&
		strcmp(Node->getName(), "rueda4") == 0){

		}else{
		
		Node->setPosition((float)Point[0], (float)Point[1], (float)Point[2]);

		}

		}

	// Set rotation
	if(strcmp(Node->getName(), "rueda1") != 0 && strcmp(Node->getName(), "rueda2") != 0 && strcmp(Node->getName(), "rueda3") != 0 &&
		strcmp(Node->getName(), "rueda4") != 0){

	const btQuaternion& TQuat = TObject->getOrientation();
	glm::vec3 axis(TQuat.getAxis().getX(), TQuat.getAxis().getY(), TQuat.getAxis().getZ());
	float angle = TQuat.getAngle() * (180.0f / M_PI);
	Node->setRotation(axis, angle);
		
		}
}

std::string EscenaJuego::getIpConexion() { return ipConexion; }

void EscenaJuego::updateHUD() {
	GestorJugadores *jugadores = GestorJugadores::getInstancia();

	//UPDATE PUESTO
	if (GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getPosicionCarrera() != puesto) {
		puesto = GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getPosicionCarrera();
		switch (GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getPosicionCarrera()) {
		case 1:
			TMotor::instancia().getActiveHud()->changeTextureElement("puesto", "assets/HUD/juego/puesto_1.png");
			break;
		case 2:
			TMotor::instancia().getActiveHud()->changeTextureElement("puesto", "assets/HUD/juego/puesto_2.png");
			break;
		case 3:
			TMotor::instancia().getActiveHud()->changeTextureElement("puesto", "assets/HUD/juego/puesto_3.png");
			break;
		case 4:
			TMotor::instancia().getActiveHud()->changeTextureElement("puesto", "assets/HUD/juego/puesto_4.png");
			break;
		case 5:
			TMotor::instancia().getActiveHud()->changeTextureElement("puesto", "assets/HUD/juego/puesto_5.png");
			break;
		case 6:
			TMotor::instancia().getActiveHud()->changeTextureElement("puesto", "assets/HUD/juego/puesto_6.png");
			break;
		}
	}

	//UPDATE VUELTAS
	if (GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getVueltas() != vueltas) {
		vueltas = GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getVueltas();
		switch (GestorJugadores::getInstancia()->getJugadores().at(controlPlayer)->getVueltas()) {
		case 1:
			TMotor::instancia().getActiveHud()->changeTextureElement("vueltas", "assets/HUD/juego/lap_1_3.png");
			break;
		case 2:
			TMotor::instancia().getActiveHud()->changeTextureElement("vueltas", "assets/HUD/juego/lap_2_3.png");
			break;
		case 3:
			TMotor::instancia().getActiveHud()->changeTextureElement("vueltas", "assets/HUD/juego/lap_3_3.png");
			break;
		}
	}
}

void EscenaJuego::debugRageIA(int k){		//Funcion que sirve para dibujar el nivel de enfado de la IA en una grafica (k=id jugador)

	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	static std::vector<std::vector<float>> rage;
	rage.resize(6);
	rage.at(k).resize(100);
	
	if (t->getTimer()<1){
		
		for (int i=0;i<100;i++){
			rage.at(k).at(i)=0;
		}
	}
	float vec[100];
	int vec2[100];
	rage.at(k).erase(rage.at(k).begin());

	static float f[6];
	if (GestorJugadores::getInstancia()->getJugadores().at(k)->getTipoObj()>0){
		f[k]=50;
	}
	if (GestorJugadores::getInstancia()->getJugadores().at(k)->getLimite()>=100){
		rage.at(k).push_back(100);
	}else{
		if (f[k]+GestorJugadores::getInstancia()->getJugadores().at(k)->getLimite()/2>=100){
			rage.at(k).push_back(99);
		}else{
			rage.at(k).push_back(f[k]+GestorJugadores::getInstancia()->getJugadores().at(k)->getLimite()/2);
		}
		
	}
	for (int i=0;i<100;i++){
		vec2[i]=rage.at(k).at(i);
		vec[i]=vec2[i];	
	}
	if (f[k]>0){
		f[k]-=1;
	}
	std::string extra="% ";
	if (GestorJugadores::getInstancia()->getJugadores().at(k)->getTipoObj()==1 || GestorJugadores::getInstancia()->getJugadores().at(k)->getTipoObj()==6)
	extra="% - Proyectil preparado";
	string str="ENFADO ACUMULADO "+to_string(k);
	ImGui::PlotLines(str.c_str(), vec, IM_ARRAYSIZE(vec),0,(to_string(vec2[99])+extra).c_str(), 0.0f, 100.0f, ImVec2(0,100));
}

void EscenaJuego::debugPlot(int j,float k,std::string str){		//Funcion que sirve para dibujar la variacion de una variable cualquiera en una grafica 
																//(j=id jugador,k=variable a mostrar, str=nombre de la grafica)
	static std::vector<std::vector<float>> valores;
	float vec[10];
	int vec2[10];
	valores.resize(6);
	valores.at(j).resize(100);
	valores.at(j).erase(valores.at(j).begin());
	float f=k;
	valores.at(j).push_back(f);
	for (int i=0;i<10;i++){
		vec2[i]=valores.at(j).at(i);
		vec[i]=vec2[i];
		
	}
	str+=to_string(j);
	ImGui::PlotLines(str.c_str(), vec, IM_ARRAYSIZE(vec),0,(to_string(vec2[9])).c_str());
	
	
}