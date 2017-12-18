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
#include "GestorColisiones.hpp"

using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif
#define TAMANYOCAJAS 10
//funciones
static void UpdatePhysics(u32 TDeltaTime);
static void UpdateRender(btRigidBody *TObject);
static btRigidBody *CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass);



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
	IrrlichtDevice  *device = m->getDevice();
    irrTimer = device->getTimer();
	//----------------------------//
	//---------BULLET-------------//
	//----------------------------//
	//inicializar mundo bullet
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	btBulletWorldImporter* fileLoader = new btBulletWorldImporter(mundo);
	mundo->setGravity(btVector3(0,-10,0));


	//Debug BUllet
	DebugDraw debugDraw(device);
	debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mundo->setDebugDrawer(&debugDraw);

	//-----------------------------//
	//-----ESCENARIO MAPA----------//
	//-----------------------------//

	Pista *pistaca = Pista::getInstancia();
	pistaca->setMapa("assets/carreteraprueba.obj","fisicas/carretera.bullet","assets/MapaPAth.obj");
	

	//-----------------------------//
	//-----GEOMETRIA COCHE---------//
	//-----------------------------//
	//Posicion del nodo y el bloque de colisiones centralizado:
	vector3df pos(0,20,20);
	CorredorJugador *pj1 = new CorredorJugador("assets/coche.obj", pos);
	pj1->InicializarFisicas();
	///////////////////////CAMARA///////////////////////////////////////////////
	Camara3persona *camara = new Camara3persona(smgr);

	btVector3 cubopos1(0,20,40);
	vector3df cuboescala1(5,5,5);

	//CreateBox(cubopos1,cuboescala1,10);



	//----------------------------//
	//---------OBJETOS------------//
	//----------------------------//
	int id =1;

	btVector3 posObj2(0,10,50);
	vector3df tamObj2(5.f, 20.f, 20.f);
	btRigidBody *obje2 = CreateBox(posObj2, tamObj2, 100000);
	ISceneNode *nodoObj2 = static_cast<ISceneNode *>(obje2->getUserPointer());
	//El problema esta en que estas variables no cambian las variables de obje2
	nodoObj2->setID(id);
	nodoObj2->setName("Destruible");
	id++;
	//---------------------------//
	//----------TURBO------------//
	//---------------------------//
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();
	IMeshSceneNode *turbo = smgr->addCubeSceneNode(5.0f);
	vector3df escala = vector3df(2.5f,0.0f,1.25f);
	turbo->setScale(escala);
	turbo->setMaterialFlag(EMF_LIGHTING, false);
	turbo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	turbo->setName("Turbo");
	turbo->setID(id);
	id++;
		smgr->getMeshManipulator()->setVertexColors(turbo->getMesh(), SColor(255, 255, 0, 0));
	//Node->setMaterialTexture(0, m->getDriver()->getTexture("assets/rust.png"));
	
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(0.0f,-4.5f,50.0f));
	

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(escala.X*2,escala.Y,escala.Z*2);
	btCollisionShape *Shape = new btBoxShape(HalfExtents);

	// Add mass
	btVector3 LocalInertia;
	Shape->calculateLocalInertia(0, LocalInertia);

	// Create the rigid body object
	btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

	RigidBody->setActivationState(DISABLE_DEACTIVATION);
	// Store a pointer to the irrlicht node so we can update it later
	RigidBody->setUserPointer((void *)(turbo));

	// Add it to the world
	mundo->addRigidBody(RigidBody);
	objetos.push_back(RigidBody);
	bullet->setObjetos(objetos);
	

