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

using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//funciones
static void UpdatePhysics(u32 TDeltaTime);
static void UpdateRender(btRigidBody *TObject);
static void CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass);



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
	IrrlichtDevice  *device = m->getDevice();
    irrTimer = device->getTimer();
	//----------------------------//
	//---------BULLET-------------//
	//----------------------------//
	//inicializar mundo bullet
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	btBulletWorldImporter* fileLoader = new btBulletWorldImporter(mundo);
	fileLoader->loadFile("fisicas/carretera.bullet");
	mundo->setGravity(btVector3(0,-10,0));


	//Debug BUllet
	DebugDraw debugDraw(device);
	debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mundo->setDebugDrawer(&debugDraw);

	//-----------------------------//
	//-----ESCENARIO MAPA----------//
	//-----------------------------//
	//smgr->addLightSceneNode(0,core::vector3df(2,5,-2), SColorf(4,4,4,1));//luz para experimentos nazis
	//vector3df escala(1,1,1);
	//vector3df posicion(0,0,0);
	//Pista *Mapa1= new Pista(posicion,escala);
	//Mapa1->InicializarFisicas(objetos,mundo);
	//if (!Mapa1){
	//	return 1;		//error no se ha cargado el mapa
	//}
	
	ISceneNode *Mapa1 = smgr->addMeshSceneNode(smgr->getMesh("assets/carreteraprueba.obj"));
	if(Mapa1) {
		Mapa1->setMaterialFlag(EMF_LIGHTING, false);
	}

	//-----------------------------//
	//-----GEOMETRIA COCHE---------//
	//-----------------------------//
	//Posicion del nodo y el bloque de colisiones centralizado:
	vector3df pos(0,20,20);
	CorredorJugador *pj1 = new CorredorJugador("assets/coche.obj", pos);
	pj1->InicializarFisicas(objetos,mundo);
	///////////////////////CAMARA///////////////////////////////////////////////
	Camara3persona *camara = new Camara3persona(smgr);

	btVector3 cubopos1(240,20,10);
	vector3df cuboescala1(5,5,5);

	CreateBox(cubopos1,cuboescala1,10);

	// ----------------------------//
	// ---------Waypoints----------//
	// ----------------------------//
//lectura de fichero

std::string line;

int j;
std::string wX, wY, wZ;
int tamanyoArrayWaypoints = 0;
Waypoint **arrayWaypoints;

  ifstream myfile ("assets/MapaPAth.obj");
  if (myfile.is_open())
  {
	 getline(myfile, line); 
	
			//crear el array de waypoints para almacenar el path
			tamanyoArrayWaypoints = stoi(line);
			arrayWaypoints = new Waypoint *[tamanyoArrayWaypoints];	
			//se crea un array con las posiciones de los waypoints que se recogeran del fichero

			  for (int j = 0 ;j<tamanyoArrayWaypoints; j++){
			  		cout<<"cuanto tengo: "<<tamanyoArrayWaypoints<<"\n";
					//seteamos los Waypoins
					arrayWaypoints[j] = new Waypoint();
					arrayWaypoints[j]->setNombre(std::to_string(j));
					if(j ==0){//si es el primero apuntara al ultimo
						arrayWaypoints[j]->setSiguiente(arrayWaypoints[tamanyoArrayWaypoints-1]);
					}
					else if(j==tamanyoArrayWaypoints-2){//si es el ultimo apuntara al primero
							arrayWaypoints[j]->setSiguiente(arrayWaypoints[0]);
					} else arrayWaypoints[j]->setSiguiente(arrayWaypoints[j+1]);
					getline(myfile, wX, ' ');
					getline(myfile, wY, ' ');
					getline(myfile, wZ);
					//cambiar a float y almacenar array de waypoints
					arrayWaypoints[j]->setPosicion(std::stof(wX),std::stof(wY),std::stof(wZ));
					//incrementar la j para los waypoints
					
					cout << "x: " << std::stof(wX) <<"y: "<<std::stof(wY)<<"z: "<< std::stof(wZ) << '\n';
		}

   
    myfile.close();
  }

  else cout << "Error abriendo archivo";	

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

			pj1->actualizarRuedas();
			camara->moveCameraControl(pj1,device);

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
	mundo->stepSimulation(TDeltaTime * 0.001f, 60);

	for(list<btRigidBody *>::Iterator Iterator = objetos.begin(); Iterator != objetos.end(); ++Iterator) {
		
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


void CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass) {

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
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