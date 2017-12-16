#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

using namespace std;

class Corredor
{
  public:
	Corredor(stringw rutaObj, vector3df pos);
	void InicializarFisicas(list<btRigidBody *> &objetos, btDiscreteDynamicsWorld *mundo);
	std::string toString();

	//Update
	void update();

	//Metodos GET
	IMeshSceneNode *getNodo();
	float getVelocidad();
	btRaycastVehicle *getVehiculo();
	btRigidBody *getRigidBody();
	std::string getDireccion();
	int getDireccionGrados();

  protected:
	//orientacion
	vector3df orientacion;

	//Mascaras de colision

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
	btCollisionShape *FormaColision;		//contorno
	btRigidBody *CuerpoColisionChasis;
	btCompoundShape *CentroGravedad;

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

	// Direccion
	bool norte, sur, este, oeste;
	float direccionGrados;

	void CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning);
	void BorrarFisicas();

	// Movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	void frenodemano();

	// UPDATES
	void actualizarRuedas();
	void updateDireccion();
	void Pedazodemierda();
	void updateDireccionGrados();

	//--movimiento del corredor
	virtual void movimiento(){}; // A implementar por derivadas
};

#endif /* JUGADOR_H */