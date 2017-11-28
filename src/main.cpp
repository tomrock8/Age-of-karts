

#include "irrlichtlib.hpp"
#include "CTeclado.hpp"
#include "corredor.hpp"
#include "ventana.hpp"
#include <iostream>

using namespace std;

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

	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, &teclado);
	if (!device)
		return 1;

	device->setWindowCaption(L"AGE OF KARTS");

	IVideoDriver *driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();
	IGUIEnvironment *guienv = device->getGUIEnvironment();

	//COLISIONES
	ITriangleSelector *selector = 0; //Selector de triangulos para las colisiones
	ISceneCollisionManager *gestorColisiones = smgr->getSceneCollisionManager();

	// -----------------------------
	//  GEOMETRIA COCHE
	// -----------------------------
	corredor *pj1 = new corredor(smgr, "assets/coche.obj", ID_COLISION);
	pj1->escalar(2.0f);

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
			selector,			  // Selector de fisicas del mundo
			pj1->getNodo(),		  // Objeto que tendra colisiones
			radioColision,		  // Radio de elipse
			vector3df(0, -5, 0), // Gravedad
			vector3df(0, 0, 0));  // Translacion

		selector->drop();
		pj1->getNodo()->addAnimator(animacionColision);
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
	vector3df camPos(0, 3, -8);
	vector3df camRot = pj1->getRotacion();
	smgr->addCameraSceneNode(pj1->getNodo(), camPos, cuboPos, ID_NULO); //3 parametros =  nodopadre, posicion, direccion

	// -----------------------------
	//  GAME LOOP
	// -----------------------------
	while (device->run())
	{

		if (device->isWindowActive())
		{
			pj1->setAxis(smgr);

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
				cout << "Colision con: " << nodoColision->getName() << endl;
			}

			//Mostrar la Posicion y Velocidad actuales.
			stringw text = L"Age Of Karts - ";

			text = L" V[";
			text += pj1->getVelocidad();
			text += "] POS[";
			text += pj1->getPosicion().X;
			text += ", ";
			text += pj1->getPosicion().Y;
			text += ", ";
			text += pj1->getPosicion().Z;
			text += "]";

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
			//-----------------------------//
			// MOVIMIENTO DE LA CAMARA     //
			//---------------------------- //

			smgr->getActiveCamera()->setTarget(pj1->getPosicion());

			//-------RENDER INI---------//
			driver->beginScene(true, true, SColor(255, 200, 200, 200));
			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

			//-----MOSTRAR VELOCIDAD Y POSICION EN VENTANA
			device->setWindowCaption(text.c_str());
		}
		else
		{
			device->yield();
		}
	}
	device->drop();

	return 0;
}