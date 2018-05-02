#include "Habilidad.hpp"

Habilidad::Habilidad(int tipo, obj3D* n, btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo) : Item(posicion, escala, masa, tiempoDesctruccion, fcolision, tamanyoNodo, radio, alturaLanzamiento, idNodo) {
	//almacenamos tipo de habilidad
	tipoHabilidad = tipo;
	NodoVehiculo = n;
	cont = 0;
	cont2 = 10;
	const char* strEscudo = "assets/escudo/escudo.obj";
	switch (tipoHabilidad) {

	case 1:
		nodo = TMotor::instancia().newMeshNode("Estatico", strEscudo, "escena_raiz",false);
		nombre = "HabilidadPirata";
		break;

	case 2:
		nodo = TMotor::instancia().newMeshNode("Estatico", strEscudo, "escena_raiz",false);
		nombre = "HabilidadVikingo";
		break;

	case 3:
		nodo = TMotor::instancia().newMeshNode("Estatico", strEscudo, "escena_raiz",false);
		nombre = "HabilidadGladiador";
		break;

	case 4:
		nodo = TMotor::instancia().newMeshNode("Estatico", strEscudo, "escena_raiz",false);
		nombre = "HabilidadChino";
		break;

	}
	
	nodo->setScale(tamanyoNodo.getX(), tamanyoNodo.getY(), tamanyoNodo.getZ());
	GestorIDs::instancia().setIdentifier(nodo, "Estatico");
	id = nodo->getID();
	nodo->setName(nombre);

	inicializarFisicas();

	if (tipoHabilidad == 4 || tipoHabilidad == 2 || tipoHabilidad == 3) {
		rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		rigidBody->setGravity(btVector3(0, 0, 0));
		rigidBody->setActivationState(DISABLE_DEACTIVATION);
	}

}

void Habilidad::lanzarItem(int direccion, btVector3 orientacion,btTransform objeto) {
	this->orientacion = orientacion;

	if (tipoHabilidad == 1) { // PIRATA

		rigidBody->setGravity(btVector3(0, -300, 0));
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 300, 1.0f, orientacion.getZ() * 300));

	}

}

void Habilidad::updateHijos() {
	movimiento();
}


void Habilidad::movimiento() {

	btTransform trans;
	btQuaternion quaternion;
	int giro = 0;


	switch (tipoHabilidad) {


	case 2: // VIKINGO

		giro = 15;
		trans.setOrigin(btVector3(NodoVehiculo->getPosition().x + orientacion.getX() * 15, NodoVehiculo->getPosition().y, NodoVehiculo->getPosition().z + orientacion.getZ() * 15));

		orientacion = orientacion.rotate(btVector3(0, 1, 0), giro * PI / 180);
		nodo->setRotation(nodo->getRotation().x, cont, nodo->getRotation().x);
		quaternion.setEulerZYX(0, nodo->getRotation().y * PI / 180, 0);

		trans.setRotation(quaternion);

		rigidBody->setCenterOfMassTransform(trans);
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 100000, 0, orientacion.getZ() * 100000));

		cont += 90;

		if (cont == 360)
			cont = 0;


		break;

	case 3: // GLADIADOR

	//nodo->setPosition(NodoVehiculo->getPosition().x,NodoVehiculo->getPosition().y+2,NodoVehiculo->getPosition().z);

	//if(nodo->getScale().X<=10)
	//nodo->setScale(nodo->getScale().X +1,nodo->getScale().Y,nodo->getScale().Z +1);

		trans.setOrigin(btVector3(NodoVehiculo->getPosition().x, NodoVehiculo->getPosition().y - cont2 + 2, NodoVehiculo->getPosition().z));
		quaternion.setEulerZYX(NodoVehiculo->getRotation().x * PI / 180, NodoVehiculo->getRotation().y * PI / 180, NodoVehiculo->getRotation().z * PI / 180);
		trans.setRotation(quaternion);
		rigidBody->setCenterOfMassTransform(trans);

		if (cont2 != 0)
			cont2--;

		break;

	case 4: //CHINICO
		trans.setOrigin(btVector3(NodoVehiculo->getPosition().x, NodoVehiculo->getPosition().y + 2, NodoVehiculo->getPosition().z));
		quaternion.setEulerZYX(NodoVehiculo->getRotation().x * PI / 180, NodoVehiculo->getRotation().y * PI / 180, NodoVehiculo->getRotation().z * PI / 180);
		trans.setRotation(quaternion);
		rigidBody->setCenterOfMassTransform(trans);

		break;
	}
}


void Habilidad::deleteHijos() {}