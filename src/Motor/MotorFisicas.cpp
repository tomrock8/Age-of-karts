#include "MotorFisicas.hpp"

//------------------------------\*
//---CONSTRUCTOR Motorfisicas---\*
//------------------------------\*

MotorFisicas *MotorFisicas::instancia = NULL;

MotorFisicas::MotorFisicas() {
	broadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000)); //limites del mundo
	confColision = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(confColision);
	solver = new btSequentialImpulseConstraintSolver();
	mundo = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, confColision); //creacion del mundo
//	fisicasMapa = new btBulletWorldImporter(mundo);
//	fisicasMapa->loadFile("assets/Mapa01/FisicasMapaIsla.bullet"); //no recoge bien el const char con la ruta del fichero si se le pasas por parametro
}

MotorFisicas::~MotorFisicas() {
	cout << "ENTRO DESTRUCTOR MOTOR DE FISICAS\n";

	cout << "Destruyendo Objetos del mundo. Son " << mundo->getNumCollisionObjects() << "\n";
	for (int i = mundo->getNumCollisionObjects() - 1; i >= 0; i--) {
		cout << "EL " << i;
		btCollisionObject* obj = mundo->getCollisionObjectArray()[i];
		btCollisionShape* shape = obj->getCollisionShape();
		mundo->removeCollisionObject(obj);
		cout << " borro collision obj, ";
		delete obj;
		cout << " borro collision shape";
		delete shape;
		cout << "\n";
	}
	cout << "Objetos del mundo destruidos.\n";

	cout << "Destruyendo mundo";
	delete mundo;
	cout << "\n";
	
	cout << "Destruyendo solver";
	delete solver;
	cout << "\n";

	cout << "Destruyendo dispatcher";
	delete dispatcher;
	cout << "\n";
	
	cout << "Destruyendo confColision";
	delete confColision;
	cout << "\n";
	
	cout << "Destruyendo broadPhase";
	delete broadPhase;
	cout << "\n";
	
	instancia = nullptr;
	cout << "SALGO DESTRUCTOR MOTOR DE FISICAS\n";
}

MotorFisicas *MotorFisicas::getInstancia() {
	if (instancia == NULL)
		instancia = new MotorFisicas();

	return instancia;
}

btDynamicsWorld *MotorFisicas::getMundo() {
	return mundo;
}

std::vector<btRigidBody *> MotorFisicas::getObjetos() {
	return objetos;
}

void MotorFisicas::setObjetos(std::vector<btRigidBody *> obj) {
	objetos = obj;
}

//void MotorFisicas::setMapaFisicas(stringw mapa){
//	fileLoader->loadFile("fisicas/carretera.bullet");
//}
//btBulletWorldImporter *MotorFisicas::getFisicas() {
//	return fisicasMapa;
//}
