#ifndef TURBO_H
#define TURBO_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Corredor.hpp"
#include "GestorIDs.hpp"
class Turbo
{
public:

	Turbo(btVector3 pos, bool estado);

	// METODOS SET
	void setTurboActivo(Corredor *corredor, bool s);
	void setFrenadaActivo(Corredor *corredor, bool s);
	void setTurbo(bool valor);
	void setTurboTocado(bool s);

	// METODOS GET
	bool getTurboActivo();
	int getTiempoTurbo();
	int getID();
	//destructor
	~Turbo();
private:
	IMeshSceneNode * turbo;
	btVector3 escala;
	btTransform transform;
	btDefaultMotionState *MotionState;
	btVector3 halfExtents;
	btCollisionShape *Shape;
	btVector3 localInertia;
	btRigidBody *rigidBody;
	bool turboActivo;
	bool turboTocado;//para que solo se active la comprobacion 1 vez
	int tiempo;
	int id;

};

#endif /* TURBO */