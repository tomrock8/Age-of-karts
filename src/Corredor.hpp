#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"

using namespace std;

class Corredor
{
public:
	Corredor(stringw rutaObj, s32 id_colision);

	//--movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	void movimiento();

	//auxiliares
	void escalar(float tam);
	void setAxis(ISceneManager *smgr);
	void moveCameraControl(IrrlichtDevice *device);
	void cambiarColor(float valor1, float valor2, float valor3, ISceneManager *smgr);

	//Update
	void updatePosicion();
	void updateVelocidadInicial();
	void updatePosicionInicial();
	void update();
	void resetGiro();

	//metodos SET
	ITriangleSelector *setColisiones(IrrlichtDevice *device, ITriangleSelector *selector);

	//metodos GET
	vector3df getPosicion();
	vector3df getRotacion();
	IMeshSceneNode *getNodo();
	float getVelocidad();


protected:

	// aceleraciones
	float aceleracion, aceleracionInversa, aceleracionFrenado, tiempo;

	//velocidades
	float velocidadIni, velocidad, velocidadX, velocidadZ, velocidadMax;

	//posiciones
	float posXIni, posZIni, posYIni, posX, posZ, posY;
	bool adelante, atras;

	// rotaciones
	float Rotacionruedas, Rotacioncoche, rotRuedasY, rotCocheY, anguloMaxRuedas, anguloMaxCoche;

	//objetos
	IMesh *coche;
	IMeshSceneNode *cuboNodo;
	IMeshSceneNode *ruedasDelanteras;
	IMeshSceneNode *ruedasTraseras;

};

#endif /* JUGADOR_H */