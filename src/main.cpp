
//proyecto 1. Cargar cubo y mover 
//proyecto 3. Cargar modelo de mapa

#include <irrlicht.h>
#include <iostream>
using namespace std;
using namespace irr;

using namespace core;		//proposito general
using namespace scene;		//Escena 3D
using namespace video;		//Driver y rendering
using namespace io;			//Ficheros
using namespace gui;		//Interfaz de usuario

#include "Teclado.h"

int main(){
	float aZ = 0.1; 		//aceleracion eje Z
	float aZInversa = 0.05;	//marcha atras
	float aX = 0.15;		//aceleracion eje X
	float t = 0.5; 			//Tiempo 
	float vIni = 0;
	float xIni = 0;
	float v = 0;
	float x = 0;
	bool back = false;
	bool front = false;
	//x = xIni + VIni*t + 1/2*a*t*t
	//v=v0+a⋅t
	Teclado teclado; 
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, 
										  dimension2d<u32>(640, 480),
										  16,
										  false,
										  false,
										  false,
										  &teclado);
	if(!device) return 1;
	
	IVideoDriver* driver   =  device->getVideoDriver();
	ISceneManager* smgr    =  device->getSceneManager();
	IGUIEnvironment* guienv =  device->getGUIEnvironment();
	
	//cargar modelo cubo
	IMeshSceneNode *cuboNodo = smgr->addCubeSceneNode(10);//anyadir directamente cubo a la escena de 5x5x5
	//cambiar a color rojo el cubo
	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(),SColor(255,255,0,0));
	
	
	//anyadir cubo a la escena
	
	//desactivar la iluminacion del cubo
	if(cuboNodo){
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false);
		cuboNodo->setPosition(vector3df(0,20,0));
	}
	//mapa cargado desde obj
	IMesh* mapa = smgr->getMesh("sources/mapaPr.obj");

	if(!mapa){
		cout<<"mierda pa ti"<<endl;
		device->drop();
		
		return 1;

	}
	IMeshSceneNode *mapaNodo = smgr->addMeshSceneNode(mapa);

	smgr->getMeshManipulator()->setVertexColors(mapaNodo->getMesh(),SColor(255,0,0,0));
	if(mapaNodo){
			 mapaNodo->setMaterialFlag(EMF_LIGHTING,false);//desactivar iluminacion
			 mapaNodo->setPosition(vector3df(0,10,0));
	}
	
	



	//------------mapa quake 3 -------------
//
//	device->getFileSystem()->addFileArchive("../media/map-20kdm2.pk3");
//	
//	//cargar mapa  QUAKE 3
//	scene::IAnimatedMesh* mapa = smgr->getMesh("20kdm2.bsp");
//	scene::ISceneNode* node = 0;
//
//	if(mapa) node = smgr->addOctreeSceneNode(mapa->getMesh(0),0,-1,1024);
//	if(node) node->setPosition(core::vector3df(-1300,-144,-1249));
//
	//------------mapa quake 3 -------------


	//camara
	//vector3df cubop = cuboNodo->getPosition();



	//camara para el mapà
	//smgr->addCameraSceneNodeFPS();//camara que se mueve como si de un counter se tratase(magia)
	//device->getCursorControl()->setVisible(false);//ocultar el cursor

	int lastFPS = -1; //quak3
	stringw text = "";

            

	while(device->run()){
		
	if(device->isWindowActive()){
		//-------Calculo de la posicion dependiendo de la velocidad en un T predefinido
			//x = xIni + VIni*t + 1/2*a*t*t
			vIni = v;
			xIni = x;
			
			text = L"Velocidad v [";
                text += v;
                text +="] posicion X: ";
                text += x;
			device->setWindowCaption(text.c_str());
		//-------ENTRADA TECLADO INI----------//
			vector3df cuboPos =  cuboNodo->getPosition();
			smgr->addCameraSceneNode(0,vector3df(cuboPos.X,cuboPos.Y+10, cuboPos.Z-25),cuboNodo->getPosition());//3 parametros =  nodopadre, posicion, direccion
			//smgr->addCameraSceneNodeFPS(0);//camara que se mueve como si de un counter se tratase(magia)

			if(teclado.isKeyDown(KEY_ESCAPE)) {
				device->closeDevice();
				return 0;
			} else if(teclado.isKeyDown(KEY_KEY_D) && teclado.isKeyDown(KEY_KEY_W) ){
				v = vIni+aZ*t;
				x = xIni + vIni*t + 1/2*aZ*t*t;
				cuboPos.Z =x;
				v = vIni+aX*t;
				x = xIni + vIni*t + 1/2*aX*t*t;
				cuboPos.X = x;
			} else if(teclado.isKeyDown(KEY_KEY_A)&& teclado.isKeyDown(KEY_KEY_W) ){
				v = vIni+aZ*t;
				x = xIni + vIni*t + 1/2*aZ*t*t;
				cuboPos.Z =x;
				v = vIni+aX*t;
				x = xIni + vIni*t + 1/2*aX*t*t;
				cuboPos.X = -x;
			}else if(teclado.isKeyDown(KEY_KEY_S)){//esto no esta bien
				v = vIni+aZInversa*t;
				x =  xIni - v*t + 1/2*aZInversa*t*t;
				cuboPos.Z =x;
				back = true;
				front = false;		
			}else if(teclado.isKeyDown(KEY_KEY_W)){
				//v=v0+a⋅t
				v = vIni+aZ*t;
				x = xIni + v *t + 1/2*aZ*t*t;
				cuboPos.Z =x;
				back  = false;
				front = true;
			}else{//desaceleracion
				//X = Xi + Vi . t - 1/2 . a . t² 
				//V = Vi - a . t
				if(v >0){
					if(back){
						x = xIni - vIni *t -1/2*aZ*t*t;
					}
					if(front){
						x = xIni + vIni *t -1/2*aZ*t*t;

					}
					v = vIni -aZ*t;
					cuboPos.Z =x;
				}
			


			}
			cuboNodo->setPosition(cuboPos);
			//-------ENTRADA TECLADO FIN----------//
			//-------RENDER INI---------//
			driver->beginScene(true,
							true,
							SColor(255,200,200,200));
			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();	

			//-------RENDER FIN---------//
				
			//------MOSTRAR MAPA INICIO----------//
			int fps = driver->getFPS();
				
			if(lastFPS !=fps){
				core::stringw str = L"Proyecto3 irrlicht - Carga mapa QUAKE3 [";
				str += driver->getName();
				str += "] FPS: ";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
			
			//-------MOSTRAR MAPA FIN----------//
		} else{
			device->yield();
		}
		
	}
	device->drop();
	
	return 0;
	
	
	
}