#include <iostream>
#include <string>

#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
#include "Corredor.hpp"
#include "CorredorIA.hpp"
#include "CorredorJugador.hpp"
#include "Waypoint.hpp"
#include "Pista.hpp"
#include "Motor3d.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Camara3persona.hpp"
#include "DebugFisicas.hpp"

using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//funciones
static void UpdatePhysics(u32 TDeltaTime);
static void UpdateRender(btRigidBody *TObject);
static void CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass);

static btDiscreteDynamicsWorld *mundo;
static core::list<btRigidBody *> objetos;
static ITimer *irrTimer;
static ILogger *irrLog;

int main()
{
	CTeclado *teclado = CTeclado::getInstancia();

	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	Motor3d *m = Motor3d::getInstancia();
	//m->setTeclado(teclado);

	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();
	IGUIEnvironment *guienv = m->getGUI();
	IrrlichtDevice *device = m->getDevice();
	irrTimer = device->getTimer();
	//----------------------------//
	//---------BULLET-------------//
	//----------------------------//

	//inicializar mundo bullet
	btBroadphaseInterface *broadPhase = new btAxisSweep3(btVector3(-1000, 1000, -1000), btVector3(1000, 1000, 1000)); //limites del mundo
	btDefaultCollisionConfiguration *confColision = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(confColision);
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
	mundo = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, confColision); //creacion del mundo
	mundo->setGravity(btVector3(0, -15, 0));

	//----------------------------//
	//---------DEBUG BULLET-------//
	//----------------------------//
	DebugDraw debugDraw(device);
	debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mundo->setDebugDrawer(&debugDraw);

	//-----------------------------//
	//-----ESCENARIO MAPA----------//
	//-----------------------------//
	smgr->addLightSceneNode(0, core::vector3df(2, 5, -2), SColorf(4, 4, 4, 1)); //luz para experimentos nazis
	vector3df escala(1, 1, 1);
	vector3df posicion(0, 0, 0);
	Pista *Mapa1 = new Pista(posicion, escala);
	Mapa1->InicializarFisicas(objetos, mundo);
	if (!Mapa1)
	{
		return 1; //error no se ha cargado el mapa
	}

	//-----------------------------//
	//-----------JUGADORES---------//
	//-----------------------------//
	//Posicion del nodo y el bloque de colisiones centralizado:
	vector3df pos(230, -50, 0);
	CorredorJugador *pj1 = new CorredorJugador("assets/coche.obj", pos);
	pj1->InicializarFisicas(objetos, mundo);
	///////////////////////CAMARA///////////////////////////////////////////////
	Camara3persona *camara = new Camara3persona(smgr);

	btVector3 cubopos1(240, 20, 10);
	vector3df cuboescala1(5, 5, 5);

	//----------------------------//
	//---------OBJETOS------------//
	//----------------------------//

	CreateBox(cubopos1, cuboescala1, 10);

	// -----------------------------
	//	Waypoints
	// -----------------------------
	int tamanyoArrayWaypoints = 33;
	Waypoint **arrayWaypoints;
	arrayWaypoints = new Waypoint *[tamanyoArrayWaypoints];
	float posanteriorZ = 0;
	float posanteriorX = 0;
	float posY = -40;

	for (int i = 0; i < tamanyoArrayWaypoints; i++)
	{
		arrayWaypoints[i] = new Waypoint();
		arrayWaypoints[i]->setNombre(std::to_string(i));

		if (i > 0 && i <= (tamanyoArrayWaypoints - 2))
		{
			arrayWaypoints[i - 1]->setSiguiente(arrayWaypoints[i]);
		}
		else if (i == tamanyoArrayWaypoints - 1)
		{
			arrayWaypoints[i - 1]->setSiguiente(arrayWaypoints[i]);
			arrayWaypoints[i]->setSiguiente(arrayWaypoints[0]);
		}

		if (i == 0)
		{
			arrayWaypoints[0]->setPosicion(235, posY, 0);
		}

		else if (i > 0 && i < 12)
		{
			posanteriorZ = arrayWaypoints[i - 1]->getPosicion().Z;
			arrayWaypoints[i]->setPosicion(235, posY, posanteriorZ + 40);
		}
		else if (i >= 12 && i < 14)
		{
			posanteriorX = arrayWaypoints[i - 1]->getPosicion().X;
			posanteriorZ = arrayWaypoints[i - 1]->getPosicion().Z;
			//arrayWaypoints[i]->setPosicion(225,0, posanteriorZ + 30);
			arrayWaypoints[i]->setPosicion(posanteriorX - 15, posY, posanteriorZ + 30);
		}
	}

	arrayWaypoints[14]->setPosicion((arrayWaypoints[13]->getPosicion().X - 20), posY, (arrayWaypoints[13]->getPosicion().Z + 10));
	arrayWaypoints[15]->setPosicion((arrayWaypoints[14]->getPosicion().X - 30), posY, (arrayWaypoints[14]->getPosicion().Z + 10));
	arrayWaypoints[16]->setPosicion((arrayWaypoints[15]->getPosicion().X - 40), posY, (arrayWaypoints[15]->getPosicion().Z + 10));
	arrayWaypoints[17]->setPosicion((arrayWaypoints[16]->getPosicion().X - 50), posY, (arrayWaypoints[16]->getPosicion().Z + 5));
	arrayWaypoints[18]->setPosicion((arrayWaypoints[17]->getPosicion().X - 60), posY, (arrayWaypoints[17]->getPosicion().Z - 5));
	arrayWaypoints[19]->setPosicion((arrayWaypoints[18]->getPosicion().X - 70), posY, (arrayWaypoints[18]->getPosicion().Z - 50));
	arrayWaypoints[20]->setPosicion((arrayWaypoints[19]->getPosicion().X - 10), posY, (arrayWaypoints[19]->getPosicion().Z - 100));
	arrayWaypoints[21]->setPosicion((arrayWaypoints[20]->getPosicion().X), posY, (arrayWaypoints[20]->getPosicion().Z - 200));
	arrayWaypoints[22]->setPosicion((arrayWaypoints[21]->getPosicion().X), posY, (arrayWaypoints[21]->getPosicion().Z - 200));
	arrayWaypoints[23]->setPosicion((arrayWaypoints[22]->getPosicion().X), posY, (arrayWaypoints[22]->getPosicion().Z - 200));
	arrayWaypoints[24]->setPosicion((arrayWaypoints[23]->getPosicion().X), posY, (arrayWaypoints[23]->getPosicion().Z - 200));
	arrayWaypoints[25]->setPosicion((arrayWaypoints[24]->getPosicion().X + 10), posY, (arrayWaypoints[24]->getPosicion().Z - 55));
	arrayWaypoints[26]->setPosicion((arrayWaypoints[25]->getPosicion().X + 50), posY, (arrayWaypoints[25]->getPosicion().Z - 40));
	arrayWaypoints[27]->setPosicion((arrayWaypoints[26]->getPosicion().X + 60), posY, (arrayWaypoints[26]->getPosicion().Z - 25));
	arrayWaypoints[28]->setPosicion((arrayWaypoints[27]->getPosicion().X + 60), posY, (arrayWaypoints[27]->getPosicion().Z));
	arrayWaypoints[29]->setPosicion((arrayWaypoints[28]->getPosicion().X + 60), posY, (arrayWaypoints[28]->getPosicion().Z + 10));
	arrayWaypoints[30]->setPosicion((arrayWaypoints[29]->getPosicion().X + 30), posY, (arrayWaypoints[29]->getPosicion().Z + 20));
	arrayWaypoints[31]->setPosicion((arrayWaypoints[30]->getPosicion().X + 30), posY, (arrayWaypoints[30]->getPosicion().Z + 30));
	arrayWaypoints[32]->setPosicion((arrayWaypoints[31]->getPosicion().X + 10), posY, (arrayWaypoints[31]->getPosicion().Z + 60));

	vector3df posIA(220, -50, 0);
	CorredorIA *pj2 = new CorredorIA("assets/coche.obj", posIA, arrayWaypoints, tamanyoArrayWaypoints);
	pj2->InicializarFisicas(objetos, mundo);
	// -----------------------------
	//  INTERFAZ
	// -----------------------------
	stringw text = L"Datos del jugador:\n"; // PARA MODIFICACIONES FUTURAS
	IGUIFont *fuente = guienv->getFont("assets/fuente.bmp");
	IGUIStaticText *textoUI = guienv->addStaticText(
		text.c_str(),				 // Texto
		rect<s32>(10, 10, 260, 300), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,							 // Id del elemento
		true);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);

	int lastFPS = -1;
	u32 TimeStamp = irrTimer->getTime(), DeltaTime = 0;
	// -----------------------------
	//  GAME LOOP
	// -----------------------------
	while (m->getDevice()->run())
	{
		if (m->getDevice()->isWindowActive())
		{

			text = "";
			DeltaTime = irrTimer->getTime() - TimeStamp;
			TimeStamp = irrTimer->getTime();
			UpdatePhysics(DeltaTime);

			pj1->update();
			//pj2->movimiento();

			//pj2->actualizarRuedas();
			camara->moveCameraControl(pj1, device);

			text += pj1->toString().c_str();
			text += "\n ---- CORREDOR 2 IA ----\n";
			text += " Waypoint siguiente: ";
			text += pj2->getNombreWaypoint().c_str();
			text += pj2->toString().c_str();

			//-------ENTRADA TECLADO ----------//
			if (teclado->isKeyDown(KEY_KEY_R))
			{
				pj2->movimiento();
			}

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

			textoUI->setText(text.c_str());

			//	RENDER
			m->dibujar();

			SMaterial debugMat;
			debugMat.Lighting = true;
			driver->setMaterial(debugMat);
			driver->setTransform(ETS_WORLD, IdentityMatrix);
			mundo->debugDrawWorld();

			driver->endScene();
		}
		else
		{
			m->getDevice()->yield();
		}
	}

	m->getDevice()->drop();

	return 0;
}

