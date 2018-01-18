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
#include "TextoPantalla.hpp"
#include "Gui.hpp"


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

int main()
{
	CTeclado *teclado = CTeclado::getInstancia();

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

	int id = 999;// estaba int id = 0; . Se cambia a 999 para evitar posibles conflictos con ids 0 creadas en mapa 
	vector3df pos(0, 10, 300);
	vector3df pos2(0, 10, 320);
	CorredorJugador **pj= new CorredorJugador*[2];
	pj[0] = new CorredorJugador("assets/coche.obj", pos);
	pj[1] = new CorredorJugador("assets/coche.obj", pos2);

	pj[0]->getNodo()->setID(id);
	id++;
	pj[1]->getNodo()->setID(id);


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
	//Gui *interfaz = new Gui(m->getDevice());
	
	
	// -----------------------------//
	// ----------GAME LOOP----------//
	// -----------------------------//
	driver->beginScene(true, true, video::SColor(255, 32, 223, 255));

	while (m->getDevice()->run())
	{
		if (m->getDevice()->isWindowActive())
		{

			textoDebug->limpiar();

			DeltaTime = irrTimer->getTime() - TimeStamp;
			TimeStamp = irrTimer->getTime();
			UpdatePhysics(DeltaTime);

			for (int i = 0; i < pistaca->getTamCajas(); i++)
			{
				pistaca->getArrayCaja()[i]->comprobarRespawn();
			}
			//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());


			pj[0]->actualizarItem();

			camara->moveCameraControl(pj[0], device);
			colisiones->ComprobarColisiones(pj);//esto deberia sobrar, puesto que las cajas ya no estan aqui, si no en pista
			//colisiones->ComprobarColisiones(pj1, pistaca->getArrayCaja());//deberia ser asi, pero CORE DUMPED

			pj[0]->update();
			//pj[1]->update();


			textoDebug->agregar("\n ---- CORREDOR 1 JUGADOR ----\n");
			textoDebug->agregar(pj[0]->toString());



			if (teclado->isKeyDown(KEY_ESCAPE))
			{
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
			//CEGUI::System::getSingleton().renderAllGUIContexts();
			
			driver->endScene();
		}
		else
		{
			m->getDevice()->yield();
		}
	}
	//----------------------------------//
	//-----------DESTRUCTORES-----------//
	//----------------------------------//
	
	for (int i = 0; i < 2; i++) {
		delete pj[i];
	}
	delete pj;
	delete pistaca;
	delete bullet;
	//delete camara;
	//delete colisiones;
	//m->getDevice()->drop();//irrlicht
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
	ISceneNode *Node = static_cast<ISceneNode *>(TObject->getUserPointer());
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

