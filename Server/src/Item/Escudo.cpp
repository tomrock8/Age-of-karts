#include "Escudo.hpp"


Escudo::Escudo(btRigidBody* n, btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo) : Item(posicion, escala, masa, tiempoDesctruccion, fcolision, tamanyoNodo, radio, alturaLanzamiento, idNodo) {
	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	NodoVehiculo = n;

	//objetoHabilidad =  Motor3d::instancia().getScene()->getMesh("assets/Objetos/escudo.obj");
	nodo = TMotor::instancia().newMeshNode("Estatico", "assets/escudo/escudo.obj", "escena_raiz",false);
	
	
	nodo->setScale(escala.getX(),escala.getY(),escala.getZ());
	
	GestorIDs::instancia().setIdentifier(nodo, "Estatico");
	id = nodo->getID();

	nombre = "Escudo";
	nodo->setName("Escudo");

	inicializarFisicas();


	rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	rigidBody->setGravity(btVector3(0, 0, 0));

}

void Escudo::lanzarItem(int direccion, btVector3 orientacion,btTransform objeto) {}
void Escudo::updateHijos() {
	movimiento();
}

void Escudo::movimiento() {
	float altura = 2;
	nodo->setPosition(NodoVehiculo->getCenterOfMassPosition().getX(), NodoVehiculo->getCenterOfMassPosition().getY()+altura, NodoVehiculo->getCenterOfMassPosition().getZ());
	btTransform trans;
	trans.setOrigin(btVector3(NodoVehiculo->getCenterOfMassPosition().getX(), NodoVehiculo->getCenterOfMassPosition().getY()+altura , NodoVehiculo->getCenterOfMassPosition().getZ()));
	trans.setRotation(NodoVehiculo->getCenterOfMassTransform().getRotation());
	rigidBody->setCenterOfMassTransform(trans);
}


void Escudo::deleteHijos() {
	//delete objetoHabilidad;
}