//-----------------------------------------------------------------------------------------------------------------------------------------//
	Proyectil *item;
	//btRigidBody *obje1=CreateBox(cubopos1,cuboescala1,10);

	//
	// Caja destruible WIP
	//
	//cout<<"------------->"<<pj1->getNodo()->getPosition().Z<<endl;
	//cubopos1 = vector3df(std::stof(pj1->getNodo()->getPosition().X),std::stof(pj1->getNodo()->getPosition().Y), std::stof(pj1->getNodo()->getPosition().Z+5));
	//vector3df tamObj2(5.f, 20.f, 20.f);
	//CreateBox(cubopos1,cuboescala1,10);
	//ISceneNode *nodoObj2 = static_cast<ISceneNode *>(obje2->getUserPointer());
	////El problema esta en que estas variables no cambian las variables de obje2
	//nodoObj2->setID(id);
	//nodoObj2->setName("Destruible");
	//id++;

	//----------
	// Cajas de municion
	//----------
	btRigidBody* rigidCaja;
	Caja** cajas;
	cajas = new Caja*[TAMANYOCAJAS];
	vector3df posCaja(5.f, 10.f, -100.f);
	for (int i=0; i<10; i++){
		posCaja.Z+=10;
		cajas[i]= new Caja(m,posCaja,id);
		rigidCaja = cajas[i]->inicializarFisicas();
		mundo->addRigidBody(rigidCaja);
		objetos.push_back(rigidCaja);
		bullet->setObjetos(objetos);
		id++;
	}
//

	
	//----------------------------//
	//------GESTOR COLISIONES------//
	//----------------------------//

	GestorColisiones *colisiones= new GestorColisiones();


//-----------------------------------------------------------------------------------------------------------------------------------------//
	// -----------------------------//  INTERFAZ
	// -----------------------------
	stringw text = L"Datos del jugador:\n"; // PARA MODIFICACIONES FUTURAS
	IGUIFont *fuente = guienv->getFont("assets/fuente.bmp");
	IGUIStaticText *textoUI = guienv->addStaticText(
		text.c_str(),				 // Texto
		rect<s32>(10, 10, 260, 300), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,					 // Id del elemento
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
			
		
			DeltaTime = irrTimer->getTime() - TimeStamp;
			TimeStamp = irrTimer->getTime();
			UpdatePhysics(DeltaTime);
			pj1->movimiento();
			pj1->actualizarItem(item,id);
			pj1->actualizarRuedas();
			camara->moveCameraControl(pj1,device);
			colisiones->ComprobarColisiones(pj1, cajas, item);

/*
			//text += pj1->toString().c_str();

			text += "\n ---- CORREDOR 2 IA ----\n";
			text += " Waypoint siguiente: ";
			text += pj2->getNombreWaypoint().c_str();
			text += pj2->toString().c_str();


			//-------ENTRADA TECLADO ----------//
			if (teclado->isKeyDown(KEY_KEY_R)) {
				pj2->movimiento();
			}

			if (teclado->isKeyDown(KEY_ESCAPE))
			{
				m->cerrar();
				return 0;
			}

*/
			
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

			textoUI->setText(text.c_str());


			//	RENDER
			m->dibujar();

			SMaterial debugMat;
			debugMat.Lighting = true;
			driver->setMaterial(debugMat);
			driver->setTransform(ETS_WORLD, IdentityMatrix);
			mundo->debugDrawWorld();
				guienv->drawAll();

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



void UpdatePhysics(u32 TDeltaTime) {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();
	mundo->stepSimulation(TDeltaTime * 0.001f, 60);
	int d=0;
	for(list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator) {
		d++;
		cout<<"NUM OBJETOS= "<<d<<endl;
		UpdateRender(*Iterator);
	}	
}
// Passes bullet's orientation to irrlicht
void UpdateRender(btRigidBody *TObject) {
	
	ISceneNode *Node = static_cast<ISceneNode *>(TObject->getUserPointer());

	//cout << Node->getName() << endl;
	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();
	
	//btTransform t;
	//TObject->getMotionState()->getWorldTransform(t);	
	//Node->setPosition(vector3df(t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ()));
	if(strcmp(Node->getName(),"Jugador") == 0)
	Node->setPosition(vector3df((f32)Point[0],(f32)Point[1]+1,(f32)Point[2]));
	else
	Node->setPosition(vector3df((f32)Point[0],(f32)Point[1],(f32)Point[2]));
	// Set rotation
	vector3df Euler;
	const btQuaternion& TQuat = TObject->getOrientation();
	quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;
	Node->setRotation(Euler);

}


btRigidBody *CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass) {

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	Motor3d *m = Motor3d::getInstancia();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	
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
	bullet->setObjetos(objetos);

	return RigidBody;
}