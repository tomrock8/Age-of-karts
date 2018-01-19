#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
#include "Corredor.hpp"
#include "CorredorIA.hpp"
#include "CorredorJugador.hpp"
#include "Waypoint.hpp"
#include "Pista.hpp"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Camara3persona.hpp"
#include "DebugFisicas.hpp"
#include "BulletWorldImporter/btBulletWorldImporter.h"
#include "Proyectil.hpp"
#include "Caja.hpp"
#include "Item.hpp"
#include "GestorColisiones.hpp"
#include "GestorJugadores.hpp"
#include "TextoPantalla.hpp"
#include "Gui.hpp"
#include "Client.hpp"

using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#define TAMANYOCAJAS 10

//funciones
static void UpdatePhysics(u32 TDeltaTime);
static void UpdateRender(btRigidBody *TObject);


static core::list<btRigidBody *> objetosm;
static ITimer *irrTimer;
static ILogger *irrLog;

int main(int argc, char* argv[])
{
	CTeclado *teclado = CTeclado::getInstancia();

	Client *client = NULL;

	if (argc == 2) {
		client = new Client(8);
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
	IGUIEnvironment *guienv = m->getGUI();
	IrrlichtDevice *device = m->getDevice();
	irrTimer = device->getTimer();



	int debug = 0;

	//----------------------------//
	//---------BULLET-------------//
	//----------------------------//
	//inicializar mundo bullet
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	mundo->setGravity(btVector3(0, -25, 0));
	//----------------------------//
	//--------Debug Bullet--------//
	//----------------------------//
	DebugDraw debugDraw(device);
	debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mundo->setDebugDrawer(&debugDraw);
	//-----------------------------//
	//-----ESCENARIO MAPA----------//
	//-----------------------------//
	Pista *pistaca = Pista::getInstancia();
	pistaca->setMapa("assets/Mapa01/mapaIsla.obj", "assets/Mapa01/FisicasMapaIsla.bullet", "assets/Mapa01/WPTrbBox2.obj");
	pistaca->getArrayWaypoints();
	//-----------------------------//
	//-----GEOMETRIA COCHE---------//
	//-----------------------------//
	//Posicion del nodo y el bloque de colisiones centralizado:

	vector3df pos(0, 1, 300);
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	Corredor **pj = jugadores->getJugadores();

	//pj[1]->getNodo()->setID(id);


	//-----------------------------//
	//-------------CAMARA----------//
	//-----------------------------//
	Camara3persona *camara = new Camara3persona();
	//-----------------------------//
	//------GESTOR COLISIONES------//
	//-----------------------------//

	GestorColisiones *colisiones = new GestorColisiones();
	TextoPantalla *textoDebug = TextoPantalla::getInstancia();
	//-----------------------------//
	//------------TIME-------------//
	//-----------------------------//
	int lastFPS = -1;
	u32 TimeStamp = irrTimer->getTime(), DeltaTime = 0;



	// -----------------------------
	//  CEGUI
	// -----------------------------
	Gui *interfaz = new Gui(device);
	bool res = interfaz->cargarScheme("AlfiskoSkin.scheme");
	if (res) {
		interfaz->setFuente("DejaVuSans-10");

		float tam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float escala[4] = { 0.5f, 0.5f, 0.1f, 0.05f };

		CEGUI::Window *botonPrueba = interfaz->crearWidget("AlfiskoSkin/Button", escala, tam, "TestLabel");
		if (botonPrueba)
			botonPrueba->setText("TOWAPOTIO");
	}



	// -----------------------------//
	// ----------GAME LOOP----------//
	// -----------------------------//
	driver->beginScene(true, true, video::SColor(255, 32, 223, 255));

	while (m->getDevice()->run())
	{

		if (argc == 2) {
			client->ReceivePackets(smgr);
			client->SpawnPlayer(smgr);
		}

		textoDebug->limpiar();

		DeltaTime = irrTimer->getTime() - TimeStamp;
		TimeStamp = irrTimer->getTime();
		UpdatePhysics(DeltaTime);

		for (int i = 0; i < pistaca->getTamCajas(); i++)
		{
			pistaca->getArrayCaja()[i]->comprobarRespawn();
		}
		//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());

		pj = jugadores->getJugadores();
		pj[0]->actualizarItem();

		camara->moveCameraControl(pj[0], device);
		colisiones->ComprobarColisiones();//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
		//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED

		pj[0]->update();
		//pj[1]->update();


		textoDebug->agregar("\n ---- CORREDOR 1 JUGADOR ----\n");
		textoDebug->agregar(pj[0]->toString());

		if (teclado->isKeyDown(KEY_KEY_R)) {
			btVector3 btPos(pos.X, pos.Y, pos.Z);
			/*btRigidBody *btRi = pj[0]->getRigidBody();
			btTransform trans;
			btRi->getMotionState()->getWorldTransform(trans);
			trans.setOrigin(*btPos);
			btRi->getMotionState()->setWorldTransform(trans);
*/
			btTransform trans;
			trans.setOrigin(btPos);
			btQuaternion quaternion;
			quaternion.setEulerZYX(pj[0]->getNodo()->getRotation().Z* PI / 180, pj[0]->getNodo()->getRotation().Y * PI / 180, pj[0]->getNodo()->getRotation().X* PI / 180);
			trans.setRotation(quaternion);

			pj[0]->getRigidBody()->setCenterOfMassTransform(trans);
			//pj[0]->getNodo()->setPosition(pos);
		}

		jugadores->setJugadores(pj);

		//textoDebug->agregar("\n\n ---- CORREDOR 2 IA ----\n");
		//textoDebug->agregar(pj2->toString());

		//-------ENTRADA TECLADO ----------//
		/*
		if (teclado->isKeyDown(KEY_KEY_R))
		{
			pj2->movimiento();
		}
		*/

		if (teclado->isKeyDown(KEY_ESCAPE))
		{
			if (argc == 2)
				client->ShutDownClient();
			m->cerrar();
			return 0;
		}

		if (teclado->isKeyDown(KEY_KEY_0))
		{
			debug = 0;
		}
		if (teclado->isKeyDown(KEY_KEY_9))
		{
			debug = 1;
		}

		//-------ENTRADA TECLADO FIN----------//
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw tmp(L"Age of karts [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			m->getDevice()->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
		//	RENDER
		m->dibujar();


		if (debug) {
			SMaterial debugMat;
			debugMat.Lighting = true;
			driver->setMaterial(debugMat);
			driver->setTransform(ETS_WORLD, IdentityMatrix);
			mundo->debugDrawWorld();
		}
		guienv->drawAll();
		// draw gui
		CEGUI::System::getSingletonPtr()->renderAllGUIContexts();
		//m->getInterfaz()->dibujar();
		driver->endScene();

	}
	//----------------------------------//
	//-----------DESTRUCTORES-----------//
	//----------------------------------//
	for (int i = 0; i < 6; i++) {
		delete pj[i];
	}
	delete pj;
	delete pistaca;
	delete bullet;
	//delete camara;
	//delete colisiones;
	delete interfaz;
	delete m;

	return 0;
}

void UpdatePhysics(u32 TDeltaTime)
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();
	mundo->stepSimulation(TDeltaTime * 0.001f, 60);
	int c = 0;
	for (list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator)
	{
		c++;

		UpdateRender(*Iterator);
	}
}
// Passes bullet's orientation to irrlicht
void UpdateRender(btRigidBody *TObject)
{
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