void UpdatePhysics(u32 TDeltaTime)
{

	mundo->stepSimulation(TDeltaTime * 0.001f, 60);

	for (list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator)
	{

		UpdateRender(*Iterator);
	}
}
// Passes bullet's orientation to irrlicht
void UpdateRender(btRigidBody *TObject)
{

	ISceneNode *Node = static_cast<ISceneNode *>(TObject->getUserPointer());

	//cout << Node->getName() << endl;
	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();

	//btTransform t;
	//TObject->getMotionState()->getWorldTransform(t);
	//Node->setPosition(vector3df(t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ()));
	if (strcmp(Node->getName(), "Jugador") == 0)
		Node->setPosition(vector3df((f32)Point[0], (f32)Point[1] + 1, (f32)Point[2]));
	else
		Node->setPosition(vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));
	// Set rotation
	vector3df Euler;
	const btQuaternion &TQuat = TObject->getOrientation();
	quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;
	Node->setRotation(Euler);
}

void CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass)
{

	Motor3d *m = Motor3d::getInstancia();

	ISceneNode *Node = m->getScene()->addCubeSceneNode(1.0f);
	Node->setScale(TScale);
	Node->setMaterialFlag(EMF_LIGHTING, 1);
	Node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, m->getDriver()->getTexture("assets/rust.png"));

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
}