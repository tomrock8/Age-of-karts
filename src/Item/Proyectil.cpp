#include "Proyectil.hpp"

Proyectil::Proyectil(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
float alturaLanzamiento,int idNodo) : Item(posicion,escala,masa,tiempoDesctruccion,fcolision,tamanyoNodo,radio,alturaLanzamiento,idNodo) {
	
	nodo = TMotor::instancia().newMeshNode("Estatico","assets/flecha/flecha.obj","escena_raiz",false);
	//nodo->setPosition(posicion.getX(), posicion.getY(), posicion.getZ());
	nodo->setScale(escala.getX(),escala.getY(),escala.getZ());

	GestorIDs::instancia().setIdentifier(nodo,"Estatico");
	id=nodo->getID();

	nombre = "Proyectil";
	nodo->setName("Proyectil");

	inicializarFisicas();

	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	rigidBody->setGravity(btVector3(0,-6,0));
	
}

void Proyectil::lanzarItem(int direccion,btVector3 orientacion,btTransform objeto) {
	/*
	direccionItem: 1=Delante
	-1=Atras	
	*/
	objeto.setOrigin(posicion);
	rigidBody->setCenterOfMassTransform(objeto);
	//cout<<anguloGiro<<endl;
	if (direccion == 1)
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 400, 5.0f, orientacion.getZ() * 400));
	else if (direccion == -1)
		rigidBody->setLinearVelocity(btVector3(-orientacion.getX() * 400, 5.0f, -orientacion.getZ() * 400));

	
}


void Proyectil::updateHijos(){

movimiento();

}

void Proyectil::deleteHijos(){
	
}

void Proyectil::movimiento(){

	MotorFisicas *mun = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = mun->getMundo();
	mundo->updateAabbs();
	mundo->computeOverlappingPairs();

	// Raycast central1
	btVector3 inicio(nodo->getPosition().x , nodo->getPosition().y , nodo->getPosition().z);
	btVector3 fin(nodo->getPosition().x , nodo->getPosition().y - 100, nodo->getPosition().z);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);

	if (RayCast1.hasHit())
	{

		for (int i = 0; i < RayCast1.m_hitFractions.size(); i++)
		{
			obj3D *Node = static_cast<obj3D *>(RayCast1.m_collisionObjects[i]->getUserPointer());
			if (Node) {

				if (strcmp(Node->getName(), "mapa") == 0) {

					cout<<"COLISION CON EL MAPA"<<endl;
				}
			}
		}

	}

}