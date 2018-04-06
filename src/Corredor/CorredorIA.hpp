#ifndef CORREDORIA_H
#define CORREDORIA_H

#include <iostream>
#include "Corredor.hpp"
#include "Waypoint.hpp"
#include "ArbolDecision.hpp"

using namespace std;
typedef enum {
	NADA1, ACELERAR, FRENAR,
	ACELERARGIRARDERECHA, ACELERARGIRARIZQUIERDA, ACELERARGIRARFUERTEDERECHA, ACELERARGIRARFUERTEIZQUIERDA,
	FRENARGIRARFUERTEDERECHA, FRENARGIRARFUERTEIZQUIERDA,
	DISTANCIAOBJETIVOCAJA, DISTANCIAOBJETIVOENEMIGO, DISTANCIAOBJETIVOTURBO,
	SEGUIRWAYPOINT, USAROBJETO, USARHABILIDAD
} acciones_IA;


class CorredorIA : public Corredor
{
public:
	CorredorIA(const char* rutaObj, btVector3 pos, Corredor::tipo_jugador tipo);
	~CorredorIA();
	void deleteArbol();

	virtual void movimiento() override;
	virtual void updateHijos() override;
	virtual void actualizarItem() override;
	void reposicionar();
	void calculoAnguloGiro(btVector3 posicion);
	void ActualizarRaytest();
	void comprobarDireccion(obj3D *nodo);
	//Logica difusa
	void logicaDifusa();
	double FuncionTrapezoidal(double valor, double a, double b, double c, double d);
	double FuncionTriangular(double valor, double a, double b, double c);
	void vision(btScalar distancia, obj3D *nodo);
	void seguirWaypoint();
	void setDebugFisicas(bool activo);
	bool getDebugFisicas();
	void debugIA();

private:

	btScalar anguloGiro;
	btScalar distanciaObjetivo;
	double pertenenciaCerca, pertenenciaMedia, pertenenciaLejos;
	double pertenenciaGiroFuerteDerecha, pertenenciaGiroFlojoDerecha, pertenenciaNoGiro, pertenenciaGiroFuerteIzquierda, pertenenciaGiroFlojoIzquierda;
	double pertenenciaVelocidadBaja, pertenenciaVelocidadMedia, pertenenciaVelocidadAlta;

	bool distanciaCerca, distanciaMedia, distanciaLejos;
	bool velocidadBaja, velocidadMedia, velocidadAlta;
	bool giroFuerteDerecha, giroFlojoDerecha, noGiro, giroFuerteIzquierda, giroFlojoIzquierda;
	bool caja, turbo, enemigo, Vision, Objeto;
	bool Waypoint;
	bool enemigoatras;
	bool enemigolados;
	bool habilidad;
	bool debugFisicas;

	btScalar distanciaCaja, distanciaTurbo, distanciaEnemigo;

	ArbolDecision *arbolconduccion;
	ArbolDecision *arbolobjetos;

	int accionActualConduccion;
	int accionActualObjetos;
	btVector3 posicionCaja;
	btVector3 posicionTurbo;
	btVector3 posicionEnemigo;
	btVector3 posicion_aux;
	int timerRecolocar;
};

#endif /* CORREDORIA_H */