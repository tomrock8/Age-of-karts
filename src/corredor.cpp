#include "irrlichtlib.hpp"
#include "corredor.hpp"
#include "AxesSceneNode.cpp"
#include <iostream>
#include <cmath>
using namespace std;

//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\* 
corredor::corredor(ISceneManager* smgr, stringw rutaObj){
     aZ        = 0.1; 		     //aceleracion eje Z
	 aZInversa = 0.15;	         //marcha atras
	 Afrenado  = 0.15;		     //aceleracion eje X
	 t         = 0.5; 			 //Tiempo 
	 vIni      = 0;
	 xIni 	   = 0;
	 zIni	   = 0;
	 vx 	   = 0;
	 vz 	   = 0;
	 v 		   = 0;
	 x 		   = 0;
	 z 		   = 0;
	 r		   = 2;
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
vector3df corredor::getRotation(){
    return cuboNodo->getRotation();
}
void corredor::actualizarPos(){
    cuboPos.Z = z;
	cuboPos.X = x;
    cuboNodo->setPosition(cuboPos);

}
void corredor::setVelocidad(){
    vIni = v;
}
void corredor::setEspacio(){
    xIni = x;
	zIni = z;
}
void corredor::setAxis(ISceneManager *smgr){
	AxesSceneNode* axis = new AxesSceneNode(cuboNodo,smgr,-1);
   	axis->setAxesScale(50); //  for the length of the axes
  	axis->drop();
}
float corredor::getVelocidad(){
    return v;
}
float corredor::getEspacioX(){
    return x;
}
float corredor::getEspacioZ(){
    return z;
}
//-----------------------\*
//---MOVIMIENTO JUGADOR--\*
//-----------------------\* 
 void corredor::acelerar(){
        //v=v0+a⋅t
        //cout<<"velocidad marcha adelante: "<<v<<"  "<<endl;

	
		if (vIni<0){
			vIni=0;
		}
		cuboRot = cuboNodo->getRotation();
		if (cuboRot.Y!=0){
			cout << PI << endl;
			if (cuboRot.Y<0){
				z=zIni+r*cos((-vIni/2*PI)*t);
				x=xIni+r*sin((-vIni/2*PI)*t);
				
			}else{
				z=zIni+r*cos((vIni/2*PI)*t);
				x=xIni+r*sin((vIni/2*PI)*t);

			}

				
		}else{			
		if(v< 5){
			v = vIni+aZ*t;
		}	
			z = movimiento(zIni,vIni,aZ,t);

        }
			atras    = false;
			adelante = true;
    } 
     
//-----------------------\*
//---FRENADO JUGADOR---\*
//-----------------------\* 
 void corredor::frenar(){
     if(adelante ==true){
         z = movimiento(zIni,vIni,-Afrenado,t); 
         if(v>-2.5f){//Controla que no pase de 2.5 de velocidad en marcha atras cuando se frena
			v = vIni - Afrenado*t;
			//	v = v -0.3;
		}
         //v = vIni -Afrenado*t;
     }
     else{
        cout<<"velocidad marcha atras: "<<v<<"  "<<endl;
		atras    = true;
		adelante = false;
		if(v<2.5f){
			v = vIni + Afrenado*t;
			//	v = v -0.3;
		}
		z = movimiento(zIni,-vIni,aZInversa,t);
     }   
    
		
					
}
//-----------------------\*
//------GIRAR JUGADOR----\*
//-----------------------\* 
void corredor::girarDerecha(){
    cuboRot = cuboNodo->getRotation();
	if (cuboRot.Y<50)
	cuboRot.Y+=0.5;
	cuboNodo->setRotation(vector3df(cuboRot.X ,cuboRot.Y,cuboRot.Z));

}

void corredor::girarIzquierda(){
    cuboRot = cuboNodo->getRotation();
	if (cuboRot.Y>-50)
	cuboRot.Y-=0.5;
	cuboNodo->setRotation(vector3df(cuboRot.X ,cuboRot.Y,cuboRot.Z));

}

void corredor::resetGiro(){
	if (cuboRot.Y>0){
		cuboRot.Y-=0.5;
	}
	if (cuboRot.Y<0){
		cuboRot.Y+=0.5;
	}
	cuboNodo->setRotation(vector3df(cuboRot.X ,cuboRot.Y,cuboRot.Z));


}

//-----------------------\*
//---DESACELERAR JUGADOR--\*
//-----------------------\* 
void corredor::desacelerar(){
    //desaceleracion
	//X = Xi + Vi . t - 1/2 . a . t² 
	//V = Vi - a . t
	if(v >0){
		if(atras){
			z = movimiento(zIni,-vIni,-aZInversa,t);
		}
		if(adelante){
			z = movimiento(zIni,vIni,-aZInversa,t);						
		}
        v = vIni -aZInversa*t;
        vector3df rotation =  vector3df(0,0,0); 
        cuboNodo->setRotation(rotation);
		
    }
}
 void corredor::escalar(float tam){
        core::vector3d<f32> factorEscalado(tam,tam,tam);
	    cuboNodo->setScale(factorEscalado);
	}