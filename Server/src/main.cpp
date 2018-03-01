#include <ctime>

#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "DebugFisicas.hpp"
#include "Camara3persona.hpp"

#include "Pista.hpp"
#include "GestorJugadores.hpp"
#include "GestorCarrera.hpp"
#include "GestorColisiones.hpp"
#include <vector>
#include "Graphics.hpp"
#include "Server.hpp"
#include "Timer.hpp"


const int W_WIDTH = 1024;
const int H_WIDTH = 600;


	void UpdatePhysics(u32 TDeltaTime);
	void UpdateRender(btRigidBody *TObject);

int main()
{
    int x = 0;

	int debug;

	DebugDraw *debugDraw;

	Camara3persona *camara;
	int tipoCamara;
	bool cambioCamara;
	GestorColisiones *colisiones;

	int lastFPS;
	u32 TimeStamp;
	u32 DeltaTime;

	Server *server;
	vector3df pos;

	clock_t tiempoRefresco;
    clock_t tiempoRefrescoServer;
	GestorCarrera *gc;
	bool fin_carrera;
    std::string mensaje;
    //RED

    server = new Server(8);
    server->CreateServerInterface();
    server->ServerStartup();
    
    
    debug = 0;
	fin_carrera=false;
    GestorIDs::instancia().restartID();

	Timer *t = Timer::getInstancia(); 
  	
    GestorIDs::instancia().restartID(); 

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
	tiempoRefrescoServer = clock();
    // -----------------------------
    //  GAME LOOP
    // -----------------------------
    RakSleep(30);
	bool checkInit=false;
    while (Motor3d::instancia().getDevice()->run())
    {
		
        server->ReceivePackets();
		
        //========================================================
        //EMPIEZA UPDATE
        //========================================================
    
        TextoPantalla *textoDebug = TextoPantalla::getInstancia();
        Pista *pistaca = Pista::getInstancia();
        vector<Item *> items = pistaca->getItems();
        jugadores = GestorJugadores::getInstancia();
        vector<Corredor*> pj = jugadores->getJugadores();

		//Comprobamos si el servidor ha iniciado la partida (para iniciar el contador)
        if (server->getStarted()){
			if (checkInit==false){
				t->restartTimer(); 
				checkInit=true;
			}
			if (t->getTimer()<=3 && t->getTimer()>=1){ 
				int desc=4-t->getTimer(); 
				textoDebug->agregar(to_string(desc)); 
			} 
			if (t->getTimer()==4){ 
				for (int i = 0; i < jugadores->getNumJugadores(); i++) { 
				pj.at(i)->getEstados()->setEstadoCarrera(1); 
				} 
			} 
		}
        //cout << irrTimer->getTime() << endl;
        textoDebug->limpiar();

        DeltaTime = Motor3d::instancia().getDevice()->getTimer()->getTime() - TimeStamp;
        TimeStamp = Motor3d::instancia().getDevice()->getTimer()->getTime();
        UpdatePhysics(DeltaTime);

        for (int i = 0; i < pistaca->getTamCajas(); i++) {
            pistaca->getArrayCaja()[i]->comprobarRespawn(); // TODO: MOVER AL UPDATE DE PISTACA
        }

        for (int i=0;i<items.size();i++)
        {
            if(items.at(i)->getLanzado()){
                if(items.at(i)->comprobarDestructor()){
                    items.at(i)->Delete();
                    items.erase(items.begin()+i);
                    break;
                }
            }
        }
        pistaca->setItems(items);

        if (fin_carrera){
            textoDebug->agregar("CARRERA FINALIZADA, PULSA F.");
        }
        //colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());
        pj = jugadores->getJugadores();

        camara->moveCameraControlServer();
        
        colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
                                            //colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED
        if (jugadores->getNumJugadores() != 0){
            for (int i = 0; i < jugadores->getNumJugadores(); i++) {
                pj.at(i)->update();
            }
        }


        if (jugadores->getNumJugadores() != 0) {
            clock_t tiempoActual = clock();
            clock_t timediff = tiempoActual - tiempoRefresco;
            float timediff_sec = ((float)timediff) / 100000;
            if (timediff_sec >= 0.01) {
                //client->PlayerMovement();
                tiempoRefresco = clock();
            }
            //client->PlayerAction();
            //client->UpdateNetworkKeyboard();
        }     
        clock_t tiempoActual = clock();
        clock_t timediff = tiempoActual - tiempoRefrescoServer;
        float timediff_sec = ((float)timediff) / 100000;
        if (timediff_sec >= 0.01) {
            cout<<"REFRESCO\n";
            server->refreshServer();
            tiempoRefrescoServer = clock();
        }
        if(server->getCommands()==1) return 0;
        

        if (jugadores->getNumJugadores() != 0){
            if (gc->update())
                fin_carrera=true;
        }


        jugadores->setJugadores(pj);

        int fps = Motor3d::instancia().getDriver()->getFPS();
        if (lastFPS != fps) {
            core::stringw tmp(L"Age of karts SERVER [");
            tmp += Motor3d::instancia().getDriver()->getName();
            tmp += L"] fps: ";
            tmp += fps;

            Motor3d::instancia().getDevice()->setWindowCaption(tmp.c_str());
            lastFPS = fps;
        }

        //========================================================
        //ACABA UPDATE
        //========================================================
        //========================================================
        //EMPIEZA DRAW
        //========================================================

        jugadores = GestorJugadores::getInstancia();
        pj = jugadores->getJugadores();

        //------- RENDER ----------
        Motor3d::instancia().iniciarDibujado();
        Motor3d::instancia().getScene()->drawAll();

        //Todo lo que se quiera dibujar debe ir aqui abajo por la iluminacion
        SMaterial materialDriver;
        materialDriver.Lighting = false;
        Motor3d::instancia().getDriver()->setTransform(video::ETS_WORLD, core::matrix4());
        Motor3d::instancia().getDriver()->setMaterial(materialDriver);
        
        //Para poder dibujar lineas debug
        if (debug) {
            SMaterial debugMat;
            debugMat.Lighting = true;
            Motor3d::instancia().getDriver()->setMaterial(debugMat);
            Motor3d::instancia().getDriver()->setTransform(ETS_WORLD, IdentityMatrix);
            MotorFisicas::getInstancia()->getMundo()->debugDrawWorld();
        }

        Motor3d::instancia().getGUI()->drawAll();
        Motor3d::instancia().terminarDibujado();
        //========================================================
        //ACABA DRAW
        //========================================================
    
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && Motor3d::instancia().getDevice()->isWindowActive()){
            server->ShutDownServer();
            Motor3d::instancia().getDevice()->drop();
            return 0;
        }
    }
    server->ShutDownServer();

    return 0;
}


void UpdatePhysics(u32 TDeltaTime) {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	vector<btRigidBody *> objetos = bullet->getObjetos();
	mundo->stepSimulation(TDeltaTime * 0.001f, 30);
	int c = 0;
	for (int i=0;i<objetos.size();i++){
		c++;
		UpdateRender(objetos.at(i));
	}
}

// Passes bullet's orientation to irrlicht
void UpdateRender(btRigidBody *TObject) {
	IMeshSceneNode *Node = static_cast<IMeshSceneNode *>(TObject->getUserPointer());
	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();

	Pista *mapa = Pista::getInstancia();
	//btTransform t;
	//TObject->getMotionState()->getWorldTransform(t);	
	//Node->setPosition(vector3df(t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ()));
	if (strcmp(Node->getName(), "Jugador") == 0 || strcmp(Node->getName(), "JugadorIA") == 0 || strcmp(Node->getName(), "JugadorRed") == 0) {
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