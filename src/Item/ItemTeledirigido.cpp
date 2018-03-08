#include "ItemTeledirigido.hpp"


ItemTeledirigido::ItemTeledirigido(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
float alturaLanzamiento,int idNodo) : Item(posicion,escala,masa,tiempoDesctruccion,fcolision,tamanyoNodo,radio,alturaLanzamiento,idNodo) {

	nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyoNodo.getX());
	nodo->setPosition(vector3df(posicion.getX(), posicion.getY(), posicion.getZ()));
	//nodo->setScale(vector3df(tamanyoNodo.getX(), tamanyoNodo.getY(), tamanyoNodo.getZ()));

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);

	GestorIDs::instancia().setIdentifier(nodo,"Estatico");
	id=nodo->getID();
	nombre="ItemTeledirigido";


	nodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/gust.png"));
	inicializarFisicas();
	rigidBody->setGravity(btVector3(0,0,0));


	enemigo=false;
	anguloGiro=0;

}




void ItemTeledirigido::lanzarItem(int direccion,btVector3 orientacion){


	this->orientacion=orientacion;
	
}

void ItemTeledirigido::updateHijos(){
	
	//updateVectorDireccion();
	setWaypointActual();
	ActualizarRaytest();
	
	if(enemigo)
	direccionGiro(PosEnemigo);
	else
	direccionGiro(siguiente->getPosicion());

	movimiento();

	//cout<< actual->getWaypoint()->getID() << endl;

}	


void ItemTeledirigido::setWaypoint(Waypoint *waypoint){

	actual = waypoint;
	siguiente = waypoint->getNextWaypoint();

}

void ItemTeledirigido::setWaypointActual(){


	if(colision && idwaypoint > actual->getWaypoint()->getID() ||  colision && idwaypoint == 6){
	actual = siguiente;
	siguiente = actual->getNextWaypoint();
	
	}
	colision=false;

}

void ItemTeledirigido::direccionGiro(btVector3 posicion){

	
	btVector3 orientacionCoche(orientacion.getX(),orientacion.getY(),orientacion.getZ());
	btVector3 direccion = btVector3(posicion.getX()-nodo->getPosition().X,
	posicion.getY()-nodo->getPosition().Y,
	posicion.getZ()-nodo->getPosition().Z);
	
	//direccion.normalize();
	anguloGiro = orientacionCoche.angle(direccion) *180/PI;
	
	btVector3 orientacionCocheGirada = orientacionCoche.rotate(btVector3(0, 1, 0),2*PI/180);

	btScalar angulo2 = orientacionCocheGirada.angle(direccion) *180/PI;
		
		if(angulo2>anguloGiro)
			anguloGiro = -anguloGiro;

}




void ItemTeledirigido::ActualizarRaytest() {

	MotorFisicas *mun = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = mun->getMundo();
	mundo->updateAabbs();
	mundo->computeOverlappingPairs();

	//cout<< orientacion.X << "   "<< orientacion.Z<<endl;
	float distanciaRaycast = 150; // longitud del rayo
	float distanciaCoche = 2; // distancia entre el rayo y el coche, donde empieza
	float Raycast23 = 10; // distancia entre raycast 2 y 3
	

	enemigo=false;

	// Raycast central1
	btVector3 inicio(nodo->getPosition().X + orientacion.getX()*distanciaCoche, nodo->getPosition().Y -1, nodo->getPosition().Z+ orientacion.getZ()*distanciaCoche);
	btVector3 fin(nodo->getPosition().X + orientacion.getX()*distanciaRaycast, nodo->getPosition().Y -1, nodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);


	//mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);
	// Raycast central2 derecha
	//inicio = btVector3(3*orientacion.Z+nodo->getPosition().X,nodo->getPosition().Y+1,orientacion.X*-3+nodo->getPosition().Z);
	//inicio = btVector3(Raycast23*orientacion.getZ() + nodo->getPosition().X + orientacion.getX(), nodo->getPosition().Y - 1, orientacion.getX()*-Raycast23 + nodo->getPosition().Z + orientacion.getZ() );
	fin = btVector3(Raycast23*orientacion.getZ() + nodo->getPosition().X + orientacion.getX()*distanciaRaycast, nodo->getPosition().Y - 1, orientacion.getX()*-Raycast23 + nodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	//mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast2(inicio, fin);
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast2);


	// Raycast central3 izquierda
	//inicio = btVector3(-Raycast23 * orientacion.getZ() + nodo->getPosition().X + orientacion.getX(), nodo->getPosition().Y - 1, orientacion.getX()*Raycast23 + nodo->getPosition().Z + orientacion.getZ() );
	fin = btVector3(-Raycast23 * orientacion.getZ() + nodo->getPosition().X + orientacion.getX()*distanciaRaycast, nodo->getPosition().Y - 1, orientacion.getX()*Raycast23 + nodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	//mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast3(inicio, fin);
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast3);

	if (RayCast1.hasHit())
	{

		for (int i=0;i<RayCast1.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast1.m_collisionObjects[i]->getUserPointer());
		if (Node) {

			if(strcmp(Node->getName(),"Jugador")==0
			|| strcmp(Node->getName(),"JugadorIA")==0
			|| strcmp(Node->getName(),"JugadorRed")==0){

			enemigo=true;
			PosEnemigo= btVector3(Node->getPosition().X,Node->getPosition().Y,Node->getPosition().Z);
			}
		}
			}

	}
	if (RayCast2.hasHit()) {

		for (int i=0;i<RayCast2.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast2.m_collisionObjects[i]->getUserPointer());
		if (Node) {
		

			if(strcmp(Node->getName(),"Jugador")==0
			|| strcmp(Node->getName(),"JugadorIA")==0
			|| strcmp(Node->getName(),"JugadorRed")==0){

			enemigo=true;
			PosEnemigo= btVector3(Node->getPosition().X,Node->getPosition().Y,Node->getPosition().Z);
			}
		}
			}

	}
	if (RayCast3.hasHit()) {

		for (int i=0;i<RayCast3.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast3.m_collisionObjects[i]->getUserPointer());
		if (Node) {
	
			if(strcmp(Node->getName(),"Jugador")==0
			|| strcmp(Node->getName(),"JugadorIA")==0
			|| strcmp(Node->getName(),"JugadorRed")==0){

			enemigo=true;
			PosEnemigo= btVector3(Node->getPosition().X,Node->getPosition().Y,Node->getPosition().Z);
			}
		}
			}
	}

}


void ItemTeledirigido::movimiento(){


	
	int giro =5;
	
	btTransform trans;
	btQuaternion quaternion;
	trans.setOrigin(btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z));

	if(anguloGiro>0){
	orientacion = orientacion.rotate(btVector3(0, 1, 0),giro*PI/180);
	nodo->setRotation(vector3df(nodo->getRotation().X,nodo->getRotation().Y+5,nodo->getRotation().Z));
	quaternion.setEulerZYX(0,nodo->getRotation().Y * PI/180,0);
	//cout<< "derecha" << endl;
	}

	else{
	orientacion= orientacion.rotate(btVector3(0, 1, 0),(-giro)*PI/180);
	nodo->setRotation(vector3df(nodo->getRotation().X,nodo->getRotation().Y-5,nodo->getRotation().Z));
	quaternion.setEulerZYX(0,nodo->getRotation().Y * PI/180,0);
	
	//cout<< "izquierda" << endl;
	}	
	trans.setRotation(quaternion);
	rigidBody->setCenterOfMassTransform(trans);

	rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 150, 0, orientacion.getZ() * 150));
	//cout<< anguloGiro << endl;
	
	

}