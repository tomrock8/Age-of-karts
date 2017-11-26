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
	void moveCameraControl(IrrlichtDevice *device);
	
	
  private:
    float aceleracionZ,aceleracionZInversa,aceleracionFrenado, tiempo, velocidadIni, posXIni, posZIni, velocidad,velocidadX,velocidadZ,velocidadMax, posX, posZ, posY,anguloRotCoche,anguloRotRueda;    		
	bool adelante, atras;
	IMesh* coche;
	IMeshSceneNode *cuboNodo;
	IMeshSceneNode *ruedasDelanteras;
	IMeshSceneNode *ruedasTraseras;
	vector3df cuboPos;
	vector3df cuboRot;
	vector3df rotRuedas;
		
};

#endif /* JUGADOR_H */