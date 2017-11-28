#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "irrlichtlib.hpp"
using namespace std;

class corredor
{
  public:
	corredor(ISceneManager *smgr, stringw rutaObj,s32 id_colision);
	//--movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	float movimiento(float pos, float vel, float accel, float delta);

	//auxiliares
	void escalar(float tam);
	void setAxis(ISceneManager *smgr);
	void moveCameraControl(IrrlichtDevice *device);
	//Update
	void updatePosicion();
	void updateVelocidadInicial();
	void updatePosicionInicial();
	void update();
	void resetGiro();
	//metodos SET

	//metodos GET
	vector3df getPosicion();
	vector3df getRotacion();
	IMeshSceneNode* getNodo();
	float getVelocidad();

  private:
	// aceleraciones
	float aceleracion, aceleracionInversa, aceleracionFrenado, tiempo;
	//velocidades
	float velocidadIni, velocidad, velocidadX, velocidadZ, velocidadMax;
	//posiciones
	float posXIni, posZIni, posYIni, posX, posZ, posY;
	bool adelante, atras;
	// rotaciones
	float Rotacionruedas,Rotacioncoche, rotRuedasY, rotCocheY,anguloMaxRuedas,anguloMaxCoche;
	//objetos
	IMesh          *coche;
	IMeshSceneNode *cuboNodo;
	IMeshSceneNode *ruedasDelanteras;
	IMeshSceneNode *ruedasTraseras;
};

#endif /* JUGADOR_H */