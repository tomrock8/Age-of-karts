#include "EscenaJuego.hpp"

EscenaJuego::EscenaJuego(tipo_escena tipo) : Escena(tipo) {
	ipConexion = "";
	init();	
}

EscenaJuego::EscenaJuego(tipo_escena tipo, std::string ipConexion) : Escena(tipo) {
	this->ipConexion = ipConexion;
	init();
}

EscenaJuego::~EscenaJuego() {
	//----------------------------------//
	//-----------DESTRUCTORES-----------//
	//----------------------------------//
	cout << "\n-------------- A BORRAR LA ESCENA DE JUEGO! -------------- \n";
	cout << "Vamos a limpiar primero la escena!...";
	limpiar();
	cout << "Bien!\n";


	cout << "Voy a entrar en el destructor de bullet. Deseadme suerte...\n";
	delete MotorFisicas::getInstancia();
	cout << "Bien!\n";

	cout << "Voy a entrar en el destructor de jugadores. Deseadme suerte...\n";
	delete GestorJugadores::getInstancia();
	cout << "Bien!\n";

	cout << "Voy a entrar en el destructor de pista. Deseadme suerte...\n";
	delete Pista::getInstancia();
	cout << "No ha ido mal.\n";

	delete TextoPantalla::getInstancia();

	delete camara;
}

void EscenaJuego::init() {
	//ARGUMENTOS MAIN
	debug = 0;
	

	if (tipoEscena == Escena::tipo_escena::ONLINE) {
		client = Client::getInstancia();
		client->CreateClientInterface();
		client->SetIP(ipConexion);
		client->ClientStartup();
	}

	// Gravedad
	MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0, -30, 0));

	//----------------------------
	//	Debug Bullet
	//----------------------------
	debugDraw = new DebugDraw(Motor3d::instancia().getDevice());
	debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	MotorFisicas::getInstancia()->getMundo()->setDebugDrawer(debugDraw);

	//-----------------------------
	//	ESCENARIO MAPA
	//-----------------------------
	Pista *pistaca = Pista::getInstancia();
	pistaca->setMapa("assets/Mapa01/mapaIsla.obj", "assets/Mapa01/FisicasMapaIsla.bullet", "assets/Mapa01/ObjMapa2.0.obj");
	pistaca->getArrayWaypoints();

	//-----------------------------
	//	JUGADORES
	//-----------------------------
	//Posicion del nodo y el bloque de colisiones centralizado:
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();

	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		pj[0] = new CorredorJugador("assets/coche.obj", btVector3(-10, 0, 310),Corredor::tipo_jugador::CHINO);
		jugadores->aumentarJugadores();

		pj[1] = new CorredorIA("assets/coche.obj", btVector3(-10, 0, 280),Corredor::tipo_jugador::GLADIADOR);
		jugadores->aumentarJugadores();

	
		pj[2] = new CorredorIA("assets/coche.obj", btVector3(-50, 0, 300),Corredor::tipo_jugador::PIRATA);
		jugadores->aumentarJugadores();

		pj[3] = new CorredorIA("assets/coche.obj", btVector3(-50, 0, 280),Corredor::tipo_jugador::VIKINGO);
		jugadores->aumentarJugadores();

		pj[4] = new CorredorIA("assets/coche.obj", btVector3(-100, 0, 300),Corredor::tipo_jugador::GLADIADOR);
		jugadores->aumentarJugadores();

		pj[5] = new CorredorIA("assets/coche.obj", btVector3(-100, 0, 290),Corredor::tipo_jugador::CHINO);
		jugadores->aumentarJugadores();



		pj[0]->getNodo()->setID(0);
		pj[1]->getNodo()->setID(1);
		

		
				pj[2]->getNodo()->setID(2);
				pj[3]->getNodo()->setID(3);
				pj[4]->getNodo()->setID(4);
				pj[5]->getNodo()->setID(5);
		

	}

	jugadores->setJugadores(pj);

	gc = new GestorCarrera();

	//-----------------------------
	//	CAMARA
	//-----------------------------
	camara = new Camara3persona();

	//-----------------------------
	//	GESTOR COLISIONES
	//-----------------------------
	colisiones = new GestorColisiones();
	TextoPantalla *textoDebug = TextoPantalla::getInstancia();

	//-----------------------------
	//	TIME
	//-----------------------------
	lastFPS = -1;
	TimeStamp = Motor3d::instancia().getDevice()->getTimer()->getTime();
	DeltaTime = 0;
	tiempoRefresco = clock();
}

