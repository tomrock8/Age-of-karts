#include "Proyectil.hpp"

Proyectil::Proyectil(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,
float alturaLanzamiento) : Item(posicion,escala,masa,tiempoDesctruccion,fcolision,tamanyoNodo,radio,alturaLanzamiento) {
	

	nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyoNodo);
	nodo->setPosition(vector3df(posicion.getX(), posicion.getY(), posicion.getZ()));
	nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
	

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	GestorIDs::instancia().setIdentifier(nodo,"Estatico");
	id=nodo->getID();

	nombre = "Proyectil";
	nodo->setName("Proyectil");
	nodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/gust.png"));
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