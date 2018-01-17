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
#include "Client.hpp"

using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#define TAMANYOCAJAS 10

//funciones
static void UpdatePhysics(u32 TDeltaTime);
static void UpdateRender(btRigidBody *TObject);

static btRigidBody *CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass, int id);

static core::list<btRigidBody *> objetosm;
static ITimer *irrTimer;
static ILogger *irrLog;

int main()
{
	CTeclado *teclado = CTeclado::getInstancia();

	Client *client;
	client = new Client(8);
	client->CreateClientInterface();
	client->SetIP();
	client->ClientStartup();

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
	btBulletWorldImporter *fileLoader = new btBulletWorldImporter(mundo);
	mundo->setGravity(btVector3(0, -25, 0));

	//Debug BUllet
	DebugDraw debugDraw(device);
	debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mundo->setDebugDrawer(&debugDraw);

	//-----------------------------//
	//-----ESCENARIO MAPA----------//
	//-----------------------------//

	Pista *pistaca = Pista::getInstancia();
	//cout << " voy a criar el mapa" << endl;
	pistaca->setMapa("assets/Mapa01/mapaIsla.obj", "assets/Mapa01/FisicasMapaIsla.bullet", "assets/Mapa01/WPTrbBox2.obj");

	pistaca->getArrayWaypoints();

	//-----------------------------//
	//-----GEOMETRIA COCHE---------//
	//-----------------------------//
	//Posicion del nodo y el bloque de colisiones centralizado:

	int id = 999;// estaba int id = 0; . Se cambia a 999 para evitar posibles conflictos con ids 0 creadas en mapa 
	vector3df pos(0, 0, 300);
	vector3df pos2(0, 0, 320);
	CorredorJugador **pj= new CorredorJugador*[2];
	pj[0] = new CorredorJugador("assets/coche.obj", pos);
	pj[1] = new CorredorJugador("assets/coche.obj", pos2);

	pj[0]->getNodo()->setID(id);
	id++;
	pj[1]->getNodo()->setID(id);

	//CorredorIA *pj2 = new CorredorIA("assets/coche.obj", pos);
	//pj2->InicializarFisicas();
	//pj2->getNodo()->setID(id);
	///////////////////////CAMARA///////////////////////////////////////////////
	Camara3persona *camara = new Camara3persona();

	/*btVector3 cubopos1(0, 20, 40);
	vector3df cuboescala1(5, 5, 5);
	id++;*/
	//CreateBox(cubopos1,cuboescala1,10);

	//----------------------------//
	//---------OBJETOS------------//
	//----------------------------//
	//---------------------------------------------------------------------------------------DESDE AQUI 
	/*btVector3 posObj2(0, 10, 70);
	vector3df tamObj2(5.f, 20.f, 20.f);
	btRigidBody *obje2 = CreateBox(posObj2, tamObj2, 100000, id);
	//ISceneNode *nodoObj2 = static_cast<ISceneNode *>(obje2->getUserPointer());
	////El problema esta en que estas variables no cambian las variables de obje2
	////nodoObj2->setID(id);
	////nodoObj2->setName("Destruible");
	id++;*/

	//irr::core::list<btRigidBody *> objetos = bullet->getObjetos();


	//----------
	// Cajas de municion
	//----------
	//Ha pasado a mejor vida

	//---------------------------------------------------------------------------------------HASTA AQUI ( si se borra el turbo y las cajas no funcionan, cosa que no entiendo porque las cajas ya no las creo aqui)
	//----------------------------//
	//------GESTOR COLISIONES-----//
	//----------------------------//

	GestorColisiones *colisiones = new GestorColisiones();
	TextoPantalla *textoDebug = TextoPantalla::getInstancia();

	int lastFPS = -1;
	u32 TimeStamp = irrTimer->getTime(), DeltaTime = 0;
	// -----------------------------//
	// ----------GAME LOOP----------//
	// -----------------------------//
	driver->beginScene(true, true, video::SColor(255, 32, 223, 255));

	while (m->getDevice()->run())
	{
		client->ReceivePackets(smgr);
		client->SpawnPlayer(smgr);

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

		SMaterial debugMat;
		debugMat.Lighting = true;
		driver->setMaterial(debugMat);
		driver->setTransform(ETS_WORLD, IdentityMatrix);
		if (debug) {
			mundo->debugDrawWorld();
		}
		guienv->drawAll();
		driver->endScene();
	
	}

	m->getDevice()->drop();

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

btRigidBody *CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass, int id)
{

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	Motor3d *m = Motor3d::getInstancia();
	core::list<btRigidBody *> objetos = bullet->getObjetos();

	ISceneNode *Node = m->getScene()->addCubeSceneNode(1.0f);
	Node->setScale(TScale);
	//----------------------------------
	//Chapuza momentanea para solucionar el core dumped al colisionar con proyectil
	Node->setName("Destruible");
	Node->setID(2);
	//----------------------------------
	Node->setMaterialFlag(EMF_LIGHTING, 1);

	Node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, m->getDriver()->getTexture("assets/rust.png"));
	Node->setName("Destruible");
	Node->setID(id);
	Node->setMaterialTexture(0, m->getDriver()->getTexture("assets/textures/rust.png"));
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(TPosition);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	btCollisionShape *Shape = new btBoxShape(HalfExtents);

	// Add mass
	btVector3 LocalInertia;
	Shape->calculateLocalInertia(TMass, LocalInertia);

	// Create the rigid body object
	btRigidBody *RigidBody = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

	RigidBody->setActivationState(DISABLE_DEACTIVATION);
	// Store a pointer to the irrlicht node so we can update it later
	RigidBody->setUserPointer((void *)(Node));

	// Add it to the world
	mundo->addRigidBody(RigidBody);
	objetos.push_back(RigidBody);
	bullet->setObjetos(objetos);

	return RigidBody;
}
