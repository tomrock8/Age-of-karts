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
using namespace std;

class Corredor
{
  public:
	Corredor(stringw rutaObj, vector3df pos);
	void InicializarFisicas();
	void lanzarItem(Proyectil *item, int direcionItem);
	void soltarItem(Estatico *item);
	void incCargador();
	void decCargador();
	void SetFuerzaVelocidad(int turbo);
	std::string toString();
	void setTipoObj();
	void setTipoObj(int i);
	void acelerar();
	void setFriccion(btScalar valor);
	void setTurbo(bool activo, bool objeto);

	//Update
	void update();

	//metodos GET
	IMeshSceneNode *getNodo();
	btRaycastVehicle *getVehiculo();
	btRigidBody *getRigidBody();
	std::string getDireccion();
	int getDireccionGrados();
	vector3df getVectorDireccion();
	int getCargador();
	int getTipoObj();
	bool getTurbo();

  protected:
	//objetos
	IMesh *coche;
	IMeshSceneNode *cuboNodo;
	ISceneNode *rueda1;
	ISceneNode *rueda2;
	ISceneNode *rueda3;
	ISceneNode *rueda4;

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

	btVector3 posicion;
	btVector3 direccionRuedas;
	btVector3 rotacionRuedas;
	btScalar suspension;
	btScalar anchoRueda;
	btScalar radioRueda;
	btScalar alturaConexionChasis;
	btScalar Masa;
	btScalar Fuerza;
	btScalar FuerzaFrenado;
	btScalar FuerzaFrenoMano;
	btScalar FuerzaGiro;
	btScalar FuerzaFrenadoReposo;

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
	void frenodemano();
	virtual void movimiento(){}; // A implementar por derivadas

	// UPDATES
	void actualizarRuedas();
	void updateDireccion();
	void updateVectorDireccion();
	void updateDireccionGrados();
};

#endif /* CORREDOR_H */