#include "EscenaJuego.hpp"

EscenaJuego::EscenaJuego(tipo_escena tipo) : Escena(tipo) {
	ipConexion = "";
	TMotor::instancia().newHud("OnGameHUD");
	//TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "aok", "assets/HUD/MainMenu/aok_logo.png");
	//TMotor::instancia().getActiveHud()->traslateElement("aok", 0.0f, 0.45f);
	init();
}

EscenaJuego::EscenaJuego(tipo_escena tipo, std::string ipConexion) : Escena(tipo) {
	this->ipConexion = ipConexion;
	TMotor::instancia().newHud("OnGameHUD");
	init();
}

EscenaJuego::~EscenaJuego() {
	//----------------------------------//
	//-----------DESTRUCTORES-----------//
	//----------------------------------//
	//cout << "\n-------------- A BORRAR LA ESCENA DE JUEGO! -------------- \n";
	//cout << "Vamos a limpiar primero la escena!...";
	limpiar();
	//cout << "Bien!\n";
	//

	//cout << "Voy a entrar en el destructor de bullet. Deseadme suerte...\n";
	delete MotorFisicas::getInstancia();
	//cout << "Bien!\n";

	//cout << "Voy a entrar en el destructor de jugadores. Deseadme suerte...\n";
	delete GestorJugadores::getInstancia();
	//cout << "Bien!\n";

	//cout << "Voy a entrar en el destructor de pista. Deseadme suerte...\n";
	delete Pista::getInstancia();
	//cout << "No ha ido mal.\n";

	//delete TextoPantalla::getInstancia();

	delete camara;
}

