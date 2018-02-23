#ifndef CORREDORIA_H
#define CORREDORIA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"
#include "ArbolDecision.hpp"

using namespace std;
typedef enum {NADA1,ACELERAR,FRENAR,ACELERARGIRARDERECHA,ACELERARGIRARIZQUIERDA,ACELERARGIRARFUERTEDERECHA,ACELERARGIRARFUERTEIZQUIERDA,
FRENARGIRARFUERTEDERECHA,FRENARGIRARFUERTEIZQUIERDA,
DISTANCIAOBJETIVOCAJA,DISTANCIAOBJETIVOENEMIGO,DISTANCIAOBJETIVOTURBO,SEGUIRWAYPOINT,USAROBJETO} acciones_IA;


class CorredorIA : public Corredor
{
  public:
	CorredorIA(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo);

	void movimiento();
	void reposicionar();
	void calculoAnguloGiro(btVector3 posicion);
	void ActualizarRaytest();
	void comprobarDireccion(ISceneNode *nodo);
	//Logica difusa
	void logicaDifusa();
	double FuncionTrapezoidal(double valor, double a, double b, double c, double d);
	double FuncionTriangular(double valor, double a, double b, double c);
	void vision(btScalar distancia,ISceneNode *nodo);
	void seguirWaypoint();
	


  private:

	btScalar anguloGiro;
	btScalar distanciaObjetivo;
	double pertenenciaCerca,pertenenciaMedia,pertenenciaLejos;
	double pertenenciaGiroFuerteDerecha,pertenenciaGiroFlojoDerecha,pertenenciaNoGiro,pertenenciaGiroFuerteIzquierda,pertenenciaGiroFlojoIzquierda;
	double pertenenciaVelocidadBaja,pertenenciaVelocidadMedia,pertenenciaVelocidadAlta;
	
	bool distanciaCerca,distanciaMedia,distanciaLejos;
	bool velocidadBaja,velocidadMedia,velocidadAlta;
	bool giroFuerteDerecha,giroFlojoDerecha,noGiro,giroFuerteIzquierda,giroFlojoIzquierda;
	bool caja,turbo,enemigo,Vision,Objeto;
	bool Waypoint;

	btScalar distanciaCaja,distanciaTurbo,distanciaEnemigo;

	ArbolDecision *arbolconduccion;
	ArbolDecision *arbolobjetos;		
	

	btVector3 posicionCaja;
	btVector3 posicionTurbo;
	btVector3 posicionEnemigo;
	btVector3 posicion_aux;
	int timerRecolocar;
};

#endif /* CORREDORIA_H */