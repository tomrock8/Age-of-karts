
//proyecto 1. Cargar cubo y mover 
//proyecto 3. Cargar modelo de mapa
#include "irrlichtlib.hpp"
#include "CTeclado.cpp"
#include "corredor.hpp"
#include <iostream>

using namespace std;

//float movimiento(float pos,float vel,float accel,f32 delta){
//    pos = pos+vel*delta+(0.5*accel*(exp2(delta)));    //donde el 0 es la velocidad inicial
//    return pos;
//}
int main(){
    // -----------------------------
    //  VARIABLES COCHE
    // -----------------------------
	//float aZ        = 0.01; 		//aceleracion eje Z
	//float aZInversa = 0.005;	    //marcha atras
	//float Afrenado  = 0.03;		//aceleracion eje X
	//float t         = 0.5; 			//Tiempo 
	//float vIni      = 0;
	//float xIni 		= 0;
	//float v 		= 0;
	//float x 		= 0;
	//bool back  		= false;
	//bool front 		= false;
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
    //  GEOMETRIA COCHE
    // -----------------------------
	corredor* pj1=new corredor(smgr, "sources/coche.obj");
		pj1->escalar(5.0f);
	//IMesh* coche = smgr->getMesh("sources/coche.obj");
	//IMeshSceneNode *cuboNodo = smgr->addMeshSceneNode(coche);
	////cambiar a color rojo del coche
	//smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(),SColor(255,255,0,0));
//
	////escalar objeto
	//core::vector3d<f32> factorEscalate(5,5,5);
	//cuboNodo->setScale(factorEscalate);
	//	
	//// Desactivar la iluminacion del cubo
	//if(cuboNodo){
	//	cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	//	cuboNodo->setPosition(vector3df(0,20,0));
	//}

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

    //---------------------//
	//---CAMARA INICIAL----//
	//---------------------//
	 	vector3df cuboPos = pj1->getPosition(); 
		vector3df cuboRot = pj1->getRotation();
    	vector3df camPos( pj1->getPosition().X,  pj1->getPosition().Y + 10,  pj1->getPosition().Z - 25); 
    	smgr->addCameraSceneNode(0, camPos, cuboPos); //3 parametros =  nodopadre, posicion, direccion 
    // -----------------------------
    //  GAME LOOP
    // -----------------------------
	while(device->run()){
		
	    if(device->isWindowActive()){
			pj1->setAxis(smgr);
			pj1->setVelocidad();
			pj1->setEspacio();
			//vIni = v;
			//xIni = x;
			
            //Mostrar la Posicion y Velocidad actuales.
            stringw text = L"Age Of Karts - ";
            
			text = L"Velocidad v [";
            text += pj1->getVelocidad();
            text +="] posicion X: ";
            text += pj1->getEspacioX();
			text +="] posicion Z: ";
			text += pj1->getEspacioZ();
			
        
			//vector3df cuboPos =  cuboNodo->getPosition();
            //Actualizar el valor del delta time
            // const u32 ahora = device->getTimer()->getTime();
            // const f32 delta = (f32)(ahora - antes) / 1000.f;
	
		
            //variable para identificar la direccion de movimiento (activo o no)
            int checkGiro=0;
            //-------ENTRADA TECLADO ----------//
			if(teclado.isKeyDown(KEY_ESCAPE)) {
				device->closeDevice();
				return 0;
			} else if(teclado.isKeyDown(KEY_KEY_D)  ){
				pj1->girarDerecha();
				checkGiro=1;
			} else if(teclado.isKeyDown(KEY_KEY_A) ){
                pj1->girarIzquierda();
				checkGiro=1;
			}
			if(teclado.isKeyDown(KEY_KEY_S)){
				pj1->frenar();
			}else if(teclado.isKeyDown(KEY_KEY_W)){
				pj1->acelerar();
			}else{
				pj1->desacelerar();
			}
			pj1->actualizarPos();
			if (checkGiro==0){
				pj1->resetGiro();
			}
			//cuboNodo->setPosition(cuboPos);
			//-------ENTRADA TECLADO FIN----------//
			//-----------------------------//
			// MOVIMIENTO DE LA CAMARA     //
			//---------------------------- //
            cuboPos = pj1->getPosition(); 
            camPos.X = pj1->getPosition().X; 
            camPos.Y = pj1->getPosition().Y + 10; 
            camPos.Z = pj1->getPosition().Z - 25; 
 
            smgr->getActiveCamera()->setPosition(camPos); 
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