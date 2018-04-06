#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include <string>
#include <vector>
#include "Waypoint.hpp"
#include "Proyectil.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "MotorFisicas.hpp"
#include "ItemTeledirigido.hpp"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "RaknetIncludes.hpp"
#include "Escudo.hpp"
#include "EstadosJugador.hpp"
#include "Habilidad.hpp"
#include "ItemTeledirigido.hpp"
#include "TMotor.hpp"


using namespace std;


class Corredor : public RakNet::NetworkIDObject
{
public:
	typedef enum { GLADIADOR, PIRATA, VIKINGO, CHINO } tipo_jugador;
	Corredor(const char* rutaObj, btVector3 pos, tipo_jugador tipo);
	void InicializarFisicas();
	void resetFuerzas();
	void limitadorVelocidad();
	// Destructor
	~Corredor();

	void acelerar();
	void recolocarWaypoint();

	//objetos
	void lanzarItemTeledirigido();
	void aplicarAceite();
	void soltarItem();
	void incCargador();
	void decCargador();

	//metodos SET
	void setID(int i);
	void setAceite();
	void setObjetivoTelederigido();
	void setParametros();
	void setTipoObj();
	void setTipoObj(int i);
	void SetFuerzaVelocidad(int turbo);
	void setFriccion(btScalar valor);
	void setTurbo(bool activo, bool objeto, int valor, int tiempo);
	void setInmunidad(bool activo);
	void setHabilidad(bool activo);
	void setWaypointActual(obj3D *nodo);
	void setProteccion(bool s);
	void setPosicion(float *pos, float *ori);
	void setLimite(int s);
	void setTipoJugador(int tj);
	void setVueltas(int j);
	void setTiempoVuelta(float t);


	std::string toString();


	//waypoints
	void setPosicionCarrera(int i, int j);
	int getPosicionCarrera();
	void setMaxVueltas(int i);
	btScalar getDistanciaPunto(btVector3 vector);
	btScalar getdistanciaWaypoint();
	btScalar getdistanciaWaypointActual();
	int getVueltas();
	float getMaxVuetas();
	float getTiempoVueltaTotal();

	// Update
	void update();
	void updateTimerObstaculos();
	void updateEstado();
	void updateText();
	void comprobarSueloRuedas();
	void comprobarTurbo();
	void comprobarInmunidad();
	virtual void actualizarItem() = 0;
	void actualizarPosicion();

	// Metodos GET
	obj3D *getNodo();
	btRaycastVehicle *getVehiculo();
	btRigidBody *getRigidBody();
	btVector3 getVectorDireccion();
	const char* getNombre();
	int getID();
	int getCargador();
	int getTipoObj();
	bool getTurbo();
	Waypoint *getWaypointActual();
	Waypoint *getWaypointSiguiente();
	Waypoint *getWaypointAnterior();
	bool getProteccion();
	EstadosJugador *getEstados();
	int getLimite();
	bool getAceiteActivado();
	tipo_jugador getTipoJugador();
	bool getInmunidad();

	//estado de los objetos a usar
	void setCheckItem(bool s);
	bool getCheckItem();
	void setPosDisparo(btVector3 posDisparo);
	void usarObjetos();
	void lanzarHabilidad();




protected:

	//Irrlicht
	obj3D * cuboNodo;
	obj3D *rueda1;
	obj3D *rueda2;
	obj3D *rueda3;
	obj3D *rueda4;

	//WAYPOINTS
	Waypoint *anterior; // Punto Actual
	Waypoint *actual; // Punto Actual
	Waypoint *siguiente; // Punto Siguiente
	Waypoint *siguiente_aux; // Punto Siguiente
	int vueltas;
	int posicionCarrera;
	int maxvueltas;

	// Control de vueltas
	btScalar distanciaWaypoint;
	btScalar distanciaWaypointActual;
	btScalar distanciaWaypointAnterior;

	//bullet
	btRaycastVehicle *vehiculo;
	btDefaultMotionState *motionStateCoche; //interpolacion
	btCollisionShape *FormaColision;		//contornoB
	btRigidBody *CuerpoColisionChasis;
	btCompoundShape *CentroGravedad;

	const char* nombre;
	int id;
	int cargador;
	int tipoObj; // si es 0 no tengo nada
	bool turboActivado; // para saber cuando esta activado turbo
	bool objetivoFijado; //para el item teledirigido
	bool aceiteActivado; //para el item teledirigido
	int timerTurbo;
	int tiempoTurbo;
	int timerTeledirigido;
	int timerAceite;
	int timerInmunidad;
	int tiempoInmunidad;

	bool inmunidad;
	bool habilidadLanzada;
	float tiempoHabilidad;
	float tiempoVuelta;
	float tiempoVueltaTotal;
	//objetos estaticos y dinamicos

	EstadosJugador *estado;
	btVector3 posDisparo;

	//tipo jugador
	tipo_jugador tipojugador;

	int limite;
	//estados de lanzamiento de objeto
	bool checkItem;

	int velocidadMedia;
	int velocidadMaxima;
	int velocidadMaximaTurbo;
	int velocidadMaximaAtras;
	int velocidadLimiteGiro;
	int direccionContraria;


	btScalar indiceGiroAlto;
	btScalar indiceGiroBajo;
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
	void frenodemano(bool activo, bool objeto);

	virtual void movimiento() = 0; // A implementar por derivadas
	virtual void updateHijos() = 0;

	// UPDATES
	void actualizarRuedas();
	void updateVectorDireccion();

};

#endif /* CORREDOR_H */