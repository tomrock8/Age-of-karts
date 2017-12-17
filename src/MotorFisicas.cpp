#include "MotorFisicas.hpp"

//------------------------------\*
//---CONSTRUCTOR Motorfisicas---\*
//------------------------------\*

MotorFisicas *MotorFisicas::instancia = NULL;

MotorFisicas::MotorFisicas()
{

	btBroadphaseInterface *broadPhase =  new btAxisSweep3(btVector3(-1000,1000,-1000), btVector3(1000,1000,1000));//limites del mundo
	btDefaultCollisionConfiguration *confColision = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(confColision);
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
	mundo = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, confColision);//creacion del mundo
		btBulletWorldImporter* fileLoader = new btBulletWorldImporter(mundo);
}

MotorFisicas::~MotorFisicas() {}

MotorFisicas *MotorFisicas::getInstancia()
{
	if (instancia == NULL)
		instancia = new MotorFisicas();

	return instancia;
}



btDynamicsWorld *MotorFisicas::getMundo()
{

	return mundo;
}



//void MotorFisicas::setMapaFisicas(stringw mapa){
//	fileLoader->loadFile("fisicas/carretera.bullet");
//}