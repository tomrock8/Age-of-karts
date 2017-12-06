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
	CTeclado *teclado = CTeclado::getInstancia();

	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	Motor3d *m = Motor3d::getInstancia();
	//m->setTeclado(teclado);

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
		else if (i == tamanyoArrayWaypoints - 1) {
			arrayWaypoints[i - 1]->setSiguiente(arrayWaypoints[i]);
			arrayWaypoints[i]->setSiguiente(arrayWaypoints[0]);
		}


		if (i == 0) {
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



	// -----------------------------
	//  CORREDORES
	// -----------------------------
	CorredorJugador *pj1 = new CorredorJugador("assets/coche.obj", ID_COLISION);
	//pj1->escalar(5.0f);
	//colisiones del jugador
	selector = pj1->setColisiones(selector);

	//IMeshSceneNode *Jugador = pj1->getNodo();
	CorredorIA *pj2 = new CorredorIA("assets/coche.obj", ID_COLISION, arrayWaypoints, tamanyoArrayWaypoints);
	selector = pj2->setColisiones(selector);
	//pj2->getNodo()->setPosition(vector3df(230, -50, 20));
	pj2->setColor(255, 255, 255);
	//IMeshSceneNode *IA = pj2->getNodo();


	//---------------------//
	//---CAMARA INICIAL----//
	//---------------------//
	vector3df cuboPos = pj1->getPosicion();
	//vector3df camPos(0, 200, -8);
	vector3df camPos(0, 3, -8);
	vector3df camRot = pj1->getRotacion();
	smgr->addCameraSceneNode(pj1->getNodo(), camPos, cuboPos, ID_NULO); //3 parametros =  nodopadre, posicion, direccion
	//smgr->addCameraSceneNodeFPS();

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
			text = L" ---- CORREDOR 1 PJ ----\n";

			//pj1->setAxis();

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
				text += " Colision con: ";
				text += nodoColision->getName();
			}

			//Mostrar la Posicion y Velocidad actuales.
			text += "\n Velocidad: ";
			text += pj1->getVelocidad();
			text += "\n Posicion [";
			text += pj1->getPosicion().X;
			text += ", ";
			text += pj1->getPosicion().Y;
			text += ", ";
			text += pj1->getPosicion().Z;
			text += "]\n";
			text += " Direccion: ";
			text += pj1->getDireccion().c_str();
			text += " [ ";
			text += (int)pj1->getNodo()->getRotation().Y;
			text += " ]\n";

			text += "\n ---- CORREDOR 2 IA ----\n";
			text += " Waypoint siguiente: ";
			text += pj2->getNombreWaypoint().c_str();
			text += "\n";
			text += " Direccion: ";
			text += pj2->getDireccion().c_str();
			text += " [ ";
			text += (int)pj2->getNodo()->getRotation().Y;
			text += " ]\n";




			//-------ENTRADA TECLADO ----------//
			if (teclado->isKeyDown(KEY_KEY_R)) {
				pj2->movimiento();
			}

			if (teclado->isKeyDown(KEY_ESCAPE))
			{
				m->cerrar();
				return 0;
			}


			pj1->update();
			pj2->update();


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