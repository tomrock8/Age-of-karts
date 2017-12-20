#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"
#include "Proyectil.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "MotorFisicas.hpp"
#include "Proyectil.hpp"
using namespace std;

class Corredor
{
public:
	
	Corredor(stringw rutaObj,vector3df pos);
	void InicializarFisicas();
	void BorrarFisicas();
	void CrearRuedas(btRaycastVehicle* vehiculo,btRaycastVehicle::btVehicleTuning tuning);

	//--movimiento del corredor
	virtual void movimiento() {}; // A implementar por derivadas

	//Update
	void actualizarRuedas();
	void actualizarMotionState();
	void lanzarItem(Proyectil *item);
	//metodos GET
	IMeshSceneNode* getNodo();
	float getVelocidad();
	void SetFuerzaVelocidad(int turbo);
	btRaycastVehicle* getVehiculo();
	btRigidBody * getRigidBody();

	int getCargador();
	void incCargador();
	void decCargador();

	std::string toString();

	//metodos GET
	IMeshSceneNode *getNodo();
	btRaycastVehicle *getVehiculo();
	btRigidBody *getRigidBody();
	std::string getDireccion();
	int getDireccionGrados();
	vector3df getVectorDireccion();
	int getCargador() { return cargador; };
	void incCargador() { cargador++; };
	void decCargador() { cargador--; };

  protected:
	//objetos
	IMesh          *coche;
	IMeshSceneNode *cuboNodo;
	ISceneNode* rueda1;
	ISceneNode*	rueda2;
	ISceneNode* rueda3;
	ISceneNode* rueda4;
	//list<ISceneNode*> Ruedas;

	//bullet
	btRaycastVehicle* vehiculo;
	btDefaultMotionState *motionStateCoche;//interpolacion
	btCollisionShape *FormaColision;//contorno
	btScalar Masa;//masa
	btRigidBody *CuerpoColisionChasis;

	int fuerzaVelocidad;

	btCompoundShape *CentroGravedad;
	int cargador = 0;

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
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	void frenodemano();

	// UPDATES
	void actualizarRuedas();
	void updateDireccion();
	void updateVectorDireccion();
	void updateDireccionGrados();


};

#endif /* JUGADOR_H */