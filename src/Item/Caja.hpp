#ifndef CAJA_H
#define CAJA_H

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

class Corredor;
class obj3D;
class AlSource;

class Caja
{
public:
	Caja(btVector3 posicionCaja);
	~Caja();
	void romper(Corredor* pj1Col);
	void comprobarRespawn();
	void Delete();
	btRigidBody* inicializarFisicas();

	obj3D* getNodo();
	btRigidBody* getRigidBody();
	const char* getNombre();
	int getID();


private:

	obj3D* nodo;
	btRigidBody* rigidBody;
	btScalar masa;
	btVector3 escala;
	btVector3 posicion;
	const char* nombre;
	int id;
	int timer;

	AlSource* fuenteCaja;
};

#endif