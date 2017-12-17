#ifndef MOTORFISICAS_H
#define MOTORFISICAS_H

#include <iostream>
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
	btDynamicsWorld* getMundo();
	

private:
	MotorFisicas(void);
	static MotorFisicas* instancia;
	btBroadphaseInterface *broadPhase;
	btDefaultCollisionConfiguration *confColision;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;
	//btBulletWorldImporter* fileLoader;
	btDynamicsWorld *mundo;
};

#endif /* MOTORFISICAS_H */