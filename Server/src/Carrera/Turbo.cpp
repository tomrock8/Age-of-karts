#include "Turbo.hpp"
#include "GestorIDs.hpp"
#include "Timer.hpp"
#include "MotorFisicas.hpp"
#include "TMotor.hpp"

//el turbo se creara con una id y una posicion que se recojera por fichero 
Turbo::Turbo(btVector3 pos, bool estado) {
	turboActivo = estado;
	turboTocado = estado;
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	
	std::vector<btRigidBody *> objetos = bullet->getObjetos();
	turbo = TMotor::instancia().newMeshNode("Turbo"," ","escena_raiz",false);
	escala = btVector3(2.5f, 0.2f, 1.25f);
	//turbo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
	//turbo->setMaterialFlag(EMF_LIGHTING, false);
	//turbo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	GestorIDs::instancia().setIdentifier(turbo, "Turbo");
	id = turbo->getID();
	//asignar un color , a falta de ponerle una textura 
	//posicion origen 
	transform;
	transform.setIdentity();
	transform.setOrigin(pos);


	MotionState = new btDefaultMotionState(transform);

	// Create the shape
	halfExtents = btVector3(escala.getX() * 2, escala.getY() + 5, escala.getZ() * 2);
	Shape = new btBoxShape(halfExtents);
	// sin masa
	localInertia;
	Shape->calculateLocalInertia(0, localInertia);
	//Crear el rigidbody
	rigidBody = new btRigidBody(0, MotionState, Shape, localInertia);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	//ACTIVA LA COLISION SIN COLISIONAR CON EL OBJETO
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// Almacenar el rigidbody asociado con el nodo turbo
	rigidBody->setUserPointer((void *)(turbo));

	//introducir el rigidbody en el mundo
	bullet->getMundo()->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);
}

int Turbo::getID() {
	return id;
}
bool Turbo::getTurboActivo() {
	//cout<<"esta activo ? :"<<turboActivo<<endl;
	return turboActivo;
}
void Turbo::setTurboActivo(Corredor *c, bool s) {
	//cout << "TURBO ACTIVADO" << endl;
	turboActivo = s;
	turboTocado = s;
	if (turboActivo && turboTocado) {//si esta activo almacenamos tiempo y aumentamos fuerza
		c->SetFuerzaVelocidad(150000);
		c->acelerar();
		Timer *t = Timer::getInstancia();
		tiempo = t->getTimer();
	}
	else c->SetFuerzaVelocidad(100000);
	turboTocado = false;
}

int Turbo::getTiempoTurbo() {
	return tiempo;
}
void Turbo::setTurboTocado(bool s) {
	s = turboTocado;
}
//destructor
Turbo::~Turbo() {
	cout << "ENTRO DELETE TURBO: " << this->getID();

	// Los rigid body se borran desde el motor de fisicas
	// delete rigidBody;

	cout << " SALGO DELETE TURBO\n";
}