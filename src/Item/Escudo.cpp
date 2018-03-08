#include "Escudo.hpp"


Escudo::Escudo(ISceneNode* n,btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
float alturaLanzamiento,int idNodo) : Item(posicion,escala,masa,tiempoDesctruccion,fcolision,tamanyoNodo,radio,alturaLanzamiento,idNodo) {
	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	NodoVehiculo=n;
	
	objetoHabilidad =  Motor3d::instancia().getScene()->getMesh("assets/Objetos/escudo.obj");
	nodo = Motor3d::instancia().getScene()->addMeshSceneNode(objetoHabilidad, 0);
	nodo->setPosition(vector3df(posicion.getX(), posicion.getY(), posicion.getZ()));
	nodo->setScale(vector3df(tamanyoNodo.getX(), tamanyoNodo.getY(), tamanyoNodo.getZ()));
	

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

	GestorIDs::instancia().setIdentifier(nodo,"Estatico");
	id=nodo->getID();

	nombre = "Escudo";
	nodo->setName("Escudo");

	inicializarFisicas();

	
	rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	rigidBody->setGravity(btVector3(0,0,0));
	
}


void Escudo::lanzarItem(int direccion,btVector3 orientacion){
	
}

void Escudo::updateHijos(){
	movimiento();
}



void Escudo::movimiento(){


		nodo->setPosition(NodoVehiculo->getPosition());
		nodo->setRotation(NodoVehiculo->getRotation());
		btTransform trans;
		btQuaternion quaternion;
		trans.setOrigin(btVector3(NodoVehiculo->getPosition().X ,NodoVehiculo->getPosition().Y+2,NodoVehiculo->getPosition().Z ));
		quaternion.setEulerZYX(NodoVehiculo->getRotation().X * PI/180,NodoVehiculo->getRotation().Y * PI/180,NodoVehiculo->getRotation().Z * PI/180);
		trans.setRotation(quaternion);
		rigidBody->setCenterOfMassTransform(trans);
		
		
}

