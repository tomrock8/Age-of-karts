#include "Turbo.hpp"
#include "GestorIDs.hpp"
#include "Timer.hpp"
#include "MotorFisicas.hpp"
#include "TMotor.hpp"
#include "btBulletDynamicsCommon.h"
#include "obj3D.hpp"
#include "Corredor.hpp"

//el turbo se creara con una id y una posicion que se recojera por fichero 
Turbo::Turbo(btVector3 pos, bool estado) {
	turboActivo = estado;
	turboTocado = estado;
	MotorFisicas* bullet = MotorFisicas::getInstancia();

	std::vector<btRigidBody*> objetos = bullet->getObjetos();
	turbo = TMotor::instancia().newMeshNode("Turbo", " ", "escena_raiz", false);
	btVector3 escala = btVector3(2.5f, 0.2f, 1.25f);

	GestorIDs::instancia().setIdentifier(turbo, "Turbo");
	id = turbo->getID();

	//posicion origen 
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(pos);

	// Create the shape
	btCollisionShape* Shape = new btBoxShape(btVector3(escala.getX() * 2, escala.getY() + 5, escala.getZ() * 2));

	// sin masa
	btVector3 localInertia;
	Shape->calculateLocalInertia(0, localInertia);
	//Crear el rigidbody
	rigidBody = new btRigidBody(0, new btDefaultMotionState(transform), Shape, localInertia);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	//ACTIVA LA COLISION SIN COLISIONAR CON EL OBJETO
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// Almacenar el rigidbody asociado con el nodo turbo
	rigidBody->setUserPointer((void*)(turbo));

	//introducir el rigidbody en el mundo
	bullet->getMundo()->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);

	Shape = nullptr;
}

//destructor
Turbo::~Turbo() {
	delete turbo; // Modelo 3D

	delete rigidBody->getCollisionShape();
	delete rigidBody->getMotionState();

	MotorFisicas::getInstancia()->getMundo()->removeCollisionObject(rigidBody);
	delete rigidBody;
}

void Turbo::setTurboActivo(Corredor* c, bool s) {
	//cout << "TURBO ACTIVADO" << endl;
	turboActivo = s;
	turboTocado = s;
	if (turboActivo && turboTocado) {//si esta activo almacenamos tiempo y aumentamos fuerza
		c->SetFuerzaVelocidad(150000);
		c->acelerar();
		Timer* t = Timer::getInstancia();
		tiempo = t->getTimer();
	}
	else c->SetFuerzaVelocidad(100000);
	turboTocado = false;
}

int Turbo::getID() {
	return id;
}

bool Turbo::getTurboActivo() {
	return turboActivo;
}

int Turbo::getTiempoTurbo() {
	return tiempo;
}

void Turbo::setTurboTocado(bool s) {
	s = turboTocado;
}
