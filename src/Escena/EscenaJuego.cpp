#include "EscenaJuego.hpp"

EscenaJuego::EscenaJuego(tipo_escena tipo) : Escena(tipo) {
}

EscenaJuego::~EscenaJuego() {
	//----------------------------------//
	//-----------DESTRUCTORES-----------//
	//----------------------------------//
	cout << "\n-------------- A BORRAR LA ESCENA DE JUEGO! -------------- \n";
	cout << "Vamos a limpiar primero la escena!...";
	limpiar();
	cout << "Bien!\n";


	cout << "Voy a entrar en el destructor de bullet. Deseadme suerte...";
	delete MotorFisicas::getInstancia();
	cout << "\n";

	cout << "Voy a entrar en el destructor de jugadores. Deseadme suerte...";
	delete GestorJugadores::getInstancia();
	cout << "\n";

	cout << "Voy a entrar en el destructor de pista. Deseadme suerte...";
	delete Pista::getInstancia();
	cout << "No ha ido mal.\n";

	delete camara;
}

void EscenaJuego::init() {
	//ARGUMENTOS MAIN
	argc = 0;
	debug = 0;

	CTeclado *teclado = CTeclado::getInstancia();

	if (argc == 2) {
		client = new Client(4);
		client->CreateClientInterface();
		client->SetIP();
		client->ClientStartup();
	}

	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	Motor3d *m = Motor3d::getInstancia();
	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();
	IrrlichtDevice *device = m->getDevice();


	//----------------------------
	//	BULLET
	//----------------------------
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	mundo->setGravity(btVector3(0, -25, 0));

	//----------------------------
	//	Debug Bullet
	//----------------------------
	debugDraw = new DebugDraw(device);
	debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mundo->setDebugDrawer(debugDraw);

	//-----------------------------
	//	ESCENARIO MAPA
	//-----------------------------
	Pista *pistaca = Pista::getInstancia();
	pistaca->setMapa("assets/Mapa01/mapaIsla.obj", "assets/Mapa01/FisicasMapaIsla.bullet", "assets/Mapa01/WPTrbBox2.obj");
	pistaca->getArrayWaypoints();

	//-----------------------------
	//	JUGADORES
	//-----------------------------
	//Posicion del nodo y el bloque de colisiones centralizado:
	vector3df pos(0, 1, 300);
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();

	if (argc != 2) {
		pj[0] = new CorredorJugador("assets/coche.obj", pos);
		jugadores->aumentarJugadores();
	}

	jugadores->setJugadores(pj);

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
	TimeStamp = m->getDevice()->getTimer()->getTime();
	DeltaTime = 0;
}

void EscenaJuego::dibujar() {
	Motor3d *m = Motor3d::getInstancia();
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();

	//------- RENDER ----------
	m->dibujar();
	//Todo lo que se quiera dibujar debe ir aqui abajo por la iluminacion
	SMaterial materialDriver;
	materialDriver.Lighting = false;
	m->getDriver()->setTransform(video::ETS_WORLD, core::matrix4());
	m->getDriver()->setMaterial(materialDriver);
	if (argc != 2)
		pj[0]->ActualizarRaytest();
	//Para poder dibujar putas lineas de mierda
	if (debug) {
		SMaterial debugMat;
		debugMat.Lighting = true;
		m->getDriver()->setMaterial(debugMat);
		m->getDriver()->setTransform(ETS_WORLD, IdentityMatrix);
		mundo->debugDrawWorld();
	}
	m->getGUI()->drawAll();
	// draw gui
	m->getDriver()->endScene();
}

void EscenaJuego::limpiar() {
	Motor3d *m = Motor3d::getInstancia();
	m->getScene()->clear();
}

