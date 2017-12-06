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


	virtual void movimiento() {}; // A implementar por derivadas

	// Auxiliares
	void moveCameraControl();
	void resetGiro();

	// Update
	void updatePosicion();
	void updateVelocidadInicial();
	void updatePosicionInicial();
	void update();
	void updateDireccion();


	// Metodos SET
	void setAxis();
	void setEscala(float tam);
	void setColor(float valor1, float valor2, float valor3);
	ITriangleSelector *setColisiones(ITriangleSelector *selector);

	// Metodos GET
	vector3df getPosicion();
	vector3df getRotacion();
	IMeshSceneNode *getNodo();

	float getVelocidad();
	std::string getDireccion();


protected:

	// Aceleraciones
	float aceleracion, aceleracionInversa, aceleracionFrenado, tiempo;

	// Velocidades
	float velocidadIni, velocidad, velocidadX, velocidadZ, velocidadMax;

	// Posiciones
	float posXIni, posZIni, posYIni, posX, posZ, posY;
	bool adelante, atras;

	// Rotaciones
	float Rotacionruedas, Rotacioncoche, rotRuedasY, rotCocheY, anguloMaxRuedas, anguloMaxCoche;

	// Direccion
	bool norte, sur, este, oeste;


	// Objetos irrlicht
	IMesh *coche;
	IMeshSceneNode *cuboNodo;
	IMeshSceneNode *ruedasDelanteras;
	IMeshSceneNode *ruedasTraseras;
	void enEscena(stringw rutaObj, s32 id_colision); // Implementa todos los elementos de irrlicht


	// Movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();

};

#endif /* JUGADOR_H */