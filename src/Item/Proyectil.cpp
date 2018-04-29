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
	subir=false;
	bajar=false;
	subealto=false;
	bajaalto=false;
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
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 400, 0, orientacion.getZ() * 400));
	else if (direccion == -1)
		rigidBody->setLinearVelocity(btVector3(-orientacion.getX() * 400, 0, -orientacion.getZ() * 400));

	
}


void Proyectil::updateHijos(){

comprobarAltura();
movimiento();

}

void Proyectil::deleteHijos(){
	
}

void Proyectil::movimiento(){

	btTransform posObj= rigidBody->getCenterOfMassTransform();
	float altura= 0.5;
	float subealto=2;
	float bajaalto=2;

	if(subir){
		posObj.setOrigin(btVector3(posObj.getOrigin().getX(),posObj.getOrigin().getY()+altura,posObj.getOrigin().getZ()));
	}else if(bajar){
		posObj.setOrigin(btVector3(posObj.getOrigin().getX(),posObj.getOrigin().getY()-altura,posObj.getOrigin().getZ()));
	}else if(subealto){
		posObj.setOrigin(btVector3(posObj.getOrigin().getX(),posObj.getOrigin().getY()+subealto,posObj.getOrigin().getZ()));
	}else if(bajaalto){
		posObj.setOrigin(btVector3(posObj.getOrigin().getX(),posObj.getOrigin().getY()-bajaalto,posObj.getOrigin().getZ()));
	}

	rigidBody->setCenterOfMassTransform(posObj);
}

void Proyectil::comprobarAltura(){

	MotorFisicas *mun = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = mun->getMundo();
	mundo->updateAabbs();
	mundo->computeOverlappingPairs();
	
	// Raycast central1
	btVector3 inicio(nodo->getPosition().x , nodo->getPosition().y , nodo->getPosition().z);
	btVector3 fin(nodo->getPosition().x , nodo->getPosition().y - 40, nodo->getPosition().z);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);
	subir=false;
	bajar=false;
	subealto=false;
	bajaalto=false;

	if (RayCast1.hasHit())
	{

		for (int i = 0; i < RayCast1.m_hitFractions.size(); i++)
		{
					
					if(RayCast1.m_hitFractions[i]< 0.11){
					subir=true;
					
						if(RayCast1.m_hitFractions[i]< 0.08){
							
							subir=false;
							subealto=true;
						}
					
					}else if(RayCast1.m_hitFractions[i]> 0.11){

					bajar=true;
					
						if(RayCast1.m_hitFractions[i]> 0.2){
							
							bajar=false;
							bajaalto=true;
						
						}
					}	
			}
		}
}