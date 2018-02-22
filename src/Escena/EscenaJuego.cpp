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
		controlPlayer = client->getControlPlayer();
		cout << "ControlPlayer: " << controlPlayer << endl;
	}else{
		controlPlayer = 0;
	}

	// Gravedad
	MotorFisicas::getInstancia()->getMundo()->setGravity(btVector3(0.0, -50.f, 0.0));

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
	vector<Corredor*> pj = jugadores->getJugadores();
	Corredor* jugador;
cout << "Empiezo a crear jugadores\n";
	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		
		jugador = new CorredorJugador("assets/coche.obj", btVector3(-10, 0, 310),Corredor::tipo_jugador::CHINO);
		pj.push_back(jugador);
		jugadores->aumentarJugadores();

		jugador = new CorredorIA("assets/coche.obj", btVector3(-10, 0, 280),Corredor::tipo_jugador::GLADIADOR);
		pj.push_back(jugador);
		jugadores->aumentarJugadores();

		jugador = new CorredorIA("assets/coche.obj", btVector3(-50, 0, 300),Corredor::tipo_jugador::PIRATA);
		pj.push_back(jugador);
		jugadores->aumentarJugadores();

		jugador = new CorredorIA("assets/coche.obj", btVector3(-50, 0, 280),Corredor::tipo_jugador::VIKINGO);
		pj.push_back(jugador);
		jugadores->aumentarJugadores();

		jugador = new CorredorIA("assets/coche.obj", btVector3(-100, 0, 300),Corredor::tipo_jugador::GLADIADOR);
		pj.push_back(jugador);
		jugadores->aumentarJugadores();

		jugador = new CorredorIA("assets/coche.obj", btVector3(-100, 0, 290),Corredor::tipo_jugador::CHINO);
		pj.push_back(jugador);
		jugadores->aumentarJugadores();



		pj.at(0)->getNodo()->setID(0);
		pj.at(1)->getNodo()->setID(1);
		
			pj.at(2)->getNodo()->setID(2);
			pj.at(3)->getNodo()->setID(3);
			pj.at(4)->getNodo()->setID(4);
			pj.at(5)->getNodo()->setID(5);
		

	}else{
		btVector3 pos2[6];
		pos2[0].setX(-10);
		pos2[0].setY(0);
		pos2[0].setZ(310);
		pos2[1].setX(-10);
		pos2[1].setY(0);
		pos2[1].setZ(290);
		pos2[2].setX(-20);
		pos2[2].setY(0);
		pos2[2].setZ(310);
		pos2[3].setX(-20);
		pos2[3].setY(0);
		pos2[3].setZ(290);
		pos2[4].setX(-30);
		pos2[4].setY(0);
		pos2[4].setZ(310);
		pos2[5].setX(-30);
		pos2[5].setY(0);
		pos2[5].setZ(290);
		int numClients = client->getNumClients();
		for(int i = 0; i< numClients; i++){
			cout << "Creamos jugador " << i << endl;
			jugador = new CorredorRed("assets/coche.obj", pos2[i], Corredor::tipo_jugador::CHINO);
			pj.push_back(jugador);
			jugadores->aumentarJugadores();
		}
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
	vector<Corredor*> pj = jugadores->getJugadores();

	//------- RENDER ----------
	Motor3d::instancia().iniciarDibujado();
	Motor3d::instancia().getScene()->drawAll();

	//Todo lo que se quiera dibujar debe ir aqui abajo por la iluminacion
	SMaterial materialDriver;
	materialDriver.Lighting = false;
	Motor3d::instancia().getDriver()->setTransform(video::ETS_WORLD, core::matrix4());
	Motor3d::instancia().getDriver()->setMaterial(materialDriver);
	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		CorredorIA *COMENARDOSAUXILIAR1 = static_cast<CorredorIA *>(pj.at(1));
		
				CorredorIA *COMENARDOSAUXILIAR2 = static_cast<CorredorIA *>(pj.at(2));
				CorredorIA *COMENARDOSAUXILIAR3 = static_cast<CorredorIA *>(pj.at(3));
				CorredorIA *COMENARDOSAUXILIAR4 = static_cast<CorredorIA *>(pj.at(4));
				CorredorIA *COMENARDOSAUXILIAR5 = static_cast<CorredorIA *>(pj.at(5));
		

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
	cout << "Entro UPDATE\n";
	TextoPantalla *textoDebug = TextoPantalla::getInstancia();
	Pista *pistaca = Pista::getInstancia();
	core::list<Item *> items = pistaca->getItems();
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	vector<Corredor*> pj = jugadores->getJugadores();

	if (tipoEscena == Escena::tipo_escena::ONLINE) {
		client->ReceivePackets();
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
	switch(tipoCamara){
		case 0:		//Camara 3a persona libre
			camara->moveCameraControl(pj.at(controlPlayer)); 
		break;
		case 1:		//Camara 3a persona fija
			camara->moveCamera(pj.at(controlPlayer));
		break;
		case 2:		//Camara 1a persona
			camara->movefpsCamera(pj.at(controlPlayer));
		break;
		case 3:
			camara->moveCameraControlPointer(pj.at(controlPlayer));

	}
	if (tipoEscena != Escena::tipo_escena::ONLINE) {
		pj.at(controlPlayer)->actualizarItem();
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
		pj.at(controlPlayer)->update();
		pj.at(1)->update();

				pj.at(2)->update();
				pj.at(3)->update();
				pj.at(4)->update();
				pj.at(5)->update();
		
		//textoDebug->agregar(pj.at(0)->toString());
	}
	else {
		//cout << jugadores->getNumJugadores() << endl;
		if (jugadores->getNumJugadores() != 0)
			pj.at(controlPlayer)->actualizarItem();

		if (jugadores->getNumJugadores() != 0)
		//	camara->moveCamera(pj[controlPlayer]);
			camara->moveCameraControl(pj.at(controlPlayer));
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
										  //colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED

		if (jugadores->getNumJugadores() != 0)
			pj.at(controlPlayer)->update();

		if (jugadores->getNumJugadores() != 0)
			for (int i = 0; i < jugadores->getNumJugadores(); i++) {
				if (i != controlPlayer) {
					pj.at(i)->updateRed();
				}
			}

		textoDebug->agregar("\n ---- CORREDOR 1 JUGADOR ----\n");
		if (jugadores->getNumJugadores() != 0)
			textoDebug->agregar(pj.at(controlPlayer)->toString());

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
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	vector<Corredor*> pj = jugadores->getJugadores();
	vector3df pos(-10, 0, 310);
	int i = 0;
	if (tipoEscena == Escena::tipo_escena::ONLINE)
		i = client->getControlPlayer();

	//------- ENTRADA TECLADO ----------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
		btVector3 btPos = pj.at(i)->getWaypointActual()->getPosicion();

		btTransform trans;
		trans.setOrigin(btPos);
		btQuaternion quaternion;
		quaternion.setEulerZYX(pj.at(i)->getNodo()->getRotation().Z* PI / 180, pj.at(i)->getNodo()->getRotation().Y * PI / 180, pj.at(i)->getNodo()->getRotation().X* PI / 180);
		trans.setRotation(quaternion);

		pj.at(i)->getRigidBody()->setCenterOfMassTransform(trans);
		pj.at(i)->resetFuerzas();
		//pj.at(0)->getNodo()->setPosition(pos);
	}

	/*if (sf::Keyboard::isKeyReleased(sf::Keyboard::C)){
		if(cambioCamara)
			cambioCamara = false;
	}*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
		if(!cambioCamara){
			tipoCamara++;
			if(tipoCamara == 4) tipoCamara = 0;
			cambioCamara = true;
		}
	}else{
		if(cambioCamara)
			cambioCamara = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)){
		float *resetPos = new float[3];
		resetPos[0] = pos.X;
		resetPos[1] = pos.Y + 40;
		resetPos[2] = pos.Z;

		float *resetOri = new float[3];
		resetOri[0] = pj.at(0)->getNodo()->getRotation().Z;
		resetOri[1] = pj.at(0)->getNodo()->getRotation().Y;
		resetOri[2] = pj.at(0)->getNodo()->getRotation().X;

		pj.at(0)->setPosicion(resetPos, resetOri);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		if (tipoEscena == Escena::tipo_escena::ONLINE)
			client->ShutDownClient();

		return Escena::tipo_escena::MENU; // Esto deberia cargar la escena de carga - menu
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){
		debug = 0;
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)){
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