void EscenaJuego::init() {

	// LUCES
	obj3D * luzInfinita = TMotor::instancia().newLightNode("light_0", glm::vec4(-1.0f, -1.f, -1.0f, 1.0f), 0.00001f, glm::cos(glm::radians(60.0f)), "escena_raiz");
	luzInfinita->translate(glm::vec3(2.5f, 1.0f, 2.0f));

	obj3D * luzPuntual = TMotor::instancia().newLightNode("light_1", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.00001f, glm::cos(glm::radians(60.0f)), "escena_raiz");
	luzPuntual->translate(glm::vec3(-10.0f,20.0f, 310.0f));

	/*TMotor::instancia().newLightNode(glm::vec3(0.0f, 30.0f, 0.0f), "light_0", glm::vec4(0.0f, 0.f, -1.0f, 1.0f), 0.25f, glm::cos(glm::radians(30.0f)), "escena_raiz");
	obj3D *luz0 = TMotor::instancia().newLightNode("light_0", glm::vec4(.0f, 0.f, 0.f, 0.0f), 0.01f, 0.0f, "escena_raiz");
	luz0->translate(glm::vec3(-290.0f, 100.0f, 30.61f));
	TMotor::instancia().newLightNode(glm::vec3(0.0f, 15.0f, 0.0f), "light_1", glm::vec4(.0f, 0.f, 0.f, 0.0f), 0.01f, 0.0f, "escena_raiz");
	*/

	/*
	obj3D *caja2 = TMotor::instancia().newMeshNode( "escudo2", "assets/wall/wall.obj", "escena_raiz");
	TNodo *aux = TMotor::instancia().getNode("rotacion_escudo2");
	caja2->translate(glm::vec3(0.0f, 0.0f, -6.61f));
	*/


	//ARGUMENTOS MAIN
	debug = 0;
	fin_carrera = false;
	t = Timer::getInstancia();
	
	GestorIDs::instancia().restartID();

	if (tipoEscena == Escena::tipo_escena::ONLINE) {
		client = Client::getInstancia();
		controlPlayer = client->getControlPlayer();
	}
	else {
		controlPlayer = 0;
	}

	// Gravedad
	MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0.0, -10.f, 0.0));

	//----------------------------
	//	Debug Bullet
	//----------------------------
	debugDraw = new DebugDraw(TMotor::instancia().getVentana());
	debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	MotorFisicas::getInstancia()->getMundo()->setDebugDrawer(debugDraw);

	//-----------------------------
	//	ESCENARIO MAPA
	//-----------------------------
	Pista::getInstancia()->setMapa("assets/Mapa01/isla.obj", "assets/Mapa01/FisicasMapaIsla.bullet", "assets/Mapa01/ObjMapa2.0.obj");
	//-----------------------------
	//	JUGADORES
	//-----------------------------
	//Posicion del nodo y el bloque de colisiones centralizado:
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	std::vector<Corredor*> pj = jugadores->getJugadores();
	Corredor* jugador;

	btVector3 pos2[6];
	pos2[0].setX(-10);
	pos2[0].setY(0);
	pos2[0].setZ(310);
	pos2[1].setX(-10);
	pos2[1].setY(0);
	pos2[1].setZ(290);
	pos2[2].setX(-50);
	pos2[2].setY(0);
	pos2[2].setZ(310);
	pos2[3].setX(-50);
	pos2[3].setY(0);
	pos2[3].setZ(290);
	pos2[4].setX(-80);
	pos2[4].setY(0);
	pos2[4].setZ(310);
	pos2[5].setX(-80);
	pos2[5].setY(0);
	pos2[5].setZ(290);

	client = Client::getInstancia();
	int numClients = client->getClientes().size();
	Corredor::tipo_jugador tj;

	for (int i = 0; i < numClients; i++) {
		if (client->getClientes().at(i).tipoCorredor == 0) {
			tj = Corredor::tipo_jugador::GLADIADOR;
		}
		else if (client->getClientes().at(i).tipoCorredor == 1) {
			tj = Corredor::tipo_jugador::PIRATA;
		}
		else if (client->getClientes().at(i).tipoCorredor == 2) {
			tj = Corredor::tipo_jugador::VIKINGO;
		}
		else if (client->getClientes().at(i).tipoCorredor == 3) {
			tj = Corredor::tipo_jugador::CHINO;
		}
		if (tipoEscena != Escena::tipo_escena::ONLINE) {
			if (i == 0) {
				jugador = new CorredorJugador(pos2[i], tj);
			}
			else {
				if (client->getClientes().at(i).corredorJugador == true) {
					jugador = new CorredorJugador(pos2[i], tj);
				}
				else {
					jugador = new CorredorIA(pos2[i], tj);
				}

			}
		}
		else {
			if (i == controlPlayer) {
				jugador = new CorredorJugador(pos2[i], tj);
			}
			else
				jugador = new CorredorRed(pos2[i], tj);
		}
		jugador->setID(i);
		pj.push_back(jugador);
		jugadores->aumentarJugadores();

	}

	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		client->setNetloaded(true);
	}

	jugadores->setJugadores(pj);

	gc = new GestorCarrera();
	
		//-----------------------------
		//	CAMARA
		//-----------------------------
		camara = new Camara3persona();
		tipoCamara = 0;
		cambioCamara = false;
	
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
}

void EscenaJuego::dibujar() {

	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	std::vector<Corredor*> pj = jugadores->getJugadores();

	//------- RENDER ----------
	if (debug) {

		//Todo lo que se quiera dibujar debe ir aqui abajo por la iluminacion
		/*SMaterial materialDriver;
		materialDriver.Lighting = false;
		Motor3d::instancia().getDriver()->setTransform(video::ETS_WORLD, core::matrix4());
		Motor3d::instancia().getDriver()->setMaterial(materialDriver);*/

		for (int i = 0; i < pj.size(); i++) {

			if (strcmp("JugadorIA", pj.at(i)->getNodo()->getName()) == 0) {
				CorredorIA *AUXILIAR = static_cast<CorredorIA *> (pj.at(i));
				AUXILIAR->setDebugFisicas(true);
				AUXILIAR->ActualizarRaytest();
			}
		}

		MotorFisicas::getInstancia()->getMundo()->debugDrawWorld();
	}
	else {
		for (int i = 0; i < pj.size(); i++) {
			if (strcmp("JugadorIA", pj.at(i)->getNodo()->getName()) == 0) {

				CorredorIA *AUXILIAR = static_cast<CorredorIA *> (pj.at(i));
				if (!AUXILIAR->getDebugFisicas()) {
					i = pj.size();
				}
				else {
					AUXILIAR->setDebugFisicas(false);
					AUXILIAR->ActualizarRaytest();
				}
			}
		}

	}
}

