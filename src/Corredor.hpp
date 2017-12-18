#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "MotorFisicas.hpp"
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

	//metodos GET
	IMeshSceneNode* getNodo();
	float getVelocidad();
	btRaycastVehicle* getVehiculo();
	btRigidBody * getRigidBody();

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