void EscenaJuego::update() {
	Motor3d *m = Motor3d::getInstancia();
	TextoPantalla *textoDebug = TextoPantalla::getInstancia();
	Pista *pistaca = Pista::getInstancia();
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();


	comprobarInputs();
	if (argc == 2) {
		client->ReceivePackets(m->getScene());
		//client->SpawnPlayer(smgr);
	}
	//cout << irrTimer->getTime() << endl;
	textoDebug->limpiar();

	DeltaTime = m->getDevice()->getTimer()->getTime() - TimeStamp;
	TimeStamp = m->getDevice()->getTimer()->getTime();
	UpdatePhysics(DeltaTime);

	for (int i = 0; i < pistaca->getTamCajas(); i++) {
		pistaca->getArrayCaja()[i]->comprobarRespawn();
	}
	//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());
	pj = jugadores->getJugadores();
	if (argc != 2) {
		pj[0]->actualizarItem();
		camara->moveCameraControl(pj[0]);
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
		pj[0]->update();
		textoDebug->agregar(pj[0]->toString());
	}
	else {
		int controlPlayer = client->getControlPlayer();
		if (jugadores->getNumJugadores() != 0)
			pj[controlPlayer]->actualizarItem();

		if (jugadores->getNumJugadores() != 0)
			camara->moveCameraControl(pj[controlPlayer]);
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
										  //colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED

		if (jugadores->getNumJugadores() != 0)
			pj[controlPlayer]->update();
		//pj[1]->update();

		textoDebug->agregar("\n ---- CORREDOR 1 JUGADOR ----\n");
		if (jugadores->getNumJugadores() != 0)
			textoDebug->agregar(pj[controlPlayer]->toString());

		if (jugadores->getNumJugadores() != 0)
			client->PlayerMovement();
	}

	jugadores->setJugadores(pj);

	int fps = m->getDriver()->getFPS();
	if (lastFPS != fps) {
		core::stringw tmp(L"Age of karts [");
		tmp += m->getDriver()->getName();
		tmp += L"] fps: ";
		tmp += fps;

		m->getDevice()->setWindowCaption(tmp.c_str());
		lastFPS = fps;
	}


	dibujar();
}

void EscenaJuego::comprobarInputs() {
	Motor3d *m = Motor3d::getInstancia();
	CTeclado *teclado = CTeclado::getInstancia();
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();

	//------- ENTRADA TECLADO ----------
	if (teclado->isKeyDown(KEY_KEY_R)) {
		btVector3 btPos(pos.X, pos.Y, pos.Z);

		btTransform trans;
		trans.setOrigin(btPos);
		btQuaternion quaternion;
		quaternion.setEulerZYX(pj[0]->getNodo()->getRotation().Z* PI / 180, pj[0]->getNodo()->getRotation().Y * PI / 180, pj[0]->getNodo()->getRotation().X* PI / 180);
		trans.setRotation(quaternion);

		pj[0]->getRigidBody()->setCenterOfMassTransform(trans);
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
		if (argc == 2)
			client->ShutDownClient();
		m->cerrar();
		//return 0;
	}

	if (teclado->isKeyDown(KEY_KEY_0)) {
		debug = 0;
	}
	else {
		if (teclado->isKeyDown(KEY_KEY_9)) {
			debug = 1;
		}
	}

	if (teclado->isKeyDown(KEY_F5)) {
		GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::CARRERA);
	}


	//-------ENTRADA TECLADO FIN----------//
}

void EscenaJuego::UpdatePhysics(u32 TDeltaTime) {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();
	mundo->stepSimulation(TDeltaTime * 0.001f, 60);
	int c = 0;
	for (list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator) {
		c++;
		UpdateRender(*Iterator);
	}
}

// Passes bullet's orientation to irrlicht
void EscenaJuego::UpdateRender(btRigidBody *TObject) {
	Motor3d *m = Motor3d::getInstancia();
	IMeshSceneNode *Node = static_cast<IMeshSceneNode *>(TObject->getUserPointer());
	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();

	Pista *mapa = Pista::getInstancia();
	//btTransform t;
	//TObject->getMotionState()->getWorldTransform(t);	
	//Node->setPosition(vector3df(t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ()));
	if (strcmp(Node->getName(), "Jugador") == 0) {
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