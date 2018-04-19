#ifndef MOTORFISICAS_H
#define MOTORFISICAS_H

#include <iostream>
#include <string>
#include <vector>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"
#include "obj3D.hpp"

using namespace std;

class MotorFisicas
{
public:
	//Destructor
	~MotorFisicas(void);

	// METODOS GET
	static MotorFisicas *getInstancia();
	btDynamicsWorld *getMundo();
	std::vector<btRigidBody *> getObjetos();
	void initializePhysics(const char *name, btVector3 pos, obj3D *obj);
	btCollisionShape  *CreateCollisionShape(const char *name);
	// METODOS SET
	void setObjetos(std::vector<btRigidBody *> objetos);

private:
	MotorFisicas(void);
	static MotorFisicas *instancia;

	btBroadphaseInterface *broadPhase;
	btDefaultCollisionConfiguration *confColision;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;
	std::vector<btRigidBody *> objetos;
	btDynamicsWorld *mundo;
};

#endif /* MOTORFISICAS_H */