

#include "irrlichtlib.hpp"
#include "CTeclado.hpp"
#include "corredor.hpp"
#include "ventana.hpp"
#include <iostream>

using namespace std;


int main(){


	CTeclado teclado; 
    // -----------------------------
    //  PREPARAR LA VENTANA
    // -----------------------------
	//ventana*  window = new ventana(teclado);

    //std::cout<<"entro a ventana"<<endl;
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480),16,false,false,false,&teclado);
	//IrrlichtDevice *device  = window->getDevice();
	if(!device) return 1;
	//
   // device->setWindowCaption(L"AGE OF KARTS");
    IVideoDriver* driver    =  device->getVideoDriver();
	ISceneManager* smgr     =  device->getSceneManager();
	IGUIEnvironment* guienv =  device->getGUIEnvironment();
	
	//BIVideoDriver* driver    = window->getDriver();
	//ISceneManager* smgr     = window->getScene(); 
	//IGUIEnvironment* guienv = window->getGUI();;

    // -----------------------------
    //  GEOMETRIA COCHE
	// -----------------------------
	
	corredor* pj1=new corredor(smgr, "sources/coche.obj");
		pj1->escalar(5.0f);

    // -----------------------------
    //  IMPORTAR MALLA (MAPA)
    // -----------------------------

	// Mapa cargado desde obj
	
	IMesh* mapa = smgr->getMesh("sources/mapaPr.obj");
  
	if(!mapa){

		device->drop();		
		return 1;
	}

    // -----------------------------
    //  GEOMETRIA MAPA
    // -----------------------------

	// Cargar modelo mapa
	   

	IMeshSceneNode *mapaNodo = smgr->addMeshSceneNode(mapa);
  

	smgr->getMeshManipulator()->setVertexColors(mapaNodo->getMesh(),SColor(255,232,128,0));
	if(mapaNodo){
        mapaNodo->setMaterialFlag(EMF_LIGHTING,false); // Desactivar iluminacion
        mapaNodo->setPosition(vector3df(0,-20,-30));
	}
		
    //--FPS y Delta time--// -> borrados
  	// int fpsAntes = -1;
	//	u32 antes = device->getTimer()->getTime();

	//variable para identificar la direccion de movimiento (activo o no)
    int checkGiro=0;
	int checkMarchaAtras=0;
	float checkVelocidad=0;
    //---------------------//
	//---CAMARA INICIAL----//
	//---------------------//
	 	vector3df cuboPos = pj1->getPosicion(); 
		/*INI 26/11/2017  Camara fija*/
		vector3df camPos(0,  2 ,  -5); 
		//vector3df camPos( pj1->getPosition().X,  pj1->getPosition().Y + 10,  pj1->getPosition().Z - 25); 
		//window->getScene()->addCameraSceneNode(0, camPos, cuboPos); //3 parametros =  nodopadre, posicion, direccion 
		 smgr->addCameraSceneNode(pj1->getNodo(), camPos, cuboPos); //3 parametros =  nodopadre, posicion, direccion 
		/*FIN 26/11/2017  Camara fija*/
		//window->getScene()->getActiveCamera()-> bindTargetAndRotation(true);
		float RotacionX = 0;
    // -----------------------------
    //  GAME LOOP
    // -----------------------------
	while(device->run()){
		
	    if(device->isWindowActive()){
			pj1->setAxis(smgr);
			//pj1->setVelocidad();
			//pj1->setEspacio();
			//vIni = v;
			//xIni = x;
			
            //Mostrar la Posicion y Velocidad actuales.
            stringw text = L"Age Of Karts - ";
            
			text = L"Velocidad v [";
            text += pj1->getVelocidad();
            text +="] posicion X: ";
            text += pj1->getPosicion().X;
			text +="] posicion Z: ";
			text += pj1->getPosicion().Z;
			text +="] rotacion: ";
			text += pj1->getRotacion().Y;
			//text +="] rotacion : ";
			//text += pj1->getRotation().Y;
			
        
			//vector3df cuboPos =  cuboNodo->getPosition();
            //Actualizar el valor del delta time
            // const u32 ahora = device->getTimer()->getTime();
            // const f32 delta = (f32)(ahora - antes) / 1000.f;
	
			checkGiro=0;
			checkMarchaAtras=0;
            checkVelocidad=pj1->getVelocidad();
			
            //-------ENTRADA TECLADO ----------//
			if(teclado.isKeyDown(KEY_ESCAPE)) {
				device->closeDevice();
				return 0;
			} else if(teclado.isKeyDown(KEY_KEY_S)){
				pj1->frenar();
				checkMarchaAtras=1;
			}else if(teclado.isKeyDown(KEY_KEY_W)){
				pj1->acelerar();
			}else{
				pj1->desacelerar();
			}
			if(teclado.isKeyDown(KEY_KEY_D)  ){
				if (checkMarchaAtras==0){
					pj1->girarDerecha();
				}else{
					if (checkVelocidad<0.5){
						pj1->girarIzquierda();
					}
				}
				checkGiro=1;
			} else if(teclado.isKeyDown(KEY_KEY_A) ){
                if (checkMarchaAtras==0){					
					pj1->girarIzquierda();
				}else{
					if (checkVelocidad<0.5){
						pj1->girarDerecha();
					}
				}
				checkGiro=1;
			}
			pj1->update();
			if (checkGiro==0){
				pj1->resetGiro();
			}
			//cuboNodo->setPosition(cuboPos);
			//-------ENTRADA TECLADO FIN----------//
			//-----------------------------//
			// MOVIMIENTO DE LA CAMARA     //
			//---------------------------- //
            cuboPos = pj1->getPosicion(); 
            //camPos.X = cuboPos.X ; 
            //camPos.Y = cuboPos.Y + 10; 
            //camPos.Z =cuboPos.Z - 35 ; 
			//camRot = pj1->getRotacion(); 
 
            //window->getScene()->getActiveCamera()->setPosition(camPos); 
			//window->getScene()->getActiveCamera()->setRotation(cuboPos); 
			
            smgr->getActiveCamera()->setTarget(cuboPos); 
			//-------RENDER INI---------//
			driver->beginScene(true,true,SColor(255,200,200,200));
			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();	

			//-----MOSTRAR VELOCIDAD Y POSICION EN VENTANA
           device->setWindowCaption(text.c_str());

			
		} else{
			device->yield();
		}
		
	}
	device->drop();
	
	return 0;
	
	
	
}