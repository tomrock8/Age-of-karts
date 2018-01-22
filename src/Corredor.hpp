#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include <string>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"
#include "Proyectil.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "MotorFisicas.hpp"
#include "Proyectil.hpp"
#include "Estatico.hpp"
#include "TextoPantalla.hpp"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "TextoPantalla.hpp"
#include "Escudo.hpp"
#include "EstadosJugador.hpp"

using namespace std;

class Corredor {
public:
	Corredor(stringw rutaObj, vector3df pos);
	void InicializarFisicas();
	void lanzarItem(Proyectil *item, int direcionItem);
	void aplicarAceite();
	void soltarItem();
	virtual void actualizarItem(){}; // A implementar por derivadas
	void incCargador();
	void decCargador();
	void SetFuerzaVelocidad(int turbo);
	std::string toString();
	void setTipoObj();
	void setTipoObj(int i);
	void acelerar();
	void setFriccion(btScalar valor);
	void setTurbo(bool activo, bool objeto,int valor);
	void setWaypointActual(ISceneNode *nodo);
	void setProteccion(bool s);
	

	//waypoints
	void calculoDistanciaPunto();
	void calculoAnguloGiro();
	void giroIA();
	void movimientoIA();
	void ActualizarRaytest();


	// Logica difusa
	void logicaDifusa();
	double FuncionTrapezoidal(double valor, double a, double b, double c, double d);
	double FuncionTriangular(double valor, double a, double b, double c);

	// Update
	void update();
	void updateEstado();
	// Metodos GET
	IMeshSceneNode *getNodo();
	btRaycastVehicle *getVehiculo();
	btRigidBody *getRigidBody();
	std::string getDireccion();
	int getDireccionGrados();
	vector3df getVectorDireccion();
	int getCargador();
	int getTipoObj();
	bool getTurbo();
	Waypoint *getWaypointActual();
	bool getProteccion();


	// Destructor
	~Corredor();

protected:
	//objetos
	IMesh * coche;
	
	IMeshSceneNode *cuboNodo;
	ISceneNode *rueda1;
	ISceneNode *rueda2;
	ISceneNode *rueda3;
	ISceneNode *rueda4;

	//WAYPOINTS
	Waypoint *actual; // Punto Actual
	Waypoint *siguiente; // Punto Siguiente

	// parametros IA
	btScalar anguloGiro;
	btScalar distanciaWaypoint;
	double pertenenciaCerca,pertenenciaMedia,pertenenciaLejos;
	double pertenenciaGiroFuerteDerecha,pertenenciaGiroFlojoDerecha,pertenenciaNoGiro,pertenenciaGiroFuerteIzquierda,pertenenciaGiroFlojoIzquierda;
	bool distanciaCerca,distanciaMedia,distanciaLejos;
	bool giroFuerteDerecha,giroFlojoDerecha,noGiro,giroFuerteIzquierda,giroFlojoIzquierda;

	//bullet
	btRaycastVehicle *vehiculo;
	btDefaultMotionState *motionStateCoche; //interpolacion
	btCollisionShape *FormaColision;		//contornoB
	btRigidBody *CuerpoColisionChasis;


	btCompoundShape *CentroGravedad;

	//Objetos
	int cargador;
	int tipoObj;
	bool turboActivado;
	int timerTurbo;
	Escudo *escudo;
	EstadosJugador *estado;

	btVector3 posicion;
	btVector3 direccionRuedas;
	btVector3 rotacionRuedas;
	btScalar suspension;
	btScalar anchoRueda;
	btScalar radioRueda;
	btScalar alturaConexionChasis;
	btScalar Masa;
	btScalar Fuerza;
	btScalar FuerzaMaxima;
	btScalar FuerzaFrenado;
	btScalar FuerzaFrenoMano;
	btScalar FuerzaGiro;
	btScalar FuerzaFrenadoReposo;
	//escudo
	bool proteccion;
	//raycast
	btVehicleRaycaster *RayCastVehiculo;

	// Direccion - Orientacion
	bool norte, sur, este, oeste;
	float direccionGrados;
	vector3df orientacion;

	void CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning);
	void BorrarFisicas();

	// Movimiento del corredor
	//void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	void frenodemano(bool activo);
	virtual void movimiento() {}; // A implementar por derivadas

	// UPDATES
	void actualizarRuedas();
	void updateDireccion();
	void updateVectorDireccion();
};

#endif /* CORREDOR_H */