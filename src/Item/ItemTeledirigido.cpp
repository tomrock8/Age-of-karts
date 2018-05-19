#include "ItemTeledirigido.hpp"
#include "Pista.hpp"
#include "math.h"
#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  4
#endif

ItemTeledirigido::ItemTeledirigido(btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo) : Item(posicion, escala, masa, tiempoDesctruccion, fcolision, tamanyoNodo, radio, alturaLanzamiento, idNodo) {

	nodo = TMotor::instancia().newMeshNode("Estatico", "assets/flecha/flecha.obj", "escena_raiz", false);
	//cout << "(ItemTeledirigido::ItemTeledirigido) Hay que comprobar la posicion que sea respecto del corredor al crear\n";
	//nodo->setPosition(posicion.getX(), posicion.getY(), posicion.getZ());
	nodo->setScale(tamanyoNodo.getX(), tamanyoNodo.getY(), tamanyoNodo.getZ());

	GestorIDs::instancia().setIdentifier(nodo, "Estatico");
	id = nodo->getID();
	nombre = "ItemTeledirigido";

	inicializarFisicas();
	rigidBody->setGravity(btVector3(0, 0, 0));

	enemigo = false;
	anguloGiro = 0;
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}



ItemTeledirigido::~ItemTeledirigido(){
	actual=nullptr; 
	siguiente=nullptr; 
}
void ItemTeledirigido::lanzarItem(int direccion, btVector3 orientacion, btTransform objeto) {
	this->orientacion = orientacion;
	objeto.setOrigin(posicion);
	rigidBody->setCenterOfMassTransform(objeto);
	//rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 400, 50.f, orientacion.getZ() * 400));
}

void ItemTeledirigido::updateHijos() {

	//updateVectorDireccion();
	setWaypointActual();
	ActualizarRaytest();

	if (enemigo)
		direccionGiro(PosEnemigo);
	else
		direccionGiro(siguiente->getPosicion());

	movimiento();

	ajustarAltura();
	comprobarAltura(0.1);

}


void ItemTeledirigido::setWaypoint(Waypoint *waypoint) {
	actual = waypoint;
	siguiente = waypoint->getNextWaypoint();

}

void ItemTeledirigido::setWaypointActual() {
	Pista *mapa = Pista::getInstancia();
	actual = mapa->getArrayWaypoints()[0];

	if (colision && idwaypoint > actual->getWaypoint()->getID() || colision && idwaypoint == 7) {
		actual = mapa->getArrayWaypoints()[idwaypoint - 7];
		if (actual->getID() - 7 == mapa->getArrayWaypoints().size() - 1) {
			siguiente = mapa->getArrayWaypoints()[0];
		}
		else {
			siguiente = actual->getNextWaypoint();
		}


	}
	colision = false;

}

