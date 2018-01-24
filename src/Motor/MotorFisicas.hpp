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
	
	// METODOS GET
	static MotorFisicas *getInstancia();
	btDynamicsWorld *getMundo();
	irr::core::list<btRigidBody *> getObjetos();
	
	// METODOS SET
	void setObjetos(irr::core::list<btRigidBody *> objetos);

private:
	MotorFisicas(void);
	static MotorFisicas *instancia;

	btBroadphaseInterface *broadPhase;
	btDefaultCollisionConfiguration *confColision;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;
	core::list<btRigidBody *> objetos;
	btDynamicsWorld *mundo;




};

#endif /* MOTORFISICAS_H */