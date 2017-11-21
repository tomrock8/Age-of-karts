
//proyecto 1. Cargar cubo y mover 
//proyecto 3. Cargar modelo de mapa
#include "irrlichtlib.hpp"
#include "CTeclado.cpp"
#include <iostream>

using namespace std;

float movimiento(float pos,float vel,float accel,f32 delta){
    pos = pos+vel*delta+(0.5*accel*(exp2(delta)));    //donde el 0 es la velocidad inicial
    return pos;
}
int main(){
    // -----------------------------
    //  VARIABLES COCHE
    // -----------------------------
	float aZ        = 0.1; 		//aceleracion eje Z
	float aZInversa = 0.05;	    //marcha atras
	float Afrenado  = 0;		//aceleracion eje X
	//float t         = 0.5; 			//Tiempo 
	float vIni      = 0;
	float xIni 		= 0;
	float v 		= 0;
	float x 		= 0;
	bool back  		= false;
	bool front 		= false;
	/*float fuerza=0;
	float fuerza_frenado=0;
    float fuerzamax=150;
	int masa=100;
    float aZ=fuerza/masa;*/

	CTeclado teclado; 
    // -----------------------------
    //  PREPARAR LA VENTANA
    // -----------------------------

	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480),16,false,false,false,&teclado);
	if(!device) return 1;
	
    device->setWindowCaption(L"AGE OF KARTS");

	IVideoDriver* driver    =  device->getVideoDriver();
	ISceneManager* smgr     =  device->getSceneManager();
	IGUIEnvironment* guienv =  device->getGUIEnvironment();
	

    // -----------------------------
    //  GEOMETRIA CUBO
    // -----------------------------

	// Cargar modelo cubo
	IMeshSceneNode *cuboNodo = smgr->addCubeSceneNode(10); // Anyadir directamente cubo a la escena de 5x5x5
	// Cambiar a color rojo el cubo
	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(),SColor(255,255,0,0));
	
		
	// Desactivar la iluminacion del cubo
	if(cuboNodo){
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(vector3df(0,20,0));
	}

    // -----------------------------
    //  IMPORTAR MALLA (MAPA)
    // -----------------------------

	// Mapa cargado desde obj
	IMesh* mapa = smgr->getMesh("sources/mapaPr.obj");

	if(!mapa){
		cout<<"Mierda pa ti"<<endl;
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

	//camara para el mapa
	//smgr->addCameraSceneNodeFPS();//camara que se mueve como si de un counter se tratase(magia)
	//device->getCursorControl()->setVisible(false);//ocultar el cursor

    // FPS
    int fpsAntes = -1;

    // Delta time
	u32 antes = device->getTimer()->getTime();
    
	
    // -----------------------------
    //  GAME LOOP
    // -----------------------------
	while(device->run()){
		
	    if(device->isWindowActive()){
			vIni = v;
			xIni = x;
			
            //Mostrar la Posicion y Velocidad actuales.
            stringw text = L"Age Of Karts - ";
            
			text = L"Velocidad v [";
            text += v;
            text +="] posicion X: ";
            text += x;
			
        
			vector3df cuboPos =  cuboNodo->getPosition();
            //Actualizar el valor del delta time
            const u32 ahora = device->getTimer()->getTime();
            const f32 delta = (f32)(ahora - antes) / 1000.f;

			smgr->addCameraSceneNode(0,vector3df(cuboPos.X,cuboPos.Y+10, cuboPos.Z-25),cuboNodo->getPosition());//3 parametros =  nodopadre, posicion, direccion
			//smgr->addCameraSceneNodeFPS(0);//camara que se mueve como si de un counter se tratase(magia)

            //variable para identificar el movimiento (activo o no)
            int checkMov=0;
            //-------ENTRADA TECLADO INI----------//
			if(teclado.isKeyDown(KEY_ESCAPE)) {
				device->closeDevice();
				return 0;
			} else if(teclado.isKeyDown(KEY_KEY_D)  ){
				checkMov=1;
				cuboPos.X+=4;
			} else if(teclado.isKeyDown(KEY_KEY_A) ){
                checkMov=2;
				cuboPos.X-=4;
			}else if(teclado.isKeyDown(KEY_KEY_S)){//esto no esta bien
				
				if(v<2.5f){
					v = vIni+aZInversa*delta;
				}
				x = movimiento(xIni,-vIni,aZInversa,delta);
	
				cuboPos.Z =x;
				back = true;
				front = false;		
			}else if(teclado.isKeyDown(KEY_KEY_W)){
				//v=v0+a⋅t
				if(v< 5){
					v = vIni+aZ*delta;
				}
				x = movimiento(xIni,vIni,aZ,delta);
				cuboPos.Z =x;
				back  = false;
				front = true;
			}else if(teclado.isKeyDown(KEY_SPACE)){
				
				v = vIni-Afrenado*delta;
				x = movimiento(xIni,vIni,Afrenado,delta);
				cuboPos.Z =x;
				
			}else{//desaceleracion
				//X = Xi + Vi . t - 1/2 . a . t² 
				//V = Vi - a . t
				if(v >0){
					if(back){
						x = movimiento(xIni,-vIni,-aZ,delta);
					}
					if(front){
						x = movimiento(xIni,vIni,-aZ,delta);						
					}
					v = vIni -aZ*delta;
					cuboPos.Z =x;
				}
			


			}
			cuboNodo->setPosition(cuboPos);
			//-------ENTRADA TECLADO FIN----------//
			//-------RENDER INI---------//
			driver->beginScene(true,true,SColor(255,200,200,200));
			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();	

			//-------RENDER FIN---------//
			// Calcular los fps
            int fpsAhora = driver->getFPS();
            if (fpsAntes != fpsAhora)
            {
                
                text += "] FPS: ";
                text += fpsAhora;
                text += " *TIMER: Antes: ";
                text += antes;
                text += " - Ahora: ";
                text += ahora;

                device->setWindowCaption(text.c_str());
                fpsAntes = fpsAhora;
            }
            antes = ahora;
			
		} else{
			device->yield();
		}
		
	}
	device->drop();
	
	return 0;
	
	
	
}