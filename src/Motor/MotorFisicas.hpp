#ifndef MOTORFISICAS_H
#define MOTORFISICAS_H

#include <iostream>
#include <string>
#include <vector>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"
#include "obj3D.hpp"
class TRecursoMalla;

class MotorFisicas
{
public:
	//Destructor
	~MotorFisicas(void);

	// METODOS GET
	static MotorFisicas *getInstancia();
	btDynamicsWorld *getMundo();
	std::vector<btRigidBody *> getObjetos();
	void initializePhysics(TRecursoMalla *m);
	void saveFilePhysics(const char * name);
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