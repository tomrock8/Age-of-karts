#include "irrlichtlib.hpp"
#include "corredor.h"



corredor::corredor(ISceneManager* smgr, stringw rutaObj){
     aZ        = 0.1; 		//aceleracion eje Z
	 aZInversa = 0.05;	    //marcha atras
	 Afrenado  = 0.3;		//aceleracion eje X
	 t         = 0.5; 			//Tiempo 
	 vIni      = 0;
	 xIni 	   = 0;
	 v 		   = 0;
	 x 		   = 0;
	 adelante  = false;
     atras 	   = false;

    coche = smgr->getMesh(rutaObj);
	cuboNodo = smgr->addMeshSceneNode(coche);
	//cambiar a color rojo del coche
	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(),SColor(255,255,0,0));
	// Desactivar la iluminacion del cubo
	if(cuboNodo){
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(vector3df(0,20,0));
	} 

}
//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\* 
float corredor::movimiento(float pos,float vel,float accel,float delta){
    pos = pos+vel*delta+(0.5*accel*(exp2(delta)));    //donde el 0 es la velocidad inicial
    return pos;
}
//-----------------------\*
//-------POSICION--------\*
//-----------------------\* 
vector3df corredor::getPosition(){
    return cuboNodo->getPosition();
}
void corredor::actualizarPos(){
    cuboPos.Z = x;
    cuboNodo->setPosition(cuboPos);

}
void corredor::setVelocidad(){
    vIni = v;
}
void corredor::setEspacio(){
    xIni = x;
}
float corredor::getVelocidad(){
    return v;
}
float corredor::getEspacio(){
    return x;
}
//-----------------------\*
//---MOVIMIENTO JUGADOR--\*
//-----------------------\* 
 void corredor::acelerar(){
        //v=v0+a⋅t
		//cout<<"velocidad marcha adelante: "<<v<<"  "<<endl;
		if(v< 5){
			v = vIni+aZ*t;
		}
		x = movimiento(xIni,vIni,aZ,t);
		atras    = false;
        adelante = true;
        //cuboPos.Z =x;
    } 

 void corredor::frenar(){
    cout<<"velocidad marcha atras: "<<v<<"  "<<endl;
		atras    = true;
		adelante = false;
		if(v<2.5f){
			v = vIni + Afrenado*t;
			//	v = v -0.3;
		}
		x = movimiento(xIni,-vIni,aZInversa,t);
		//cuboPos.Z =x;
					
    }
void corredor::desacelerar(){
    //desaceleracion
	//X = Xi + Vi . t - 1/2 . a . t² 
	//V = Vi - a . t
	if(v >0){
		if(atras){
			x = movimiento(xIni,-vIni,-aZ,t);
		}
		if(adelante){
			x = movimiento(xIni,vIni,-aZ,t);						
		}
		v = vIni -aZ*t;
		cuboPos.Z =x;
    }
}
 void corredor::escalar(float tam){
        core::vector3d<f32> factorEscalado(tam,tam,tam);
	    cuboNodo->setScale(factorEscalado);
	}