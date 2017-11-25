#include "irrlichtlib.hpp"
#include "corredor.hpp"
#include "AxesSceneNode.cpp"


//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\* 
corredor::corredor(ISceneManager* smgr, stringw rutaObj){
     aceleracionZ = 0.1; 		     //aceleracion eje Z
	 aceleracionZInversa = 0.05;	         //marcha atras
	 aceleracionFrenado = 0.15;		     //aceleracion eje X
	 tiempo = 0.5; 			 //Tiempo 
	 velocidadIni = 0;
	 posXIni = 0;
	 posZIni = 0;
	 velocidadX = 0;
	 velocidadZ = 0;
	 velocidad = 0;
	 velocidadMax = 5;
	 posX = 0;
	 posZ = 0;
	 adelante = false;
     atras = false;

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
    cuboPos.Z = posZ;
	cuboPos.X = posX;
    cuboNodo->setPosition(cuboPos);
}
void corredor::setAxis(ISceneManager *smgr){
	AxesSceneNode* axis = new AxesSceneNode(cuboNodo,smgr,-1);
   	axis->setAxesScale(20); //  for the length of the axes
  	axis->drop();
}
void corredor::setVelocidad(){
    velocidadIni = velocidad;
}
void corredor::setEspacio(){
    posXIni = posX;
	posZIni = posZ;
}
float corredor::getVelocidad(){
    return velocidad;
}
float corredor::getEspacioX(){
    return posX;
}
float corredor::getEspacioZ(){
    return posZ;
}
//-----------------------\*
//---MOVIMIENTO JUGADOR--\*
//-----------------------\* 
 void corredor::acelerar(){

        //cout<<"velocidad marcha adelante: "<<v<<"  "<<endl;

		//esto da un ligero retroceso al arrancar de nuevo (sin ser excesivo con v=-2,5)
		if (velocidadIni<0){
			velocidadIni=0;
		}
		cuboRot = cuboNodo->getRotation();
		//variamos la velocidad en funcion de la acceleracion (sin pasar de la velocidad max=5)
		if(velocidad< velocidadMax){
			velocidad = velocidadIni+aceleracionZ*tiempo;
		}
		//calculamos el desplazamiento en z y x, en funcion del angulo (radianes)
		posZ=posZIni+velocidad*cos((PI/180)*(cuboRot.Y));
		posX=posXIni+velocidad*sin((PI/180)*(cuboRot.Y));
		atras    = false;
		adelante = true;
    } 
     
//-----------------------\*
//---FRENADO JUGADOR---\*
//-----------------------\* 
 void corredor::frenar(){
     if(adelante ==true){
        posZ=posZIni+velocidad*cos((PI/180)*(cuboRot.Y));
		posX=posXIni+velocidad*sin((PI/180)*(cuboRot.Y));
        if(velocidad>-2.5f){	//Controla que no pase de 2.5 de velocidad en marcha atras cuando se frena
			velocidad = velocidadIni - aceleracionFrenado*tiempo;
			//	v = v -0.3;
		}
         //v = vIni -Afrenado*t;
     }
     else{
        //cout<<"velocidad marcha atras: "<<v<<"  "<<endl;
		atras    = true;
		adelante = false;
		if(velocidad>-2.5f && velocidad<2.5f){
			//v = vIni + Afrenado*t;
			velocidad = velocidadIni - aceleracionZInversa*tiempo;
			//	v = v -0.3;
		}
		
		posZ=posZIni+velocidad*cos((PI/180)*(cuboRot.Y));
		posX=posXIni+velocidad*sin((PI/180)*(cuboRot.Y));
     }   
    
		
					
}
//-----------------------\*
//------GIRAR JUGADOR----\*
//-----------------------\* 
void corredor::girarDerecha(){
    cuboRot = cuboNodo->getRotation();

	cuboRot.Y+=1;
	if(cuboRot.Y>180){
		cuboRot.Y=-179;
	}
	cuboNodo->setRotation(vector3df(cuboRot.X ,cuboRot.Y,cuboRot.Z));

}

void corredor::girarIzquierda(){
    cuboRot = cuboNodo->getRotation();

	cuboRot.Y-=1;
	if(cuboRot.Y<-180){
		cuboRot.Y=179;
	}
	cuboNodo->setRotation(vector3df(cuboRot.X ,cuboRot.Y,cuboRot.Z));

}

void corredor::resetGiro(){
	/*if (cuboRot.Y>0){
		cuboRot.Y-=0.5;
	}
	if (cuboRot.Y<0){
		cuboRot.Y+=0.5;
	}
	cuboNodo->setRotation(vector3df(cuboRot.X ,cuboRot.Y,cuboRot.Z));*/

}

//-----------------------\*
//---DESACELERAR JUGADOR--\*
//-----------------------\* 
void corredor::desacelerar(){
    //desaceleracion
	//X = Xi + Vi . t - 1/2 . a . t² 
	//V = Vi - a . t
	if(velocidad >0){
		velocidad = velocidadIni -aceleracionZInversa*tiempo;
		if(atras){	//esta parte no se si entra alguna vez XD
	
			posZ=posZIni-velocidad*cos((PI/180)*(cuboRot.Y));
			posX=posXIni-velocidad*sin((PI/180)*(cuboRot.Y));
		}
		if(adelante){ //desacelerar una vez el coche iba hacia adelante
			posZ=posZIni+velocidad*cos((PI/180)*(cuboRot.Y));
			posX=posXIni+velocidad*sin((PI/180)*(cuboRot.Y));					
		}
		
    }
}
 void corredor::escalar(float tam){
        core::vector3d<f32> factorEscalado(tam,tam,tam);
	    cuboNodo->setScale(factorEscalado);
	}