void EscenaJuego::limpiar() {
	//Motor3d::instancia().getScene()->clear();
	//Motor3d::instancia().getGUI()->clear();
}

void EscenaJuego::update() {
	//TextoPantalla *textoDebug = TextoPantalla::getInstancia();
	Pista *pistaca = Pista::getInstancia();
	std::vector<Item *> items = pistaca->getItems();
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	std::vector<Corredor*> pj = jugadores->getJugadores();

	if (tipoEscena == Escena::tipo_escena::ONLINE) {
		client->ReceivePackets();
		controlPlayer = client->getControlPlayer();
	}

	DeltaTime = glfwGetTime()*1000 - TimeStamp;
	TimeStamp = glfwGetTime()*1000;
	UpdatePhysics(DeltaTime);

	for (int i = 0; i < pistaca->getTamCajas(); i++) {
		pistaca->getArrayCaja()[i]->comprobarRespawn(); // TODO: MOVER AL UPDATE DE PISTACA
	}

	for (int i = 0; i < items.size(); i++)
	{
		if (items.at(i)->getLanzado()) {

			if (items.at(i)->update()) {

				if (strcmp("Escudo", items.at(i)->getNombre()) == 0) {
					pj.at(items.at(i)->getIDPadre())->setProteccion(false);
				}

				if (strcmp("HabilidadPirata", items.at(i)->getNombre()) == 0
					|| strcmp("HabilidadVikingo", items.at(i)->getNombre()) == 0
					|| strcmp("HabilidadGladiador", items.at(i)->getNombre()) == 0
					|| strcmp("HabilidadChino", items.at(i)->getNombre()) == 0) {


					pj.at(items.at(i)->getIDPadre())->setHabilidad(false);

				}

				items.at(i)->Delete();
				items.erase(items.begin() + i);
				pistaca->setItems(items);
				break;

			}
			else {
				items.at(i)->updateHijos();

			}
		}
	}


	if (fin_carrera) {
		cout << "CARRERA FINALIZADA, PULSA F.";
	}
	//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());



	if (t->getTimer() <= 3 && t->getTimer() >= 1) {
		int desc = 4 - t->getTimer();
		//textoDebug->agregar(to_string(desc));
	}

	if (t->getTimer() >= 4) {
		for (int i = 0; i < jugadores->getNumJugadores(); i++) {
			pj.at(i)->getEstados()->setEstadoCarrera(CARRERA);
		}
		colisiones->IniciarTimer();
	}

	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		//pj.at(controlPlayer)->actualizarItem();
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
		//pj.at(controlPlayer)->update();

		if (jugadores->getNumJugadores() > 0){
			for (int i = 0; i < jugadores->getNumJugadores(); i++) {
				pj.at(i)->update();

			}
		}
		//textoDebug->agregar(pj.at(0)->toString());
	}
	else {
		//cout << jugadores->getNumJugadores() << endl;
		//if (jugadores->getNumJugadores() != 0)
		//	pj.at(controlPlayer)->actualizarItem();

		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
										  //colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED
		if (jugadores->getNumJugadores() != 0)
			client->aumentarTimestamp();
		for (int i = 0; i < jugadores->getNumJugadores(); i++) {
			pj.at(i)->update();
		}

		//textoDebug->agregar("\n ---- CORREDOR 1 JUGADOR ----\n");
		//if (jugadores->getNumJugadores() != 0)
			//textoDebug->agregar(pj.at(controlPlayer)->toString());

			if (jugadores->getNumJugadores() != 0) {
				float tiempoActual = glfwGetTime();
				float timediff_sec = tiempoActual - tiempoRefresco;

				//if (timediff_sec >= 0.01) {
				client->PlayerMovement();
				tiempoRefresco = glfwGetTime();
				//}
				//client->PlayerAction();
				client->UpdateNetworkKeyboard();
			}
	}

	if (jugadores->getNumJugadores() != 0)
		if (gc->update())
			fin_carrera = true;


	jugadores->setJugadores(pj);


	pj = jugadores->getJugadores();
	/*switch(tipoCamara){
		case 0:		//Camara 3a persona fija
			camara->moveCamera(pj.at(controlPlayer));
		break;
		case 1:		//Camara 3a persona libre
			camara->moveCameraControl(pj.at(controlPlayer));
		break;
		case 2:		//Camara 1a persona
			camara->movefpsCamera(pj.at(controlPlayer));
		break;
		case 3:
			camara->moveCameraControlPointer(pj.at(controlPlayer));

	}*/


}