void EscenaJuego::dibujar() {

	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();

	//------- RENDER ----------
	Motor3d::instancia().iniciarDibujado();
	Motor3d::instancia().getScene()->drawAll();

	//Todo lo que se quiera dibujar debe ir aqui abajo por la iluminacion
	SMaterial materialDriver;
	materialDriver.Lighting = false;
	Motor3d::instancia().getDriver()->setTransform(video::ETS_WORLD, core::matrix4());
	Motor3d::instancia().getDriver()->setMaterial(materialDriver);
	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		CorredorIA *COMENARDOSAUXILIAR1 = static_cast<CorredorIA *>(pj[1]);
		
				CorredorIA *COMENARDOSAUXILIAR2 = static_cast<CorredorIA *>(pj[2]);
				CorredorIA *COMENARDOSAUXILIAR3 = static_cast<CorredorIA *>(pj[3]);
				CorredorIA *COMENARDOSAUXILIAR4 = static_cast<CorredorIA *>(pj[4]);
				CorredorIA *COMENARDOSAUXILIAR5 = static_cast<CorredorIA *>(pj[5]);
		

		//COMENARDOSAUXILIAR->update();
		COMENARDOSAUXILIAR1->ActualizarRaytest();
		
				COMENARDOSAUXILIAR2->ActualizarRaytest();
				COMENARDOSAUXILIAR3->ActualizarRaytest();
				COMENARDOSAUXILIAR4->ActualizarRaytest();
				COMENARDOSAUXILIAR5->ActualizarRaytest();
		

		//Para poder dibujar putas lineas de mierda
	}
	//Para poder dibujar putas lineas de mierda
	if (debug) {
		SMaterial debugMat;
		debugMat.Lighting = true;
		Motor3d::instancia().getDriver()->setMaterial(debugMat);
		Motor3d::instancia().getDriver()->setTransform(ETS_WORLD, IdentityMatrix);
		MotorFisicas::getInstancia()->getMundo()->debugDrawWorld();
	}

	Motor3d::instancia().getGUI()->drawAll();
	Motor3d::instancia().terminarDibujado();
}

void EscenaJuego::limpiar() {
	Motor3d::instancia().getScene()->clear();
	Motor3d::instancia().getGUI()->clear();
}

void EscenaJuego::update() {
	TextoPantalla *textoDebug = TextoPantalla::getInstancia();
	Pista *pistaca = Pista::getInstancia();
	core::list<Item *> items = pistaca->getItems();
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();

	if (tipoEscena == Escena::tipo_escena::ONLINE) {
		client->ReceivePackets(Motor3d::instancia().getScene());
		//client->SpawnPlayer(smgr);
	}
	//cout << irrTimer->getTime() << endl;
	textoDebug->limpiar();

	DeltaTime = Motor3d::instancia().getDevice()->getTimer()->getTime() - TimeStamp;
	TimeStamp = Motor3d::instancia().getDevice()->getTimer()->getTime();
	UpdatePhysics(DeltaTime);

	for (int i = 0; i < pistaca->getTamCajas(); i++) {
		pistaca->getArrayCaja()[i]->comprobarRespawn(); // TODO: MOVER AL UPDATE DE PISTACA
	}

	for (core::list<Item *>::Iterator Iterator = items.begin(); Iterator != items.end(); ++Iterator)
	{
		Item *item = *Iterator;
		if(item->getLanzado()){
			if(item->comprobarDestructor()){
				item->Delete();
				Iterator = items.erase(Iterator);
				break;
			}
		}
	}
	pistaca->setItems(items);


	//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());
	pj = jugadores->getJugadores();
	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		pj[0]->actualizarItem();
		//pj[1]->actualizarItem();
		camara->moveCameraControl(pj[0]); 
		//camara->moveCamera(pj[0]);
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
		pj[0]->update();
		pj[1]->update();

				pj[2]->update();
				pj[3]->update();
				pj[4]->update();
				pj[5]->update();
		
		//textoDebug->agregar(pj[0]->toString());
	}
	else {
		//cout << jugadores->getNumJugadores() << endl;
		int controlPlayer = client->getControlPlayer();
		if (jugadores->getNumJugadores() != 0)
			pj[controlPlayer]->actualizarItem();

		if (jugadores->getNumJugadores() != 0)
		//	camara->moveCamera(pj[controlPlayer]);
			camara->moveCameraControl(pj[controlPlayer]);
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
										  //colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED

		if (jugadores->getNumJugadores() != 0)
			pj[controlPlayer]->update();
		//pj[1]->update();

		if (jugadores->getNumJugadores() != 0)
			for (int i = 0; i < jugadores->getNumJugadores(); i++) {
				if (i != controlPlayer) {
					pj[i]->updateRed();
				}
			}

		textoDebug->agregar("\n ---- CORREDOR 1 JUGADOR ----\n");
		if (jugadores->getNumJugadores() != 0)
			textoDebug->agregar(pj[controlPlayer]->toString());

		if (jugadores->getNumJugadores() != 0) {
			clock_t tiempoActual = clock();
			clock_t timediff = tiempoActual - tiempoRefresco;
			float timediff_sec = ((float)timediff) / 100000;
			if (timediff_sec >= 0.01) {
				client->PlayerMovement();
				tiempoRefresco = clock();
			}
			client->PlayerAction();
		}
	}

	if (jugadores->getNumJugadores() != 0)
		gc->update();

	jugadores->setJugadores(pj);

	int fps = Motor3d::instancia().getDriver()->getFPS();
	if (lastFPS != fps) {
		core::stringw tmp(L"Age of karts [");
		tmp += Motor3d::instancia().getDriver()->getName();
		tmp += L"] fps: ";
		tmp += fps;

		Motor3d::instancia().getDevice()->setWindowCaption(tmp.c_str());
		lastFPS = fps;
	}
}

