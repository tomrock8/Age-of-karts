#include "Habilidad.hpp"

Habilidad::Habilidad(int tipo, ISceneNode* n,btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,
  float alturaLanzamiento)
: Item(posicion,escala,masa,tiempoDesctruccion,fcolision,tamanyoNodo,radio,alturaLanzamiento) {
	//almacenamos tipo de habilidad
	tipoHabilidad = tipo;
	NodoVehiculo = n;
	
	switch(tipoHabilidad){

		case 1:
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/onda.obj");
		nombre = "HabilidadPirata";
		break;

		case 2:
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/habilidadVikingo.obj");
		nombre = "HabilidadVikingo";
		break;

		case 3:
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/habilidadGladiador.obj");
		nombre = "HabilidadGladiador";
		break;

		case 4:
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/habilidadChino.obj");
		nombre = "HabilidadChino";
		break;

	}
		nodo = Motor3d::instancia().getScene()->addMeshSceneNode(objetoHabilidad, 0);
		nodo->setPosition(vector3df(posicion.getX(), posicion.getY(), posicion.getZ()));
		nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
		//nodo->setVisible(false);
		nodo->setMaterialFlag(EMF_LIGHTING, false);
		nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
		//nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

		GestorIDs::instancia().setIdentifier(nodo,"Estatico");
		id=nodo->getID();
		nodo->setName(nombre);

		inicializarFisicas();
}






void Habilidad::lanzarItem(int direccion,btVector3 orientacion){

	this->orientacion = orientacion;

	
	if(tipoHabilidad==2){
		
	}

	if(tipoHabilidad == 1){ // PIRATA

		rigidBody->setGravity(btVector3(0,-300,0));
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 300, 1.0f, orientacion.getZ() * 300));

	}

}

void Habilidad::updateHijos(){

	movimiento();

}


void Habilidad::movimiento(){

	btTransform trans;
	btQuaternion quaternion;
	int giro =0;
	
		
	switch(tipoHabilidad){


		case 2: // VIKINGO

		giro=8;
		trans.setOrigin(btVector3(NodoVehiculo->getPosition().X + orientacion.getX() *15,NodoVehiculo->getPosition().Y,NodoVehiculo->getPosition().Z + orientacion.getZ() *15));

		orientacion = orientacion.rotate(btVector3(0, 1, 0),giro*PI/180);
		nodo->setRotation(vector3df(nodo->getRotation().X,nodo->getRotation().Y,nodo->getRotation().Z));
		quaternion.setEulerZYX(0,nodo->getRotation().Y * PI/180,0);

		trans.setRotation(quaternion);

	
		rigidBody->setCenterOfMassTransform(trans);

		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 30000, 0, orientacion.getZ() * 30000));
		break;

		case 3: // GLADIADOR

		
		
		break;

		case 4: //CHINICO
		trans.setOrigin(btVector3(NodoVehiculo->getPosition().X ,NodoVehiculo->getPosition().Y+2,NodoVehiculo->getPosition().Z ));
		quaternion.setEulerZYX(NodoVehiculo->getRotation().X * PI/180,NodoVehiculo->getRotation().Y * PI/180,NodoVehiculo->getRotation().Z * PI/180);
		trans.setRotation(quaternion);
		rigidBody->setCenterOfMassTransform(trans);
		rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		rigidBody->setGravity(btVector3(0,0,0));
		break;

	}



}