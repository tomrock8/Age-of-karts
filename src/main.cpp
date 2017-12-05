#include <iostream>
#include <string>

#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
#include "Corredor.hpp"
#include "CorredorIA.hpp"
#include "Waypoint.hpp"
#include "Pista.hpp"
#include "Motor3d.hpp"

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
	CTeclado *teclado = new CTeclado();

	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	Motor3d *m = Motor3d::getInstancia();
	m->setTeclado(teclado);

	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();
	IGUIEnvironment *guienv = m->getGUI();

	//COLISIONES
	ITriangleSelector *selector = 0; //Selector de triangulos para las colisiones
	ISceneCollisionManager *gestorColisiones = smgr->getSceneCollisionManager();

	// -----------------------------
	//	MAPA
	// -----------------------------
	Pista *pista = new Pista();
	selector = pista->setColisiones(selector);


	// -----------------------------
	//	Waypoints
	// -----------------------------
	int tamanyoArrayWaypoints = 20;
	Waypoint **arrayWaypoints;
	arrayWaypoints = new Waypoint *[tamanyoArrayWaypoints];
	float posanteriorZ = 0;
	float posanteriorX = 0;

	for (int i = 0; i < tamanyoArrayWaypoints; i++)
	{
		arrayWaypoints[i] = new Waypoint();
		arrayWaypoints[i]->setNombre(to_string(i));
		if (i > 0 && i <= (tamanyoArrayWaypoints - 2))
		{
			arrayWaypoints[i - 1]->setSiguiente(arrayWaypoints[i]);

		}
		else if (i == tamanyoArrayWaypoints - 1) {
			arrayWaypoints[i - 1]->setSiguiente(arrayWaypoints[i]);
			arrayWaypoints[i]->setSiguiente(arrayWaypoints[0]);
		}


		if (i == 0) {
			arrayWaypoints[0]->setPosicion(235, -50, 0);
		}

		else if (i > 0 && i < 12)
		{
			posanteriorZ = arrayWaypoints[i - 1]->getPosicion().Z;
			arrayWaypoints[i]->setPosicion(235, 0, posanteriorZ + 40);
		}
		else if (i >= 12 && i < 14)
		{
			posanteriorX = arrayWaypoints[i - 1]->getPosicion().X;
			posanteriorZ = arrayWaypoints[i - 1]->getPosicion().Z;
			//arrayWaypoints[i]->setPosicion(225,0, posanteriorZ + 30);
			arrayWaypoints[i]->setPosicion(posanteriorX - 15, 0, posanteriorZ + 30);
		}
	}


	arrayWaypoints[14]->setPosicion((arrayWaypoints[13]->getPosicion().X - 20), 0, (arrayWaypoints[13]->getPosicion().Z + 10));
	arrayWaypoints[15]->setPosicion((arrayWaypoints[14]->getPosicion().X - 30), 0, (arrayWaypoints[14]->getPosicion().Z + 10));
	arrayWaypoints[16]->setPosicion((arrayWaypoints[15]->getPosicion().X - 40), 0, (arrayWaypoints[15]->getPosicion().Z + 10));
	arrayWaypoints[17]->setPosicion((arrayWaypoints[16]->getPosicion().X - 50), 0, (arrayWaypoints[16]->getPosicion().Z + 5));
	arrayWaypoints[18]->setPosicion((arrayWaypoints[17]->getPosicion().X - 60), 0, (arrayWaypoints[17]->getPosicion().Z - 5));
	arrayWaypoints[19]->setPosicion((arrayWaypoints[18]->getPosicion().X - 70), 0, (arrayWaypoints[18]->getPosicion().Z - 50));


	// -----------------------------
	//  CORREDORES
	// -----------------------------
	Corredor *pj1 = new Corredor("assets/coche.obj", ID_COLISION);
	//pj1->escalar(5.0f);
	//colisiones del jugador
	selector = pj1->setColisiones(m->getDevice(), selector);

	//IMeshSceneNode *Jugador = pj1->getNodo();
	CorredorIA *pj2 = new CorredorIA("assets/coche.obj", ID_COLISION, arrayWaypoints, tamanyoArrayWaypoints);
	selector = pj2->setColisiones(m->getDevice(), selector);
	pj2->getNodo()->setPosition(vector3df(230, -50, 0));
	pj2->cambiarColor(255, 255, 255, smgr);
	//IMeshSceneNode *IA = pj2->getNodo();


	//variable para identificar la direccion de movimiento (activo o no)
	int checkGiro = 0;
	int checkMarchaAtras = 0;
	float checkVelocidad = 0;
	//---------------------//
	//---CAMARA INICIAL----//
	//---------------------//
	vector3df cuboPos = pj1->getPosicion();
	//vector3df camPos(0, 200, -8);
	vector3df camPos(0, 3, -8);
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

	int lastFPS = -1;



	//driver->draw3DLine(vector3df(), vector3df(), SColor(255, 255, 255, 0));
	driver->draw3DLine(vector3df(230, 0, 0), vector3df(230, 1000, 0), SColor(255, 255, 255, 0));
	// -----------------------------
	//  GAME LOOP
	// -----------------------------	
	while (m->getDevice()->run())
	{
		if (m->getDevice()->isWindowActive())
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
			text += "]\nNodoActual:";
			text += pj2->getNombreWaypoint().c_str();
			text += "\n";

			checkGiro = 0;
			checkMarchaAtras = 0;
			checkVelocidad = pj1->getVelocidad();


			if (teclado->isKeyDown(KEY_KEY_R)) {

				pj2->movimiento();

			}
			//-------ENTRADA TECLADO ----------//
			if (teclado->isKeyDown(KEY_ESCAPE))
			{
				m->cerrar();
				return 0;
			}
			else if (teclado->isKeyDown(KEY_KEY_S))
			{
				pj1->frenar();
				checkMarchaAtras = 1;
			}
			else if (teclado->isKeyDown(KEY_KEY_W))
			{
				pj1->acelerar();
			}
			else
			{
				pj1->desacelerar();
			}
			if (teclado->isKeyDown(KEY_KEY_D))
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
			else if (teclado->isKeyDown(KEY_KEY_A))
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
			pj2->update();
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

				m->getDevice()->setWindowCaption(tmp.c_str());
				lastFPS = fps;
			}

			textoUI->setText(text.c_str());

			// MOVIMIENTO DE LA CAMARA
			smgr->getActiveCamera()->setTarget(pj1->getPosicion());

			//	RENDER
			m->dibujar();
		}
		else
		{
			m->getDevice()->yield();
		}
	}

	m->getDevice()->drop();

	return 0;
}