void ItemTeledirigido::direccionGiro(btVector3 posicion) {


	btVector3 orientacionCoche(orientacion.getX(), orientacion.getY(), orientacion.getZ());
	btVector3 direccion = btVector3(posicion.getX() - nodo->getPosition().x,
		posicion.getY() - nodo->getPosition().y,
		posicion.getZ() - nodo->getPosition().z);

	//direccion.normalize();
	anguloGiro = orientacionCoche.angle(direccion) * 180 / M_PI;

	btVector3 orientacionCocheGirada = orientacionCoche.rotate(btVector3(0, 1, 0), 2 * M_PI / 180);

	btScalar angulo2 = orientacionCocheGirada.angle(direccion) * 180 / M_PI;

	if (angulo2 > anguloGiro)
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
	float Raycast45 = 20;

	enemigo = false;

	// Raycast central1
	btVector3 inicio(nodo->getPosition().x + orientacion.getX()*distanciaCoche, nodo->getPosition().y - 2, nodo->getPosition().z + orientacion.getZ()*distanciaCoche);
	btVector3 fin(nodo->getPosition().x + orientacion.getX()*distanciaRaycast, nodo->getPosition().y - 2, nodo->getPosition().z + orientacion.getZ() *distanciaRaycast);


	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);
	// Raycast central2 derecha
	//inicio = btVector3(3*orientacion.Z+nodo->getPosition().x,nodo->getPosition().y+1,orientacion.X*-3+nodo->getPosition().z);
	//inicio = btVector3(Raycast23*orientacion.getZ() + nodo->getPosition().x + orientacion.getX(), nodo->getPosition().y - 1, orientacion.getX()*-Raycast23 + nodo->getPosition().z + orientacion.getZ() );
	fin = btVector3(Raycast23*orientacion.getZ() + nodo->getPosition().x + orientacion.getX()*distanciaRaycast, nodo->getPosition().y - 2, orientacion.getX()*-Raycast23 + nodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast2(inicio, fin);
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast2);


	// Raycast central3 izquierda
	//inicio = btVector3(-Raycast23 * orientacion.getZ() + nodo->getPosition().x + orientacion.getX(), nodo->getPosition().y - 1, orientacion.getX()*Raycast23 + nodo->getPosition().z + orientacion.getZ() );
	fin = btVector3(-Raycast23 * orientacion.getZ() + nodo->getPosition().x + orientacion.getX()*distanciaRaycast, nodo->getPosition().y - 2, orientacion.getX()*Raycast23 + nodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast3(inicio, fin);
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast3);

	//RAycast 4 izquierda
	fin = btVector3(-Raycast45 * orientacion.getZ() + nodo->getPosition().x + orientacion.getX()*distanciaRaycast, nodo->getPosition().y - 2, orientacion.getX()*Raycast45 + nodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast4(inicio, fin);
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast4);

	// Raycast central5 derecha
	fin = btVector3(Raycast45*orientacion.getZ() + nodo->getPosition().x + orientacion.getX()*distanciaRaycast, nodo->getPosition().y - 2, orientacion.getX()*-Raycast45 + nodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast5(inicio, fin);
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast5);


	if (RayCast1.hasHit())
	{

		for (int i = 0; i < RayCast1.m_hitFractions.size(); i++)
		{
			obj3D *Node = static_cast<obj3D *>(RayCast1.m_collisionObjects[i]->getUserPointer());
			if (Node) {

				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0
					|| strcmp(Node->getName(), "JugadorRed") == 0) {

					enemigo = true;
					PosEnemigo = btVector3(Node->getPosition().x, Node->getPosition().y, Node->getPosition().z);
				}
			}
		}

	}
	if (RayCast2.hasHit()) {
		for (int i = 0; i < RayCast2.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast2.m_collisionObjects[i]->getUserPointer());
			if (Node) {


				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0
					|| strcmp(Node->getName(), "JugadorRed") == 0) {

					enemigo = true;
					PosEnemigo = btVector3(Node->getPosition().x, Node->getPosition().y, Node->getPosition().z);
				}
			}
		}

	}
	if (RayCast3.hasHit()) {
		for (int i = 0; i < RayCast3.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast3.m_collisionObjects[i]->getUserPointer());
			if (Node) {

				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0
					|| strcmp(Node->getName(), "JugadorRed") == 0) {

					enemigo = true;
					PosEnemigo = btVector3(Node->getPosition().x, Node->getPosition().y, Node->getPosition().z);
				}
			}
		}
	}
	if (RayCast4.hasHit()) {
		for (int i = 0; i < RayCast4.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast4.m_collisionObjects[i]->getUserPointer());
			if (Node) {

				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0
					|| strcmp(Node->getName(), "JugadorRed") == 0) {

					enemigo = true;
					PosEnemigo = btVector3(Node->getPosition().x, Node->getPosition().y, Node->getPosition().z);
				}
			}
		}
	}
	if (RayCast5.hasHit()) {
		for (int i = 0; i < RayCast5.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast5.m_collisionObjects[i]->getUserPointer());
			if (Node) {

				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0
					|| strcmp(Node->getName(), "JugadorRed") == 0) {

					enemigo = true;
					PosEnemigo = btVector3(Node->getPosition().x, Node->getPosition().y, Node->getPosition().z);
				}
			}
		}
	}
}


void ItemTeledirigido::movimiento() {



	int giro = 3;

	btTransform trans;
	btQuaternion quaternion;
	trans.setOrigin(btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z));

	if (anguloGiro > 0) {
		orientacion = orientacion.rotate(btVector3(0, 1, 0), giro * M_PI / 180);
		quaternion.setEulerZYX(0, (nodo->getRotation().y + giro) * M_PI / 180, 0);
		//cout<< "derecha" << endl;
	}

	else {
		orientacion = orientacion.rotate(btVector3(0, 1, 0), (-giro) * M_PI / 180);
		quaternion.setEulerZYX(0, (nodo->getRotation().y - giro) * M_PI / 180, 0);

		//cout<< "izquierda" << endl;
	}
	trans.setRotation(quaternion);
	rigidBody->setCenterOfMassTransform(trans);
	rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 150, 0, orientacion.getZ() * 150));
	//cout<< anguloGiro << endl;



}


