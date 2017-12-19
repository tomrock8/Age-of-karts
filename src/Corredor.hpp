#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"
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

	// Metodos SET
	//void setAxis();

	// Metodos GET

	//std::string getDireccion();
	//int getDireccionGrados();

	//std::string toString();


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
	btCompoundShape* CentroGravedad;
	int cargador;
	//fuerza de movimiento
	int fuerzaVelocidad;

	// Direccion
	//bool norte, sur, este, oeste;
	//int direccionGrados;

	// Movimiento del corredor
	//void acelerar();
	//void frenar();
	//void desacelerar();
	//void girarDerecha();
	//void girarIzquierda();

	//void updateDireccionGrados();

};

#endif /* JUGADOR_H */