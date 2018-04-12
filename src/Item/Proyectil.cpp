#include "Proyectil.hpp"

Proyectil::Proyectil(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
float alturaLanzamiento,int idNodo) : Item(posicion,escala,masa,tiempoDesctruccion,fcolision,tamanyoNodo,radio,alturaLanzamiento,idNodo) {
	
	nodo = TMotor::instancia().newMeshNode("Estatico","assets/flecha/flecha.obj","escena_raiz");
	cout << "(Proyectil::Proyectil) Hay que comprobar la posicion que sea respecto del corredor al crear\n";
	nodo->setPosition(posicion.getX(), posicion.getY(), posicion.getZ());
	
	GestorIDs::instancia().setIdentifier(nodo,"Estatico");
	id=nodo->getID();

	nombre = "Proyectil";
	nodo->setName("Proyectil");

	inicializarFisicas();

	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	rigidBody->setGravity(btVector3(0,-6,0));
	
}

void Proyectil::lanzarItem(int direccion,btVector3 orientacion) {
	/*
	direccionItem: 1=Delante
	-1=Atras	
	*/
	
	if (direccion == 1)
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 400, 5.0f, orientacion.getZ() * 400));
	else if (direccion == -1)
		rigidBody->setLinearVelocity(btVector3(-orientacion.getX() * 400, 5.0f, -orientacion.getZ() * 400));

	
}


void Proyectil::updateHijos(){

}

void Proyectil::deleteHijos(){
	
}