#ifndef CORREDORIA_H
#define CORREDORIA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"

using namespace std;

class CorredorIA : public Corredor
{
  public:
	CorredorIA(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo);

	void movimiento();
	void reposicionar();
	void calculoAnguloGiro(btVector3 posicion);
	void ActualizarRaytest();

	//Logica difusa
	void logicaDifusa();
	void acelerarGirar();
	double FuncionTrapezoidal(double valor, double a, double b, double c, double d);
	double FuncionTriangular(double valor, double a, double b, double c);
	void vision(btScalar distancia,ISceneNode *nodo);
	void seguirWaypoint();
	void visionArbol();


  private:

	btScalar anguloGiro;
	btScalar distanciaObjetivo;
	double pertenenciaCerca,pertenenciaMedia,pertenenciaLejos;
	double pertenenciaGiroFuerteDerecha,pertenenciaGiroFlojoDerecha,pertenenciaNoGiro,pertenenciaGiroFuerteIzquierda,pertenenciaGiroFlojoIzquierda;
	bool distanciaCerca,distanciaMedia,distanciaLejos;
	bool giroFuerteDerecha,giroFlojoDerecha,noGiro,giroFuerteIzquierda,giroFlojoIzquierda;
	
	bool caja,turbo,enemigo;
	float distanciaCaja,distanciaTurbo,distanciaEnemigo;

	
	btVector3 posicionCaja;
	btVector3 posicionTurbo;
	btVector3 posicionEnemigo;
	btVector3 posicion_aux;
	int timerRecolocar;
};

#endif /* CORREDORIA_H */