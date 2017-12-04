#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
#include "Corredor.hpp"
#include "Waypoint.hpp"
#include "IVentana.hpp"
#include <iostream>

using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Ids para asignar a cada elemento.
//  Seran analizados por el gestor de colisiones.
enum
{
	// No colisionable, para evitar cogerlo por error
	ID_NULO = 0,

	// Objetos que pueden colisionar
	ID_COLISION = 1 << 0,

	// Objetos para iluminar
	ID_ILUMINAR = 1 << 1
};

int main()
{

	CTeclado teclado;
	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	IVentana *irrlicht = new IVentana(teclado);
	IrrlichtDevice *device = irrlicht->getDevice();
	IVideoDriver *driver = irrlicht->getDriver();
	ISceneManager *smgr = irrlicht->getScene();
	IGUIEnvironment *guienv = irrlicht->getGUI();

	//COLISIONES
	ITriangleSelector *selector = 0; //Selector de triangulos para las colisiones
	ISceneCollisionManager *gestorColisiones = smgr->getSceneCollisionManager();

	// -----------------------------
	//  GEOMETRIA COCHE
	// -----------------------------
	Corredor *pj1 = new Corredor(smgr, "assets/coche.obj", ID_COLISION);
	//pj1->escalar(5.0f);
	IMeshSceneNode *Jugador = pj1->getNodo();
	Corredor *pj2 = new Corredor(smgr, "assets/coche.obj", ID_COLISION);
	pj2->getNodo()->setPosition(vector3df(230, -50, 0));
	pj2->cambiarColor(255,255,255,smgr);
	IMeshSceneNode *IA = pj2->getNodo();



	// array de Waypoints
	Waypoint **arrayWaypoints;
	arrayWaypoints = new Waypoint *[20];
	float posanteriorZ=0;
	float posanteriorX=0;

	for(int i=0; i< 20 ; i++){

		arrayWaypoints[i] = new Waypoint(smgr);
		
		if (i==0){
		arrayWaypoints[i]->SetPosicion(235,-50,0);
		}else if(i<12){
		posanteriorZ= arrayWaypoints[i-1]->GetPosicion().Z;	
		arrayWaypoints[i]->SetPosicion(235,0, posanteriorZ + 40);	
		}else if(i>=12 && i<14){
		posanteriorX= arrayWaypoints[i-1]->GetPosicion().X;	
		posanteriorZ= arrayWaypoints[i-1]->GetPosicion().Z;	
		//arrayWaypoints[i]->SetPosicion(225,0, posanteriorZ + 30);
		arrayWaypoints[i]->SetPosicion(posanteriorX -15,0, posanteriorZ + 30);	
		}

	}



	//pj2->escalar(5.0f);
	// -----------------------------
	//  IMPORTAR MALLA (MAPA)
	// -----------------------------

	// Mapa cargado desde obj
	IMesh *mapa = smgr->getMesh("assets/mapa01.obj");

	if (!mapa)
	{
		device->drop();
		return 1;
	}

	// -----------------------------
	//  GEOMETRIA MAPA
	// -----------------------------

	// Cargar modelo mapa
	IMeshSceneNode *mapaNodo = smgr->addOctreeSceneNode(mapa, 0, ID_COLISION);

	smgr->getMeshManipulator()->setVertexColors(mapaNodo->getMesh(), SColor(255, 232, 128, 0));
	if (mapaNodo)
	{
		mapaNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		mapaNodo->setPosition(vector3df(0, 0, 0));
		//mapaNodo->setScale(vector3df(25,25,25));
		selector = smgr->createTriangleSelector(mapa, 0);
		mapaNodo->setTriangleSelector(selector);
		selector->drop();
		mapaNodo->setName("MAPA");
	}

	//colisiones del jugador
	if (selector)
	{
		const aabbox3d<f32> &cajaColision = pj1->getNodo()->getBoundingBox();
		vector3df radioColision = cajaColision.MaxEdge - cajaColision.getCenter();

		ISceneNodeAnimator *animacionColision = smgr->createCollisionResponseAnimator(
			selector,			 // Selector de fisicas del mundo
			pj1->getNodo(),		 // Objeto que tendra colisiones
			radioColision,		 // Radio de elipse
			vector3df(0, -5, 0), // Gravedad
			vector3df(0, 0, 0)); // Translacion

		selector->drop();
		pj1->getNodo()->addAnimator(animacionColision);
		animacionColision->drop();
	}


	//colisiones de la IA 
	if (selector)
	{
		const aabbox3d<f32> &cajaColision = pj2->getNodo()->getBoundingBox();
		vector3df radioColision = cajaColision.MaxEdge - cajaColision.getCenter();

		ISceneNodeAnimator *animacionColision = smgr->createCollisionResponseAnimator(
			selector,			 // Selector de fisicas del mundo
			pj2->getNodo(),		 // Objeto que tendra colisiones
			radioColision,		 // Radio de elipse
			vector3df(0, -5, 0), // Gravedad
			vector3df(0, 0, 0)); // Translacion

		selector->drop();
		pj2->getNodo()->addAnimator(animacionColision);
		animacionColision->drop();
	}

	//variable para identificar la direccion de movimiento (activo o no)
	int checkGiro = 0;
	int checkMarchaAtras = 0;
	float checkVelocidad = 0;
	//---------------------//
	//---CAMARA INICIAL----//
	//---------------------//
	vector3df cuboPos = pj1->getPosicion();
	vector3df camPos(0, 200, -8);
	vector3df camRot = pj1->getRotacion();
	smgr->addCameraSceneNode(pj1->getNodo(), camPos, cuboPos, ID_NULO); //3 parametros =  nodopadre, posicion, direccion

	// -----------------------------
	//  INTERFAZ
	// -----------------------------
	stringw text = L"Datos del jugador:\n"; // PARA MODIFICACIONES FUTURAS
	IGUIFont *fuente = guienv->getFont("assets/fuente.bmp");
	IGUIStaticText *textoUI = guienv->addStaticText(
		text.c_str(),				 // Texto
		rect<s32>(10, 10, 260, 150), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		ID_NULO,					 // Id del elemento
		true);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);

	// -----------------------------
	//  GAME LOOP
	// -----------------------------
	int lastFPS = -1;
	while (device->run())
	{
		if (device->isWindowActive())
		{
			// PARA MODIFICACIONES DEBUG
			text = L"Datos del jugador:\n";

			//pj1->setAxis(smgr);

			// Linea que comprueba las colisiones del objeto
			line3d<f32> rayo;
			rayo.start = pj1->getPosicion();

			vector3df radioColision = (pj1->getNodo()->getBoundingBox().MaxEdge - pj1->getNodo()->getBoundingBox().getCenter()) + 2;
			rayo.end = rayo.start;
			rayo.end.Y -= radioColision.Y;

			vector3df interseccion;		// Analizar el punto de colision con la malla u objeto
			triangle3df trianguloGolpe; // Para mostrar el triangulo de la colision

			ISceneNode *nodoColision = gestorColisiones->getSceneNodeAndCollisionPointFromRay(
				rayo, interseccion, trianguloGolpe, ID_COLISION, 0);

			if (nodoColision)
			{
				//cout << "CHOQUE" << endl;
				text += "Colision con: ";
				text += nodoColision->getName();
			}

			//Mostrar la Posicion y Velocidad actuales.
			text += "\nVelocidad: ";
			text += pj1->getVelocidad();
			text += "\nPosicion [";
			text += pj1->getPosicion().X;
			text += ", ";
			text += pj1->getPosicion().Y;
			text += ", ";
			text += pj1->getPosicion().Z;
			text += "]\n";

			checkGiro = 0;
			checkMarchaAtras = 0;
			checkVelocidad = pj1->getVelocidad();

			//-------ENTRADA TECLADO ----------//
			if (teclado.isKeyDown(KEY_ESCAPE))
			{
				device->closeDevice();
				return 0;
			}
			else if (teclado.isKeyDown(KEY_KEY_S))
			{
				pj1->frenar();
				checkMarchaAtras = 1;
			}
			else if (teclado.isKeyDown(KEY_KEY_W))
			{
				pj1->acelerar();
			}
			else
			{
				pj1->desacelerar();
			}
			if (teclado.isKeyDown(KEY_KEY_D))
			{
				if (checkMarchaAtras == 0)
				{
					pj1->girarDerecha();
				}
				else
				{
					if (checkVelocidad < 0.5)
					{
						pj1->girarIzquierda();
					}
				}
				checkGiro = 1;
			}
			else if (teclado.isKeyDown(KEY_KEY_A))
			{
				if (checkMarchaAtras == 0)
				{
					pj1->girarIzquierda();
				}
				else
				{
					if (checkVelocidad < 0.5)
					{
						pj1->girarDerecha();
					}
				}
				checkGiro = 1;
			}
			pj1->update();
			if (checkGiro == 0)
			{
				pj1->resetGiro();
			}

			//-------ENTRADA TECLADO FIN----------//
			int fps = driver->getFPS();
			if (lastFPS != fps)
			{
				core::stringw tmp(L"Age of karts [");
				tmp += driver->getName();
				tmp += L"] fps: ";
				tmp += fps;

				device->setWindowCaption(tmp.c_str());
				lastFPS = fps;
			}

			textoUI->setText(text.c_str());

			// MOVIMIENTO DE LA CAMARA     //
			smgr->getActiveCamera()->setTarget(pj1->getPosicion());

			//-------RENDER INI---------//
			driver->beginScene(true, true, SColor(255, 200, 200, 200));
			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();
		}
		else
		{
			device->yield();
		}
	}
	device->drop();

	return 0;
}