Escena::tipo_escena EscenaJuego::comprobarInputs() {
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	std::vector<Corredor*> pj = jugadores->getJugadores();
	btVector3 pos(-10, 0, 310);
	int i = 0;
	if (tipoEscena == Escena::tipo_escena::ONLINE)
		i = client->getControlPlayer();

	//------- ENTRADA TECLADO ----------
	if (fin_carrera == true && glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_F) == GLFW_PRESS) {
		if (tipoEscena == Escena::tipo_escena::ONLINE && controlPlayer == 0) {
			for (int i = 0; i < pj.size(); i++) {
				pj.at(i)->setVueltas(1);
			}
			jugadores->setJugadores(pj);
			client->FinalizarCarrera();
		}
		else if (tipoEscena != Escena::tipo_escena::ONLINE) {
			return Escena::tipo_escena::MENU;
		}
	}
	if (tipoEscena == Escena::tipo_escena::ONLINE && !client->getStarted()) {

		return Escena::tipo_escena::LOBBY;
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
		resetPos[1] = pos.getY() + 40;
		resetPos[2] = pos.getZ();

		float *resetOri = new float[3];
		resetOri[0] = pj.at(0)->getNodo()->getRotation().z;
		resetOri[1] = pj.at(0)->getNodo()->getRotation().y;
		resetOri[2] = pj.at(0)->getNodo()->getRotation().x;

		pj.at(0)->setPosicion(resetPos, resetOri);
	}

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (tipoEscena == Escena::tipo_escena::ONLINE) {
			client->ShutDownClient();
		}
		else {
			client->BorrarClientes();
		}

		return Escena::tipo_escena::MENU; // Esto deberia cargar la escena de carga - menu
	}

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_0) == GLFW_PRESS) {
		debug = 0;
	}
	else {
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_9) == GLFW_PRESS) {
			debug = 1;
		}
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
	}
	else
		Node->setPosition((float)Point[0], (float)Point[1], (float)Point[2]);

	
	
	// Set rotation
	const btQuaternion& TQuat = TObject->getOrientation();

	glm::vec3 axis(TQuat.getAxis().getX(), TQuat.getAxis().getY(), TQuat.getAxis().getZ());
	float angle = TQuat.getAngle() * RADTODEG;
	/*if (strcmp(Node->getName(), "Jugador") == 0 ){
		axis.x = abs(axis.x);
		axis.y = abs(axis.y);
		axis.z = abs(axis.z);
		axis.x = round(axis.x);
		axis.y = round(axis.y);
		axis.z = round(axis.z);
		if (angle < 5){
			angle = 0;
		}
		angle = round(angle);*/
		/*
		axis.x = trunc(axis.x * 20 + 0.05) / 20;
		axis.y = trunc(axis.y * 20 + 0.05) / 20;
		axis.z = trunc(axis.z * 20 + 0.05) / 20;*/
	/*cout<< "AXIS: "<<axis.x<<","<< axis.y<<","<< axis.z<<endl;
	cout<<"ANGLE: "<<angle<<endl;
	cout<<"---------------------"<<endl;
	}*/
	Node->setRotation(axis, angle);
	
}
std::string EscenaJuego::getIpConexion() {
	return ipConexion;
}