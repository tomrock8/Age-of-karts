#ifndef MOTORFISICAS_H
#define MOTORFISICAS_H

#include <iostream>
#include <string>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"
#include "IrrlichtLib.hpp"

using namespace std;

class MotorFisicas
{
  public:
	//Destructor
	~MotorFisicas(void);
	//metodos GET
	static MotorFisicas *getInstancia();
	//void setMapaFisicas(stringw fichero);
	btDynamicsWorld *getMundo();
	irr::core::list<btRigidBody *> getObjetos();
	void setObjetos(irr::core::list<btRigidBody *> objetos);
	void setFisicas(const char *f);

  private:
	MotorFisicas(void);
	static MotorFisicas *instancia;
	const char *fisicas;
	btBroadphaseInterface *broadPhase;
	btDefaultCollisionConfiguration *confColision;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;
	btBulletWorldImporter *fileLoader;
	core::list<btRigidBody *> objetos;
	btDynamicsWorld *mundo;
};

#endif /* MOTORFISICAS_H */