Escena::tipo_escena EscenaJuego::comprobarInputs() {
	CTeclado *teclado = CTeclado::getInstancia();
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();
	vector3df pos(-10, 0, 310);
	int i = 0;
	if (tipoEscena == Escena::tipo_escena::ONLINE)
		i = client->getControlPlayer();

	//------- ENTRADA TECLADO ----------
	if (teclado->isKeyDown(KEY_KEY_R)) {
		btVector3 btPos = pj[i]->getWaypointActual()->getPosicion();

		btTransform trans;
		trans.setOrigin(btPos);
		btQuaternion quaternion;
		quaternion.setEulerZYX(pj[i]->getNodo()->getRotation().Z* PI / 180, pj[i]->getNodo()->getRotation().Y * PI / 180, pj[i]->getNodo()->getRotation().X* PI / 180);
		trans.setRotation(quaternion);

		pj[i]->getRigidBody()->setCenterOfMassTransform(trans);
		pj[i]->resetFuerzas();
		//pj[0]->getNodo()->setPosition(pos);
	}
	if (teclado->isKeyDown(KEY_KEY_T)) {
		float *resetPos = new float[3];
		resetPos[0] = pos.X;
		resetPos[1] = pos.Y + 40;
		resetPos[2] = pos.Z;

		float *resetOri = new float[3];
		resetOri[0] = pj[0]->getNodo()->getRotation().Z;
		resetOri[1] = pj[0]->getNodo()->getRotation().Y;
		resetOri[2] = pj[0]->getNodo()->getRotation().X;

		pj[0]->setPosicion(resetPos, resetOri);
	}

	if (teclado->isKeyDown(KEY_ESCAPE)) {
		if (tipoEscena == Escena::tipo_escena::ONLINE)
			client->ShutDownClient();

		return Escena::tipo_escena::MENU; // Esto deberia cargar la escena de carga - menu
	}

	if (teclado->isKeyDown(KEY_KEY_0)) {
		debug = 0;
	}
	else {
		if (teclado->isKeyDown(KEY_KEY_9)) {
			debug = 1;
		}
	}

	return tipoEscena; // Significa que debe seguir ejecutando
}

void EscenaJuego::UpdatePhysics(u32 TDeltaTime) {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();
	mundo->stepSimulation(TDeltaTime * 0.001f, 30);
	int c = 0;
	for (list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator) {
		c++;
		UpdateRender(*Iterator);
	}
}

// Passes bullet's orientation to irrlicht
void EscenaJuego::UpdateRender(btRigidBody *TObject) {
	IMeshSceneNode *Node = static_cast<IMeshSceneNode *>(TObject->getUserPointer());
	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();

	Pista *mapa = Pista::getInstancia();
	//btTransform t;
	//TObject->getMotionState()->getWorldTransform(t);	
	//Node->setPosition(vector3df(t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ()));
	if (strcmp(Node->getName(), "Jugador") == 0 || strcmp(Node->getName(), "JugadorIA") == 0) {
		Node->setPosition(vector3df((f32)Point[0], (f32)Point[1] + 2, (f32)Point[2]));
	}
	else
		Node->setPosition(vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));
	// Set rotation
	vector3df Euler;
	const btQuaternion& TQuat = TObject->getOrientation();
	quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;
	Node->setRotation(Euler);

}
