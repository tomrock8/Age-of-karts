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
#include "ItemTeledirigido.hpp"
#include "Estatico.hpp"
#include "TextoPantalla.hpp"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "RaknetIncludes.hpp"
#include "TextoPantalla.hpp"
#include "Escudo.hpp"
#include "EstadosJugador.hpp"
#include "Habilidad.hpp"

using namespace std;

class Corredor: public RakNet::NetworkIDObject 
{
  public:
	Corredor(stringw rutaObj, btVector3 pos);
	void InicializarFisicas();
	void lanzarItem(Proyectil *item, int direcionItem);
	void lanzarItemTeledirigido();
	void virtual lanzarItemRed(){};
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
	void setPosicion(float *pos, float *ori);
	void setLimite(int s);
	

	//waypoints
	void calculoDistanciaPunto();
	void calculoDistanciaPuntoActual();
	void calculoDistanciaPuntoAnterior();
	void setPosicionCarrera(int i);
	int getPosicionCarrera();
	btScalar getdistanciaWaypoint();
	btScalar getdistanciaWaypointAnterior();
	btScalar getdistanciaWaypointActual();
	int getVueltas();
	
	// Update
	void update();
	void updateEstado();
	// Metodos GET
	IMeshSceneNode *getNodo();
	btRaycastVehicle *getVehiculo();
	btRigidBody *getRigidBody();
	btVector3 getVectorDireccion();
	int getCargador();
	int getTipoObj();
	bool getTurbo();
	Waypoint *getWaypointActual();
	Waypoint *getWaypointSiguiente();
	Waypoint *getWaypointAnterior();
	bool getProteccion();
	EstadosJugador *getEstados();
	int getLimite();
	virtual void updateRed() {}; 


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
	Waypoint *anterior; // Punto Actual
	Waypoint *actual; // Punto Actual
	Waypoint *siguiente; // Punto Siguiente
	Waypoint *siguiente_aux; // Punto Siguiente
	int vueltas;
	int posicionCarrera;

	// parametros IA
	btScalar distanciaWaypoint;
	btScalar distanciaWaypointActual;
	btScalar distanciaWaypointAnterior;

	//bullet
	btRaycastVehicle *vehiculo;
	btDefaultMotionState *motionStateCoche; //interpolacion
	btCollisionShape *FormaColision;		//contornoB
	btRigidBody *CuerpoColisionChasis;
	btCompoundShape *CentroGravedad;

	//Objetos
	int cargador;
	int tipoObj; // si es 0 no tengo nada
	bool turboActivado; // para saber cuando esta activado turbo
	int timerTurbo;
	Escudo *escudo;
	EstadosJugador *estado;

	//habilidad 
	Habilidad * h;
	int limite;

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

	
	btVector3 orientacion;
	
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
	void updateVectorDireccion();
	void updateHabilidad();
};

#endif /* CORREDOR_H */