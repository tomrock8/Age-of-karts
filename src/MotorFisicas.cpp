#include "MotorFisicas.hpp"

//------------------------------\*
//---CONSTRUCTOR Motorfisicas---\*
//------------------------------\*

MotorFisicas *MotorFisicas::instancia = NULL;

MotorFisicas::MotorFisicas()
{

	broadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000)); //limites del mundo
	confColision = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(confColision);
	solver = new btSequentialImpulseConstraintSolver();
	mundo = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, confColision); //creacion del mundo
	fileLoader = new btBulletWorldImporter(mundo);
	fileLoader->loadFile("assets/Mapa01/FisicasMapaIsla.bullet"); //no recoge bien el const char con la ruta del fichero si se le pasas por parametro
}

MotorFisicas::~MotorFisicas() {
	delete broadPhase;
	delete confColision;
	delete dispatcher;
	delete solver;
	delete fileLoader;
	delete mundo;
}

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

irr::core::list<btRigidBody *> MotorFisicas::getObjetos()
{
	return objetos;
}
void MotorFisicas::setObjetos(irr::core::list<btRigidBody *> obj)
{
	objetos = obj;
}

void MotorFisicas::setFisicas(const char *f)
{
	//cout << f << endl;
	fisicas = f;
	//fileLoader->loadFile("fisicas/carretera.bullet");
	fileLoader->loadFile(f);
}
//void MotorFisicas::setMapaFisicas(stringw mapa){
//	fileLoader->loadFile("fisicas/carretera.bullet");
//}