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
	void setVelocidad();
	void setEspacio();
	//---recogida de variables
	float getEspacio();
	float getVelocidad();
	vector3df getPosition();
	vector3df getRotation();
	
  private:
    float aZ,aZInversa,Afrenado, t, vIni, xIni, v, x;    		
	bool adelante, atras ;
	IMesh* coche;
	IMeshSceneNode *cuboNodo;
	vector3df cuboPos;
		
};

#endif /* JUGADOR_H */