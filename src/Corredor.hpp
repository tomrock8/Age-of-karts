#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "Waypoint.hpp"
#include "Proyectil.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

using namespace std;

class Corredor
{
public:

	Corredor(stringw rutaObj,vector3df pos);
	void InicializarFisicas(list<btRigidBody*> &objetos, btDiscreteDynamicsWorld *mundo);
	void BorrarFisicas();
	void CrearRuedas(btRaycastVehicle* vehiculo,btRaycastVehicle::btVehicleTuning tuning);
	//--movimiento del corredor
	virtual void movimiento() {}; // A implementar por derivadas

	//Update
	void actualizarRuedas();
	void actualizarMotionState();
	void Pedazodemierda();
	void lanzarItem(Proyectil *&item,btDiscreteDynamicsWorld *mundo,core::list<btRigidBody *> &objetos);
	//metodos GET
	IMeshSceneNode* getNodo();
	float getVelocidad();
	btRaycastVehicle* getVehiculo();
	btRigidBody * getRigidBody();
	int getCargador(){return cargador;};
	void incCargador(){cargador++;};
	void decCargador(){cargador--;};

	// Metodos SET
	//void setAxis();

	// Metodos GET

	//std::string getDireccion();
	//int getDireccionGrados();

	//std::string toString();


protected:

	//orientacion
	vector3df orientacion;

	//Mascaras de colision
	
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
	btRigidBody *CuerpoColisionChasis;
	btCompoundShape* CentroGravedad;
	int cargador=0;

	
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
	//bool norte, sur, este, oeste;
	//int direccionGrados;

	// Movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	void frenodemano();
	//void updateDireccionGrados();

};

#endif /* JUGADOR_H */