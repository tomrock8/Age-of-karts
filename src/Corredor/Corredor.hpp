#ifndef CORREDOR_H
#define CORREDOR_H

#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  
#endif

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"

#include "RaknetIncludes.hpp"
#include "EstadosJugador.hpp"

class obj3D;
class Waypoint;
class TNodo;
class particleSystem;
class billboard;
class AlSource;

class Corredor : public RakNet::NetworkIDObject
{
public:
	enum tipo_jugador { GLADIADOR, PIRATA, VIKINGO, CHINO };
	/* Animacion de burla /vacile comentada para ahorrar memoria
	typedef enum { PARADO, GIRODERINI, GIRODERFIN, GIROIZQINI, GIROIZQFIN,HABILIDAD,LANZAROBJETO,VACILE} movimiento_jugador;
	*/
	enum movimiento_jugador { PARADO, GIRODERINI, GIRODERFIN, GIROIZQINI, GIROIZQFIN, HABILIDAD, LANZAROBJETO };
	Corredor(btVector3 pos, tipo_jugador tipo);

	void InicializarFisicas();
	void InicializarFisicasRuedas();
	void resetFuerzas();
	void limitadorVelocidad();
	// Destructor
	virtual ~Corredor();

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
	void setTipoObj(EstadosJugador::estado_objeto objeto);
	void SetFuerzaVelocidad(int turbo);
	void setFriccion(btScalar valor);
	void setTurbo(bool activo, bool objeto, int valor, int tiempo);
	void setInmunidad(bool activo);
	void setHabilidad(bool activo);
	void setWaypointActual(obj3D* nodo);
	void setProteccion(bool s);
	void setPosicion(float* pos, float* ori);
	void setPosicionBullet(float* pos, btQuaternion ori);
	void setPosicionSources();
	void setLimite(int s);
	void setTipoJugador(int tj);
	void setVueltas(int j);
	void setTiempoVuelta(float t);
	void setParametrosDebug(float fuerza, float velocidadMedia, float velocidadMaximaTurbo, float velocidadMaxima, float masa, float indiceGiroAlto, float indiceGiroBajo, float velocidadLimiteGiro);
	void setParametrosRuedasDebug(float suspensionStiffness, float DampingCompression, float DampingRelaxation, float frictionSlip, float rollInfluence, float suspForce, float suspTravelCm);

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
	float getTiempoVuelta();
	void getParametrosDebug(float* fuerza, float* velocidadMedia, float* velocidadMaximaTurbo, float* velocidadMaxima, float* masa, float* indiceGiroAlto, float* indiceGiroBajo, float* velocidadLimiteGiro);
	void getParametrosRuedasDebug(float* suspensionStiffness, float* DampingCompression, float* DampingRelaxation, float* frictionSlip, float* rollInfluence, float* suspForce, float* suspTravelCm);

	// Update
	void update();
	void updateTimerObstaculos();
	void updateEstado();
	void comprobarSueloRuedas();
	void comprobarTurbo();
	void comprobarInmunidad();
	virtual void actualizarItem() = 0;

	// Metodos GET
	obj3D* getNodo();
	btRaycastVehicle* getVehiculo();
	btRigidBody* getRigidBody();
	btVector3 getVectorDireccion();
	const char* getNombre();
	int getID();
	int getCargador();
	int getTipoObj();
	bool getTurbo();
	Waypoint* getWaypointActual();
	Waypoint* getWaypointSiguiente();
	bool getProteccion();
	EstadosJugador* getEstados();
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

	std::string toString();

	//movimiento de animaciones
	void iniciarAnimacion(movimiento_jugador mov, TNodo* previo, TNodo* previoGiro);

protected:
	obj3D* cuboNodo;
	obj3D* rueda1;
	obj3D* rueda2;
	obj3D* rueda3;
	obj3D* rueda4;

	//BILLBOARD
	billboard* b;

	//PARTICULAS HUMO
	particleSystem* p;

	//VARIABLES OPENAL SONIDO

	AlSource* fuenteMotor;
	AlSource* fuenteItem;
	AlSource* fuenteFrenos;
	float pitchMotor;

	//WAYPOINTS
	Waypoint* actual; // Punto Actual
	Waypoint* siguiente; // Punto Siguiente
	Waypoint* siguiente_aux; // Punto Siguiente
	int vueltas;
	int posicionCarrera;
	int maxvueltas;

	// Control de vueltas
	btScalar distanciaWaypoint;
	btScalar distanciaWaypointActual;

	//bullet
	btRaycastVehicle* vehiculo;
	btRigidBody* CuerpoColisionChasis;

	//raycast
	btVehicleRaycaster* RayCastVehiculo;
	btVector3 orientacion;

	//RUEDAS
	btRigidBody* CuerpoColisionRueda1;
	btRigidBody* CuerpoColisionRueda2;
	btRigidBody* CuerpoColisionRueda3;
	btRigidBody* CuerpoColisionRueda4;


	const char* nombre;
	int id;
	int cargador;
	EstadosJugador::estado_objeto tipoObj; // si es 0 no tengo nada
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

	EstadosJugador* estado;
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


	//Animaciones
	TNodo* quieto;
	TNodo* giroDerIni;
	TNodo* giroDerFin;
	TNodo* giroIzqIni;
	TNodo* giroIzqFin;
	//TNodo *vacile;
	//TNodo *animHabilidad;
	//TNodo *lanzarObjeto;

	void CrearRuedas(btRaycastVehicle* vehiculo, btRaycastVehicle::btVehicleTuning tuning);
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

	TNodo* getGiroDerIni();
	TNodo* getGiroDerFin();
	TNodo* getGiroIzqIni();
	TNodo* getGiroIzqFin();
	TNodo* getAnimQuieto();
	//TNodo * getHabilidadAnim();
	//TNodo * getLanzarObjeto();
	//TNodo * getVacile();
};

#endif /* CORREDOR_H */


