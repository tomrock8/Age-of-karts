#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream> 
#include "irrlichtlib.hpp"
using namespace std;
 
class corredor {
public:
	corredor(ISceneManager* smgr, stringw rutaObj);
	//--movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	float movimiento(float pos,float vel,float accel,float delta);
	//--actualizacion de variables----
	void escalar (float tam);
	void actualizarPos();
	void setAxis(ISceneManager *smgr);
	void setVelocidad();
	void setEspacio();
	void resetGiro();
	//---recogida de variables
	float getEspacioX();
	float getEspacioZ();
	float getVelocidad();
	vector3df getPosition();
	vector3df getRotation();
	
	
  private:
    float aZ,aZInversa,Afrenado, t, vIni, xIni, zIni, v,vx,vz, x, z;    		
	bool adelante, atras ;
	IMesh* coche;
	IMeshSceneNode *cuboNodo;
	vector3df cuboPos;
	vector3df cuboRot;
		
};

#endif /* JUGADOR_H */