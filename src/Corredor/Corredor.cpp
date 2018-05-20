#include "Corredor.hpp"
#include "Pista.hpp"
#include "Timer.hpp"
#include "GestorJugadores.hpp"
#include "Client.hpp"
#include "Item.hpp"
#include "CajaFalsa.hpp"
#include "Aceite.hpp"
#include "Proyectil.hpp"
#include "ItemTeledirigido.hpp"
#include "Escudo.hpp"
#include "Habilidad.hpp"
#include "math.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  
#endif
//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*

Corredor::Corredor(btVector3 pos, tipo_jugador tipo) {

	cargador = 0;
	tipoObj = EstadosJugador::estado_objeto::NADA;
	turboActivado = false;
	objetivoFijado = false;
	aceiteActivado = false;
	timerTurbo = 0;
	timerAceite = 0;
	limite = 0;
	direccionContraria = 0;
	estado = new EstadosJugador();
	vueltas = 1;
	maxvueltas = 4;
	tipojugador = tipo;
	tiempoVuelta = 0.f;
	tiempoVueltaTotal = 0.f;
	tiempoHabilidad = 0;
	inmunidad = false;
	tiempoInmunidad = 2;
	timerInmunidad = 0;
	habilidadLanzada = false;
	tipojugador = tipo;
	tiempoTurbo = 0;
	proteccion = false;
	estado->setEstadoCarrera(EstadosJugador::estado_carrera::PARRILLA);




	//-------------bullet----------------


	//establecemos el primer waypoint del mapa

	Pista *mapa = Pista::getInstancia();
	actual = mapa->getArrayWaypoints()[0];
	siguiente = actual->getNextWaypoint();
	siguiente_aux = siguiente;

	direccionRuedas = btVector3(0, -1, 0);
	rotacionRuedas = btVector3(-1, 0, 0);
	suspension = btScalar(1.5f); // cuanto mas valor el chasis mas alto respecto a las ruedas
	anchoRueda = btScalar(0.4);			  //0.4
	radioRueda = btScalar(0.5);			  //No menor de 0.4 sino ni se mueve (ruedas pequenyas)
	alturaConexionChasis = btScalar(1.2); //influye mucho en la acceleracion de salida
	FuerzaFrenado = btScalar(-5000);
	FuerzaFrenoMano = btScalar(700);
	FuerzaFrenadoReposo = btScalar(60);

	//VALORES POR DEFECTO
	FuerzaGiro = btScalar(0.1); //manejo a la hora de girar
	Masa = btScalar(800);
	FuerzaMaxima = btScalar(400); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
	Fuerza = FuerzaMaxima;
	indiceGiroAlto = btScalar(0.4);
	indiceGiroBajo = btScalar(0.1);
	velocidadMaxima = 380;
	velocidadMaximaTurbo = 420;
	velocidadLimiteGiro = 150;
	velocidadMaximaAtras = -100;

	setParametros();

	// HAY QUE MODIFICAR DE QUE NODO PENDEN!!!

	rueda1 = TMotor::instancia().newMeshNode("rueda1", "assets/rueda/ruedaDer.obj", "escena_raiz", false);
	rueda2 = TMotor::instancia().newMeshNode("rueda2", "assets/rueda/ruedaIzq.obj", "escena_raiz", false);
	rueda3 = TMotor::instancia().newMeshNode("rueda3", "assets/rueda/ruedaDer.obj", "escena_raiz", false);
	rueda4 = TMotor::instancia().newMeshNode("rueda4", "assets/rueda/ruedaIzq.obj", "escena_raiz", false);


	rueda1->setScale(1, 1, 1);//alante derecha
	rueda2->setScale(1, 1, 1);//delante izquierda
	rueda3->setScale(1, 1, 1);//atras derecha
	rueda4->setScale(1, 1, 1);//atras izquierda

	rueda2->setRotation(glm::vec3(1, 0, 0), 180);
	rueda4->setRotation(glm::vec3(1, 0, 0), 180);

	if (cuboNodo) {
		cuboNodo->setPosition(pos.getX(), pos.getY(), pos.getZ());
		cuboNodo->setScale(1, 1, 1.5);
		cuboNodo->setRotation(0.0f, 90.0f, 0.0f);

	}
	//Creamos el billboard de cada corredor
	b = TMotor::instancia().newBillboard(cuboNodo);
	b->setImage("assets/HUD/MainMenu/sw_logo.png");
	//Creamos las particulas del humo
	p = TMotor::instancia().newParticleSystem();

	InicializarFisicas();
	InicializarFisicasRuedas();

	//OPENAL
	pitchMotor = 0.5f;
	fuenteMotor = new AlSource();
	fuenteMotor->setLoop(true);
	fuenteMotor->setPitch(pitchMotor);
	fuenteMotor->volume(1 * TMotor::instancia().getGestorSonido()->getVolEfectos());
	fuenteItem = new AlSource();
	fuenteItem->volume(0.5 * TMotor::instancia().getGestorSonido()->getVolEfectos());
}

void Corredor::setParametrosDebug(float fuerza, float velocidadMedia, float velocidadMaximaTurbo, float velocidadMaxima, float masa, float indiceGiroAlto, float indiceGiroBajo, float velocidadLimiteGiro) {
	this->Fuerza = fuerza;
	this->velocidadMedia = velocidadMedia;
	this->velocidadMaximaTurbo = velocidadMaximaTurbo;
	this->velocidadMaxima = velocidadMaxima;
	if (masa != this->Masa) {
		this->Masa = masa;
		btVector3 inertia(0, 0, 0);
		CuerpoColisionChasis->getCollisionShape()->calculateLocalInertia(masa, inertia);
		CuerpoColisionChasis->setMassProps(masa, inertia);
	}
	this->indiceGiroAlto = btScalar(indiceGiroAlto);
	this->indiceGiroBajo = btScalar(indiceGiroBajo);
	this->velocidadLimiteGiro = velocidadLimiteGiro;
}

void Corredor::getParametrosDebug(float *fuerza, float *velocidadMedia, float *velocidadMaximaTurbo, float *velocidadMaxima, float *masa, float *indiceGiroAlto, float *indiceGiroBajo, float *velocidadLimiteGiro) {
	*fuerza = static_cast<float>(this->Fuerza);
	*velocidadMedia = static_cast<float>(this->velocidadMedia);
	*velocidadMaximaTurbo = static_cast<float>(this->velocidadMaximaTurbo);
	*velocidadMaxima = static_cast<float>(this->velocidadMaxima);
	*masa = static_cast<float>(this->Masa);
	*indiceGiroAlto = static_cast<float>(this->indiceGiroAlto);
	*indiceGiroBajo = static_cast<float>(this->indiceGiroBajo);
	*velocidadLimiteGiro = static_cast<float>(this->velocidadLimiteGiro);
}

void Corredor::getParametrosRuedasDebug(float *suspensionStiffness, float *DampingCompression, float *DampingRelaxation, float *frictionSlip, float *rollInfluence, float *suspForce, float *suspTravelCm) {
	btWheelInfo &wheel = this->vehiculo->getWheelInfo(0);

	*suspensionStiffness = static_cast<float>(wheel.m_suspensionStiffness);
	*DampingCompression = static_cast<float>(wheel.m_wheelsDampingCompression);
	*DampingRelaxation = static_cast<float>(wheel.m_wheelsDampingRelaxation);
	*frictionSlip = static_cast<float>(wheel.m_frictionSlip);
	*rollInfluence = static_cast<float>(wheel.m_rollInfluence);
	*suspForce = static_cast<float>(wheel.m_maxSuspensionForce);
	*suspTravelCm = static_cast<float>(wheel.m_maxSuspensionTravelCm);
}

void Corredor::setParametrosRuedasDebug(float suspensionStiffness, float DampingCompression, float DampingRelaxation, float frictionSlip, float rollInfluence, float suspForce, float suspTravelCm) {
	btWheelInfo &wheel = this->vehiculo->getWheelInfo(0);

	for (int i = 0; i < vehiculo->getNumWheels(); i++) {
		btWheelInfo &wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = btScalar(suspensionStiffness);    // a mas valor mas altura del chasis respecto a las ruedas va en funcion de compresion y relajacion
		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f; 
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f; 
		wheel.m_frictionSlip = btScalar(frictionSlip);  //100;  //conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover 
		wheel.m_rollInfluence = btScalar(rollInfluence);       //0.1f;  //Empieza a rodar muy loco, si el valor es alto 
		wheel.m_maxSuspensionForce = btScalar(suspForce);  //A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa) 
		wheel.m_maxSuspensionTravelCm = btScalar(suspTravelCm); //a menos valor la suspension es mas dura por lo tanto el chasis no baja tanto 										   
	}
}

void Corredor::setParametros() {
	const char* objeto = "assets/Karts/Vikingo/vikingoConIzq.obj";

	//cambiar parametros en funcion del tipo
	int num = 0;

	switch (tipojugador) {
	case GLADIADOR:

		//cuboNodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/red.png"));
		cuboNodo = TMotor::instancia().newMeshNode("Jugador", "assets/Animacion/Gladiador/giroDer/GladiGiroDer_000538.obj", "escena_raiz", false);
		//quieto = TMotor::instancia().createStaticMeshNode(cuboNodo->getNode()->getPadre(), "assets/Animacion/Gladiador/giroDer/GladiGiroDer_000538.obj", "quieto");
		giroDerIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/giroDer/GladiGiroDer_000", 538, 555), "GiroDerIni");
		giroDerFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/giroDer/GladiGiroDer_000", 555, 563), "GiroDerFin");
		giroIzqIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/giroIzq/GladiGiroIzq_000", 538, 555), "GiroIzqIni");
		giroIzqFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/giroIzq/GladiGiroIzq_000", 555, 563), "GiroIzqFin");
		//vacile = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/Vacile/gladiVacile_000", 505, 591), "Vacile");
		//lanzarObjeto = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/Lanzamiento/GladiLanzamiento_000", 517, 530), "LanzarObj");
		//animHabilidad = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/Habilidad/GladiHabilidad_000", 517, 540), "Habilidad");

		//----ACELERACION-----
		FuerzaMaxima = btScalar(3800);
		Fuerza = FuerzaMaxima;
		//----VELOCIDAD-------
		velocidadMedia = 375;
		velocidadMaximaTurbo = 490;
		velocidadMaxima = velocidadMedia;
		//----GIRO/MANEJO-----
		indiceGiroAlto = btScalar(0.4);
		indiceGiroBajo = btScalar(0.063);
		velocidadLimiteGiro = 150;
		//------PESO------
		//Masa = btScalar(400);
		//-----HABILIDAD-----
		num = 1;
		break;
	case PIRATA:


		/* Nodos de animacion */

		cuboNodo = TMotor::instancia().newMeshNode("Jugador", "assets/Animacion/Pirata/GiroDer/pirataGiroDer_000162.obj", "escena_raiz", false);
		giroDerIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/GiroDer/pirataGiroDer_000", 162, 176), "GiroDerIni");
		giroDerFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/GiroDer/pirataGiroDer_000", 176, 187), "GiroDerFin");
		giroIzqIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/GiroIzq/pirataGiroIzq_000", 162, 176), "GiroIzqIni");
		giroIzqFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/GiroIzq/pirataGiroIzq_000", 176, 187), "GiroIzqFin");

		//quieto = TMotor::instancia().createStaticMeshNode(cuboNodo->getNode()->getPadre(), "assets/Animacion/Pirata/GiroDer/pirataGiroDer_000162.obj", "quieto");
		//lanzarObjeto = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/LanzamientoOBJ/pirataLanzamiento_000", 639, 652), "LanzarObj");
		//animHabilidad = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/Habilidad/PirataHabilidad_000", 165, 202), "Habilidad");
		//vacile = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/Vacile/VacilePirata_000", 500, 551), "Vacile");


		//----ACELERACION-----
		FuerzaMaxima = btScalar(4200); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
		Fuerza = FuerzaMaxima;
		//----VELOCIDAD-------
		velocidadMedia = 380;
		velocidadMaximaTurbo = 495;
		velocidadMaxima = velocidadMedia;
		//----GIRO/MANEJO-----
		indiceGiroAlto = btScalar(0.4);
		indiceGiroBajo = btScalar(0.065);
		velocidadLimiteGiro = 130;
		//------PESO------
		//Masa = btScalar(400);
		//-----HABILIDAD-----
		num = 2;
		break;
	case VIKINGO:
		cuboNodo = TMotor::instancia().newMeshNode("Jugador", "assets/Animacion/Vikingo/giroDer/vikingoGiroDer_000534.obj", "escena_raiz", false);
		//quieto = TMotor::instancia().createStaticMeshNode(cuboNodo->getNode()->getPadre(), "assets/Animacion/Vikingo/giroDer/vikingoGiroDer_000534.obj", "quieto");
		giroDerIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/giroDer/vikingoGiroDer_000", 534, 555), "GiroDerIni");
		giroDerFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/giroDer/vikingoGiroDer_000", 555, 566), "GiroDerFin");
		giroIzqIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/giroIzq/vikingoGiroIzq_000", 534, 555), "GiroIzqIni");
		giroIzqFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/giroIzq/vikingoGiroIzq_000", 555, 566), "GiroIzqFin");
		//vacile = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/Vacile/vikingoVacile_000", 432, 481), "Vacile");
		//lanzarObjeto = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/Lanzamiento/vikingoLanzamiento_000", 516, 530), "LanzarObj");
		//animHabilidad = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/Habilidad/vikingoKartHabilidad_000", 499, 515), "Habilidad");

		//----ACELERACION-----
		FuerzaMaxima = btScalar(3600); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
		Fuerza = FuerzaMaxima;
		//----VELOCIDAD-------
		velocidadMedia = 375;
		velocidadMaximaTurbo = 490;
		velocidadMaxima = velocidadMedia;
		//----GIRO/MANEJO-----
		indiceGiroAlto = btScalar(0.4);
		indiceGiroBajo = btScalar(0.07);
		velocidadLimiteGiro = 180;
		//------PESO------
		//Masa = btScalar(400);
		//-----HABILIDAD-----
		num = 3;
		break;

	case CHINO:

		cuboNodo = TMotor::instancia().newMeshNode("Jugador", "assets/Animacion/Chino/giroDer/ChinoGiroDer_000179.obj", "escena_raiz", false);
		//quieto = TMotor::instancia().createStaticMeshNode(cuboNodo->getNode()->getPadre(), "assets/Animacion/Chino/giroDer/ChinoGiroDer_000179.obj", "quieto");
		giroDerIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/giroDer/ChinoGiroDer_000", 179, 190), "GiroDerIni");
		giroDerFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/giroDer/ChinoGiroDer_000", 190, 201), "GiroDerFin");
		giroIzqIni = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/giroIzq/ChinoGiroIzq_000", 179, 190), "GiroIzqIni");
		giroIzqFin = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/giroIzq/ChinoGiroIzq_000", 190, 201), "GiroIzqFin");
		//vacile = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/Vacile/Chinovacile_000", 223, 291), "Vacile");
		//lanzarObjeto = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/Lanzamiento/ChinoLanzamiento_000", 648, 662), "LanzarObj");
		//animHabilidad = TMotor::instancia().createAnimationNode(cuboNodo->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/Habilidad/ChinoHabilidad_000", 216, 259), "Habilidad");

		//----ACELERACION-----
		FuerzaMaxima = btScalar(4000); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
		Fuerza = FuerzaMaxima;
		//----VELOCIDAD-------
		velocidadMedia = 385;
		velocidadMaximaTurbo = 500;
		velocidadMaxima = velocidadMedia;
		//----GIRO/MANEJO-----
		indiceGiroAlto = btScalar(0.4);
		indiceGiroBajo = btScalar(0.06);
		velocidadLimiteGiro = 110;
		//------PESO------
		//Masa = btScalar(1200);
		//-----HABILIDAD-----
		num = 4;
		break;
	}
	//camara asignada al jugador

}


void Corredor::InicializarFisicasRuedas() {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	float masar = 0.003f;
	float radio = 1.5;
	float ancho = 1.2;
	btVector3 HalfExtents(1, 1, 1);

	//posicion inicial
	btTransform transRueda;
	transRueda.setIdentity();
	btVector3 posTransRueda = btVector3(rueda1->getPosition().x, rueda1->getPosition().y, rueda1->getPosition().z);
	transRueda.setOrigin(posTransRueda);
	btQuaternion quaternion;
	quaternion.setEulerZYX(rueda1->getRotation().z* PI / 180, rueda1->getRotation().y * PI / 180, rueda1->getRotation().x* PI / 180);
	transRueda.setRotation(quaternion);

	//Motionstate
	btDefaultMotionState *motionStateRueda1 = new btDefaultMotionState(transRueda); //motionState = interpolacion
	btCollisionShape *FormaColisionR1 = new btSphereShape(radio);

	// Add mass
	btVector3 LocalInertia;
	FormaColisionR1->calculateLocalInertia(masar, LocalInertia);


	CuerpoColisionRueda1 = new btRigidBody(masar, motionStateRueda1, FormaColisionR1, LocalInertia);
	CuerpoColisionRueda1->setUserPointer((void *)(rueda1));

	CuerpoColisionRueda1->setActivationState(DISABLE_DEACTIVATION);
	//CuerpoColisionRueda1->setCollisionFlags(CuerpoColisionRueda1->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// Add it to the world
	mundo->addRigidBody(CuerpoColisionRueda1);
	objetos.push_back(CuerpoColisionRueda1);

	// rueda 2

	transRueda.setIdentity();
	posTransRueda = btVector3(rueda2->getPosition().x, rueda2->getPosition().y, rueda2->getPosition().z);
	transRueda.setOrigin(posTransRueda);
	quaternion.setEulerZYX(rueda2->getRotation().z* PI / 180, rueda2->getRotation().y * PI / 180, rueda2->getRotation().x* PI / 180);
	transRueda.setRotation(quaternion);

	//Motionstate
	btDefaultMotionState *motionStateRueda2 = new btDefaultMotionState(transRueda); //motionState = interpolacion
	btCollisionShape *FormaColisionR2 = new btSphereShape(radio);

	CuerpoColisionRueda2 = new btRigidBody(masar, motionStateRueda2, FormaColisionR2, LocalInertia);
	CuerpoColisionRueda2->setUserPointer((void *)(rueda2));
	CuerpoColisionRueda2->setActivationState(DISABLE_DEACTIVATION);

	// Add it to the world
	mundo->addRigidBody(CuerpoColisionRueda2);
	objetos.push_back(CuerpoColisionRueda2);


	// rueda 3
	transRueda.setIdentity();
	posTransRueda = btVector3(rueda3->getPosition().x, rueda3->getPosition().y, rueda3->getPosition().z);
	transRueda.setOrigin(posTransRueda);
	quaternion.setEulerZYX(rueda3->getRotation().z* PI / 180, rueda3->getRotation().y * PI / 180, rueda3->getRotation().x* PI / 180);
	transRueda.setRotation(quaternion);

	//Motionstate
	btDefaultMotionState *motionStateRueda3 = new btDefaultMotionState(transRueda); //motionState = interpolacion
	btCollisionShape *FormaColisionR3 = new btSphereShape(radio);

	CuerpoColisionRueda3 = new btRigidBody(masar, motionStateRueda3, FormaColisionR3, LocalInertia);
	CuerpoColisionRueda3->setUserPointer((void *)(rueda3));
	CuerpoColisionRueda3->setActivationState(DISABLE_DEACTIVATION);

	// Add it to the world
	mundo->addRigidBody(CuerpoColisionRueda3);
	objetos.push_back(CuerpoColisionRueda3);

	// rueda 4
	transRueda.setIdentity();
	posTransRueda = btVector3(rueda4->getPosition().x, rueda4->getPosition().y, rueda4->getPosition().z);
	transRueda.setOrigin(posTransRueda);
	quaternion.setEulerZYX(rueda4->getRotation().z* PI / 180, rueda4->getRotation().y * PI / 180, rueda4->getRotation().x* PI / 180);
	transRueda.setRotation(quaternion);

	//Motionstate
	btDefaultMotionState *motionStateRueda4 = new btDefaultMotionState(transRueda); //motionState = interpolacion
	btCollisionShape *FormaColisionR4 = new btSphereShape(radio);

	CuerpoColisionRueda4 = new btRigidBody(masar, motionStateRueda4, FormaColisionR4, LocalInertia);
	CuerpoColisionRueda4->setUserPointer((void *)(rueda4));
	CuerpoColisionRueda4->setActivationState(DISABLE_DEACTIVATION);

	// Add it to the world
	mundo->addRigidBody(CuerpoColisionRueda4);
	objetos.push_back(CuerpoColisionRueda4);


	//Restricciones para las ruedas
	btVector3 axisA(1.f, 1.f, 1.f);
	btVector3 axisB(1.f, 1.f, 1.f);
	btVector3 pivotA(3.f, 5.f, 3.f);
	btVector3 pivotB(0.f, 0.f, 0.f);

	btHingeConstraint *restriccion1 = new btHingeConstraint(*CuerpoColisionChasis, *CuerpoColisionRueda1, pivotA, pivotB, axisA, axisB, false);
	pivotA = btVector3(-3.f, 5.f, 3.f);
	btHingeConstraint *restriccion2 = new btHingeConstraint(*CuerpoColisionChasis, *CuerpoColisionRueda2, pivotA, pivotB, axisA, axisB, false);
	pivotA = btVector3(-3.f, 5.f, -3.f);
	btHingeConstraint *restriccion3 = new btHingeConstraint(*CuerpoColisionChasis, *CuerpoColisionRueda3, pivotA, pivotB, axisA, axisB, false);
	pivotA = btVector3(3.f, 5.f, -3.f);
	btHingeConstraint *restriccion4 = new btHingeConstraint(*CuerpoColisionChasis, *CuerpoColisionRueda4, pivotA, pivotB, axisA, axisB, false);

	mundo->addConstraint(restriccion1, true);
	mundo->addConstraint(restriccion2, true);
	mundo->addConstraint(restriccion3, true);
	mundo->addConstraint(restriccion4, true);

	bullet->setObjetos(objetos);
}

//-----------------------------//
//----------FISICAS------------//
//-----------------------------//
void Corredor::InicializarFisicas() {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	std::vector<btRigidBody *> objetos = bullet->getObjetos();

	//posicion inicial
	btTransform transCoche;
	transCoche.setIdentity();
	btVector3 posTransCoche = btVector3(cuboNodo->getPosition().x, cuboNodo->getPosition().y, cuboNodo->getPosition().z);
	transCoche.setOrigin(posTransCoche);
	btQuaternion quaternion;
	quaternion.setEulerZYX(cuboNodo->getRotation().z* PI / 180, cuboNodo->getRotation().y * PI / 180, cuboNodo->getRotation().x* PI / 180);
	transCoche.setRotation(quaternion);

	//establecemos su centro de gravedad
	btTransform localTransform;
	localTransform.setIdentity();
	localTransform.setOrigin(btVector3(0, 1.5, 0));
	btCompoundShape *CentroGravedad = new btCompoundShape();

	//Forma Colision
	btVector3 TamanyoFormaColision(cuboNodo->getScale().x * 2, cuboNodo->getScale().y * 1.2, cuboNodo->getScale().z * 2);
	btCollisionShape *FormaColision = new btBoxShape(TamanyoFormaColision);
	//masa coche
	btVector3 Inercia(0, 0, 0);
	FormaColision->calculateLocalInertia(Masa, Inercia);

	CentroGravedad->addChildShape(localTransform, FormaColision);

	//rigidbody del coche
	CuerpoColisionChasis = new btRigidBody(Masa, new btDefaultMotionState(transCoche), CentroGravedad, Inercia);
	CuerpoColisionChasis->setFriction(1110);
	CuerpoColisionChasis->setUserPointer((void *)(cuboNodo));

	//RaycastDel Coche
	RayCastVehiculo = new btDefaultVehicleRaycaster(mundo);
	btRaycastVehicle::btVehicleTuning tuning;

	vehiculo = new btRaycastVehicle(tuning, CuerpoColisionChasis, RayCastVehiculo);
	CuerpoColisionChasis->setActivationState(DISABLE_DEACTIVATION);
	mundo->addVehicle(vehiculo);

	objetos.push_back(CuerpoColisionChasis);
	CrearRuedas(vehiculo, tuning);
	mundo->addRigidBody(CuerpoColisionChasis);
	bullet->setObjetos(objetos);

	// Desasignacion de punteros
	CentroGravedad = nullptr;
}

void Corredor::CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning) {

	//btVector3 TamanyoFormaColision(1,btScalar(0.5),2);
	btVector3 puntoConexionChasis(1 - radioRueda, alturaConexionChasis, 2 - anchoRueda);

	vehiculo->setCoordinateSystem(0, 1, 2); // 0, 1, 2

	// Agrega las ruedas delanteras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, 2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);
	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, 2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	// Agrega las ruedas traseras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, -2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);
	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, -2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);


	for (int i = 0; i < vehiculo->getNumWheels(); i++) {
		btWheelInfo &wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = btScalar(20);    // a mas valor mas altura del chasis respecto a las ruedas va en funcion de compresion y relajacion
		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f;  
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f;  
		wheel.m_frictionSlip = btScalar(10000);  //100;  //conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover 
		wheel.m_rollInfluence = btScalar(0);       //0.1f;  //Empieza a rodar muy loco, si el valor es alto 
		wheel.m_maxSuspensionForce = 50000.f;  //A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa)  
		wheel.m_maxSuspensionTravelCm = 10000.f; //a menos valor la suspension es mas dura por lo tanto el chasis no baja tanto 										   
	}

}

void Corredor::BorrarFisicas() {}

//-----------------------------//
//---------METODOS GET---------//
//-----------------------------//
int Corredor::getCargador() { return cargador; };
int Corredor::getTipoObj() { return tipoObj; };
int Corredor::getPosicionCarrera() { return posicionCarrera; }
bool Corredor::getTurbo() { return turboActivado; }
btRaycastVehicle *Corredor::getVehiculo() { return vehiculo; }
btRigidBody *Corredor::getRigidBody() { return CuerpoColisionChasis; }
obj3D *Corredor::getNodo() { return cuboNodo; }
btVector3 Corredor::getVectorDireccion() { return orientacion; }
bool Corredor::getProteccion() { return proteccion; }
Waypoint *Corredor::getWaypointActual() { return actual; }
Waypoint *Corredor::getWaypointSiguiente() { return siguiente; }
btScalar Corredor::getdistanciaWaypoint() { return distanciaWaypoint; }
btScalar Corredor::getdistanciaWaypointActual() { return distanciaWaypointActual; }
int Corredor::getVueltas() { return vueltas; }
int Corredor::getLimite() { return limite; }
bool Corredor::getCheckItem() { return checkItem; }
EstadosJugador *Corredor::getEstados() { return estado; }
const char* Corredor::getNombre() { return nombre; }
int Corredor::getID() { return id; }
Corredor::tipo_jugador Corredor::getTipoJugador() { return tipojugador; }
bool Corredor::getAceiteActivado() { return aceiteActivado; }
float Corredor::getMaxVuetas() { return maxvueltas; }
bool Corredor::getInmunidad() { return inmunidad; }
float Corredor::getTiempoVueltaTotal() { return tiempoVueltaTotal; }

float Corredor::getTiempoVuelta() { return tiempoVuelta; }

//-----------------------------//
//---------METODOS SET---------//
//-----------------------------//
void Corredor::setVueltas(int j) { vueltas = j; }
void Corredor::setMaxVueltas(int i) { maxvueltas = i; }
void Corredor::SetFuerzaVelocidad(int turbo) { Fuerza = btScalar(turbo); }
void Corredor::setProteccion(bool s) { proteccion = s; }
void Corredor::setLimite(int s) { limite = s; }
void Corredor::setCheckItem(bool s) { checkItem = s; }
void Corredor::setPosDisparo(btVector3 s) { posDisparo = s; }

void Corredor::setPosicion(float *pos, float *ori) {
	btVector3 btPos(pos[0], pos[1], pos[2]);

	btTransform trans;
	trans.setOrigin(btPos);
	btQuaternion quaternion;
	quaternion.setEulerZYX(ori[0] * PI / 180, ori[1] * PI / 180, ori[2] * PI / 180);
	trans.setRotation(quaternion);

	CuerpoColisionChasis->setCenterOfMassTransform(trans);

}

void Corredor::setPosicionBullet(float *pos, btQuaternion ori) {
	btVector3 btPos(pos[0], pos[1], pos[2]);

	btTransform trans;
	trans.setOrigin(btPos);
	trans.setRotation(ori);

	CuerpoColisionChasis->setCenterOfMassTransform(trans);

}

void Corredor::setPosicionSources() {
	float *pos = new float[3];
	pos[0] = cuboNodo->getPosition().x;
	pos[1] = cuboNodo->getPosition().y;
	pos[2] = cuboNodo->getPosition().z;
	fuenteMotor->setPitch(pitchMotor);
	fuenteMotor->setPosition(pos);
	fuenteItem->setPosition(pos);
}

void Corredor::setPosicionCarrera(int i, int j) {
	if (j == 0) {
		if (vueltas <= maxvueltas)
			posicionCarrera = i;
	}
	else
		posicionCarrera = i;
}

void Corredor::setTipoObj() {
	int random;
	srand(time(NULL));
	random = rand() % 100 + 1;
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	int totalJugadores = jugadores->getNumJugadores();
	int SimPosCarrera;

	/*
		En caso de no ser el total de jugadores, adaptamos la posicion
		a otra para que cambien las probabilidades de los objetos
	*/
	if (totalJugadores == 2) {
		if (posicionCarrera == 1) {
			SimPosCarrera = 3;
		}
		else if (posicionCarrera == 2) {
			SimPosCarrera = 4;
		}
	}
	else if (totalJugadores == 3) {
		if (posicionCarrera == 1) {
			SimPosCarrera = 2;
		}
		else if (posicionCarrera == 2) {
			SimPosCarrera = 3;
		}
		else if (posicionCarrera == 3) {
			SimPosCarrera = 4;
		}
	}
	else if (totalJugadores == 4) {
		if (posicionCarrera == 1) {
			SimPosCarrera = 2;
		}
		else if (posicionCarrera == 2) {
			SimPosCarrera = 3;
		}
		else if (posicionCarrera == 3) {
			SimPosCarrera = 4;
		}
		else if (posicionCarrera == 4) {
			SimPosCarrera = 5;
		}
	}

	else {
		SimPosCarrera = posicionCarrera;
	}

	/*
					1º	2º-3º	4º-5º	6º
1.	Flecha			15	15		15		10
2.	Caja falsa		27	15		10		3
3.	Turbo			10	15		15		15
4.	Aceite			26	15		10		2
5.	Escudo			17	15		10		15
6.	Flecha Triple	5	10		15		15
7.	Flecha Tele		0	5		10		25
8.	Turbo Triple	0	10		15		15
	*/
	if (SimPosCarrera == 1) {
		if (random < 16)
			tipoObj = EstadosJugador::estado_objeto::FLECHA;
		else if (random < 43)
			tipoObj = EstadosJugador::estado_objeto::CAJA_FALSA;
		else if (random < 53)
			tipoObj = EstadosJugador::estado_objeto::TURBO;
		else if (random < 79)
			tipoObj = EstadosJugador::estado_objeto::ACEITE;
		else if (random < 96)
			tipoObj = EstadosJugador::estado_objeto::ESCUDO;
		else if (random < 101)
			tipoObj = EstadosJugador::estado_objeto::FLECHA_TRIPLE;
	}
	else if (SimPosCarrera == 2 || SimPosCarrera == 3) {
		if (random < 16)
			tipoObj = EstadosJugador::estado_objeto::FLECHA;
		else if (random < 31)
			tipoObj = EstadosJugador::estado_objeto::CAJA_FALSA;
		else if (random < 46)
			tipoObj = EstadosJugador::estado_objeto::TURBO;
		else if (random < 61)
			tipoObj = EstadosJugador::estado_objeto::ACEITE;
		else if (random < 76)
			tipoObj = EstadosJugador::estado_objeto::ESCUDO;
		else if (random < 86)
			tipoObj = EstadosJugador::estado_objeto::FLECHA_TRIPLE;
		else if (random < 91)
			tipoObj = EstadosJugador::estado_objeto::ITEM_TELEDIRIGIDO;
		else if (random < 101)
			tipoObj = EstadosJugador::estado_objeto::TURBO_TRIPLE;
	}
	else if (SimPosCarrera == 4 || SimPosCarrera == 5) {
		if (random < 16)
			tipoObj = EstadosJugador::estado_objeto::FLECHA;
		else if (random < 26)
			tipoObj = EstadosJugador::estado_objeto::CAJA_FALSA;
		else if (random < 41)
			tipoObj = EstadosJugador::estado_objeto::TURBO;
		else if (random < 51)
			tipoObj = EstadosJugador::estado_objeto::ACEITE;
		else if (random < 61)
			tipoObj = EstadosJugador::estado_objeto::ESCUDO;
		else if (random < 76)
			tipoObj = EstadosJugador::estado_objeto::FLECHA_TRIPLE;
		else if (random < 86)
			tipoObj = EstadosJugador::estado_objeto::ITEM_TELEDIRIGIDO;
		else if (random < 101)
			tipoObj = EstadosJugador::estado_objeto::TURBO_TRIPLE;
	}
	else if (SimPosCarrera == 6) {
		if (random < 11)
			tipoObj = EstadosJugador::estado_objeto::FLECHA;
		else if (random < 14)
			tipoObj = EstadosJugador::estado_objeto::CAJA_FALSA;
		else if (random < 29)
			tipoObj = EstadosJugador::estado_objeto::TURBO;
		else if (random < 31)
			tipoObj = EstadosJugador::estado_objeto::ACEITE;
		else if (random < 46)
			tipoObj = EstadosJugador::estado_objeto::ESCUDO;
		else if (random < 61)
			tipoObj = EstadosJugador::estado_objeto::FLECHA_TRIPLE;
		else if (random < 76)
			tipoObj = EstadosJugador::estado_objeto::ITEM_TELEDIRIGIDO;
		else if (random < 101)
			tipoObj = EstadosJugador::estado_objeto::TURBO_TRIPLE;
	}

	//cout << "Posicion: " << posicionCarrera << " - NumRandom: " << random << " - Objeto: " << tipoObj << endl;

	if (tipoObj == 8)
		cargador = 3;
	else if (tipoObj == 3)
		cargador = 1;

	//cout << "Random ------>" << tipoObj << endl;
}
void Corredor::setTipoObj(EstadosJugador::estado_objeto objeto) {
	tipoObj = objeto;
	if (tipoObj == 8)
		cargador = 3;
	else if (tipoObj == 3)
		cargador = 1;
	//cout << "el objeto --- " << i << " ---" << endl;
	//cout << "Random ------>" << tipoObj << endl;
}

void Corredor::setWaypointActual(obj3D *nodo) {
	//de momento lo pongo así, no da la segunda vuelta pero habria que mirar cuales se han visitado y cuales no
	//mas adelante se ve, Las IDS no funcionan bien tengo que preguntarle a santi a si que de momento lo comento para que
	//se puedan coger las cajas.

	bool b = false;	//booleano para comprobar si esta siguiendo bien los waypoints
	if (nodo->getID() > actual->getID() && nodo->getID() == siguiente_aux->getID()) {

		actual = actual->getNextWaypoint();
		siguiente = actual->getNextWaypoint();

		if (actual->getID() == siguiente_aux->getID()) {
			siguiente_aux = siguiente;
			b = true;
			direccionContraria = 0;
		}
	}

	if (nodo->getID() - 7 == 0) { //caso de vuelta completa

		if (siguiente_aux->getID() - 7 == 0) {
			//cout<<"HAS DADO UNA VUELTA"<<endl;
			if (getMaxVuetas() >= getVueltas()) {
				cout << "VUELTA: " << tiempoVuelta << endl;
			}
			vueltas++;
			actual = siguiente_aux;
			siguiente = actual->getNextWaypoint();
			siguiente_aux = actual->getNextWaypoint();
			b = true;
			direccionContraria = 0;
		}

	}

	if (b == false && nodo->getID() - 7 != 0 && nodo->getID() >= siguiente_aux->getWaypoint()->getID() - 2 && direccionContraria != 2) { //comprobador de recuperacion de direccion correcta en caso de no estar en la meta
		direccionContraria = 0;
	}
	Pista *mapa = Pista::getInstancia();
	//comprobadores direccion opuesta
	if ((b == false && nodo->getID() < siguiente_aux->getID() - 4 && nodo->getID() - 7 != 0) || (mapa->getTamArrayWaypoints() - 1 == nodo->getID() - 7 && (actual->getID() - 7 != mapa->getTamArrayWaypoints() - 1))) {
		direccionContraria = 1;
		if ((mapa->getTamArrayWaypoints() - 1 == nodo->getID() - 7 && (actual->getID() - 7 != mapa->getTamArrayWaypoints() - 1))) {
			direccionContraria = 2;	//va marcha atras en la meta id=0 la comprobacion es distinta (por ser mayor el id siempre, tanto para alante como para atras)
		}
	}


	//cout<< "NODO ACTUAL:"<< actual->getWaypoint()->getID() <<endl;
	//cout<< "NODO SIGUIENTE:"<< siguiente->getWaypoint()->getID() <<endl;
}

void Corredor::setTurbo(bool activo, bool objeto, int valor, int tiempo) {
	turboActivado = activo;
	tiempoTurbo = tiempo;
	if (turboActivado) {
		SetFuerzaVelocidad(valor);
		velocidadMaxima = velocidadMaximaTurbo;
		Timer *time = Timer::getInstancia();
		timerTurbo = time->getTimer();
		if (pitchMotor < 2)	pitchMotor += 0.4f;
		if (objeto) {
			if (cargador == 1)
				tipoObj = EstadosJugador::estado_objeto::NADA;
			cargador--;
			//cout << "Turbos restantes: " << cargador << endl;
		}
	}
	else {
		SetFuerzaVelocidad(FuerzaMaxima);
		velocidadMaxima = velocidadMedia;
	}
}

void Corredor::comprobarTurbo() {
	if (turboActivado) {
		Timer *time = Timer::getInstancia();
		acelerar();
		if (time->getTimer() - timerTurbo >= tiempoTurbo) {
			setTurbo(false, false, 0, 0);
		}
	}
}

void Corredor::setInmunidad(bool activo) {
	inmunidad = activo;

	if (inmunidad) {
		Timer *time = Timer::getInstancia();
		timerInmunidad = time->getTimer();
	}
}

void Corredor::comprobarInmunidad() {
	if (inmunidad) {
		estado->setEstadoInmunidad(EstadosJugador::estado_inmunidad::INMUNIDAD);
		resetFuerzas();
		CuerpoColisionChasis->setCollisionFlags(CuerpoColisionChasis->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);
		CuerpoColisionRueda1->setCollisionFlags(CuerpoColisionRueda1->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);
		CuerpoColisionRueda2->setCollisionFlags(CuerpoColisionRueda2->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);
		CuerpoColisionRueda3->setCollisionFlags(CuerpoColisionRueda3->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);
		CuerpoColisionRueda4->setCollisionFlags(CuerpoColisionRueda4->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);
		Timer *time = Timer::getInstancia();
		if (time->getTimer() - timerInmunidad >= tiempoInmunidad) {
			CuerpoColisionChasis->setCollisionFlags(0);
			CuerpoColisionRueda1->setCollisionFlags(0);
			CuerpoColisionRueda2->setCollisionFlags(0);
			CuerpoColisionRueda3->setCollisionFlags(0);
			CuerpoColisionRueda4->setCollisionFlags(0);
			inmunidad = false;
			estado->setEstadoInmunidad(EstadosJugador::estado_inmunidad::NORMAL);
		}
	}
}

void Corredor::setHabilidad(bool activo) {
	habilidadLanzada = activo;
	if (habilidadLanzada)
		estado->setEstadoHabilidad(EstadosJugador::estado_habilidad::CONHABILIDAD);
	else
		estado->setEstadoHabilidad(EstadosJugador::estado_habilidad::SINHABILIDAD);
}

void Corredor::setFriccion(btScalar valor) {
	for (int i = 0; i < vehiculo->getNumWheels(); i++) {
		vehiculo->getWheelInfo(i).m_frictionSlip = btScalar(valor);  //100;	//conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover	
	}
}

void Corredor::setTipoJugador(int tj) {
	switch (tj) {
	case 0:
		tipojugador = GLADIADOR;
		break;
	case 1:
		tipojugador = PIRATA;
		break;
	case 2:
		tipojugador = VIKINGO;
		break;
	case 3:
		tipojugador = CHINO;
		break;
	}
}

void Corredor::setTiempoVuelta(float t) {
	tiempoVueltaTotal += t;
	tiempoVuelta = t;
}

//-------------------------------------//
//-------TRATAMIENTOS OBJETOS----------//
//-------------------------------------//

void Corredor::lanzarItemTeledirigido() {
	resetFuerzas();
	aplicarAceite();
	//tipoObj = 0;
}
void Corredor::setObjetivoTelederigido() {
	objetivoFijado = true;
	Timer *time = Timer::getInstancia();
	timerTeledirigido = time->getTimer();
}
void Corredor::setAceite() {
	aceiteActivado = true;
	Timer *time = Timer::getInstancia();
	timerAceite = time->getTimer();
}
void Corredor::setID(int i) {
	id = i;
	cuboNodo->setID(id);
}
void Corredor::resetFuerzas() {
	CuerpoColisionChasis->clearForces();
	btVector3 zeroVector(0, 0, 0);
	CuerpoColisionChasis->setLinearVelocity(zeroVector);
	CuerpoColisionChasis->setAngularVelocity(zeroVector);
}
void Corredor::soltarItem() {
	setTipoObj(EstadosJugador::estado_objeto::NADA);
	setLimite(getLimite() + 10);
}

/*
Tipos de objeto:
	1. Proyectil/Flecha
	2. Estatico/Caja Falsa
	3. Turbo
	4. Aceite
	5. Escudo
	6. Flecha Triple
	7. Flecha Teledirigida
	8. Turbo Triple
	9. HABILIDAD (No entra en el pool de objetos)
*/

void Corredor::usarObjetos() {
	Pista *pista = Pista::getInstancia();
	std::vector<Item *> items = pista->getItems();

	btVector3 posicion(cuboNodo->getPosition().x + orientacion.getX() * 10, cuboNodo->getPosition().y + 2, cuboNodo->getPosition().z + orientacion.getZ() * 10);
	btVector3 escala(2, 2, 2);
	btScalar masa = 50;
	float tiempoDestruccion = 15;
	btVector3 tamanyoNodo(1, 1, 1);
	btScalar radio = 8;
	float alt = 1;

	if (getTipoObj() == 1) {		// PROYECTIL
		Proyectil *pro = new Proyectil(posicion, escala, masa, tiempoDestruccion, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
		pro->lanzarItem(1, orientacion, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		pro->setLanzado(true);
		items.push_back(pro);
		soltarItem();
	}
	else if (getTipoObj() == 2) {	//CAJA FALSA
		alt = 3;
		masa = 30;
		tiempoDestruccion = 0;
		escala = btVector3(2, 2, 2);
		posicion.setX(cuboNodo->getPosition().x - orientacion.getX() * 10);
		posicion.setZ(cuboNodo->getPosition().z - orientacion.getZ() * 10);
		posicion.setY(cuboNodo->getPosition().y + alt);
		CajaFalsa *est = new CajaFalsa(posicion, escala, masa, tiempoDestruccion, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
		est->lanzarItem(0, btVector3(0, 0, 0), CuerpoColisionChasis->getCenterOfMassTransform());
		items.push_back(est);

		soltarItem();
	}
	else if (getTipoObj() == 3) {	//TURBO
		setTurbo(true, true, FuerzaMaxima * 4, 2);
	}
	else if (getTipoObj() == 4) {	//ACEITE
		alt = 3;
		masa = 30;
		tiempoDestruccion = 0;
		escala = btVector3(2, 2, 2);
		posicion.setX(cuboNodo->getPosition().x - orientacion.getX() * 10);
		posicion.setZ(cuboNodo->getPosition().z - orientacion.getZ() * 10);
		posicion.setY(cuboNodo->getPosition().y + alt);
		Aceite *est2 = new Aceite(posicion, escala, masa, tiempoDestruccion, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
		est2->lanzarItem(0, btVector3(0, 0, 0), CuerpoColisionChasis->getCenterOfMassTransform());
		items.push_back(est2);

		soltarItem();
	}
	else if (getTipoObj() == 5) {//ESCUDO
		//if (getProteccion() == false) setProteccion(true);
		//decCargador();
		if (!proteccion) {
			alt = 0;
			masa = 50;
			radio = 5;
			escala = btVector3(5, 5, 5);
			tamanyoNodo = btVector3(9, 9, 9);
			tiempoDestruccion = 30;
			posicion = btVector3(cuboNodo->getPosition().x, cuboNodo->getPosition().y, cuboNodo->getPosition().z);
			posicion.setY(posicion.getY() + alt);
			Escudo *escudo = new Escudo(CuerpoColisionChasis, posicion, escala, masa, tiempoDestruccion, ESFERA, tamanyoNodo, radio, alt, cuboNodo->getID());
			escudo->setLanzado(true);
			items.push_back(escudo);
			soltarItem();
			proteccion = true;
		}
	}
	else if (getTipoObj() == 6)	//FLECHA TRIPLE
	{

		btVector3 orientacioncentral(orientacion.getX(), orientacion.getY(), orientacion.getZ());
		btVector3 centro(cuboNodo->getPosition().x + orientacion.getX() * 10, cuboNodo->getPosition().y, cuboNodo->getPosition().z + orientacion.getZ() * 10);
		btVector3 orientacionderecha = orientacioncentral.rotate(btVector3(0, 1, 0), 10 * PI / 180);
		btVector3 orientacionizquierda = orientacioncentral.rotate(btVector3(0, 1, 0), -10 * PI / 180);
		btVector3 iz(cuboNodo->getPosition().x + orientacionizquierda.getX() * 10, cuboNodo->getPosition().y, cuboNodo->getPosition().z + orientacionizquierda.getZ() * 10);
		btVector3 d(cuboNodo->getPosition().x + orientacionderecha.getX() * 10, cuboNodo->getPosition().y, cuboNodo->getPosition().z + orientacionderecha.getZ() * 10);

		posicion = iz;
		Proyectil *pro1 = new Proyectil(posicion, escala, masa, tiempoDestruccion, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
		//pro1->inicializarFisicas();
		pro1->lanzarItem(1, orientacionizquierda, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		pro1->setLanzado(true);
		items.push_back(pro1);

		posicion = centro;
		Proyectil *pro2 = new Proyectil(posicion, escala, masa, tiempoDestruccion, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
		//pro2->inicializarFisicas();
		pro2->lanzarItem(1, orientacion, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		pro2->setLanzado(true);
		items.push_back(pro2);

		posicion = d;
		Proyectil *pro3 = new Proyectil(posicion, escala, masa, tiempoDestruccion, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
		//pro3->inicializarFisicas();
		pro3->lanzarItem(1, orientacionderecha, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		pro3->setLanzado(true);
		items.push_back(pro3);


		//decCargador();
		soltarItem();
	}
	else if (tipoObj == 7)	//FLECHA TELEDIRIGIDA
	{
		//cout << "Teledirigido de Jugador " << this->getID() << " - posicion: " << posicionCarrera << endl;
		alt = 1;
		posicion.setY(posicion.getY() + alt);
		ItemTeledirigido *pt = new ItemTeledirigido(posicion, escala, masa, 50, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
		pt->setWaypoint(actual);
		pt->lanzarItem(1, orientacion, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		pt->setLanzado(true);
		items.push_back(pt);
		soltarItem();

	}
	else if (tipoObj == 8)	//TURBO TRIPLE
	{
		setTurbo(true, true, FuerzaMaxima * 2, 5);

	}
	//std::cout << "Tipo obj: " << getTipoObj() << " / " << items.size() << std::endl;

	pista->setItems(items);

}
void Corredor::aplicarAceite() {
	//CuerpoColisionChasis->setAngularFactor(btScalar(120*PI/180));
	//CuerpoColisionChasis->applyCentralForce(btVector3(100, 5.0f,100));
	frenodemano(true, false);
	CuerpoColisionChasis->setAngularVelocity(btVector3(0, 40, 0));
	for (int i = 0; i < 100; i++) {
		girarIzquierda();
	}

	frenodemano(false, false);
}
void Corredor::incCargador() { cargador++; };
void Corredor::decCargador() { cargador--; };

//----------------------------------------//
//-------CALCULO DE DISTANCIA-------------//
//----------------------------------------//
btScalar Corredor::getDistanciaPunto(btVector3 vector) {
	btVector3 posCoche(cuboNodo->getPosition().x, cuboNodo->getPosition().y, cuboNodo->getPosition().z);
	btVector3 posWaypoint(vector.getX(), vector.getY(), vector.getZ());
	return posCoche.distance2(posWaypoint);
}

//----------------------------------------//
//------------MOVIMIENTO------------------//
//----------------------------------------//
void Corredor::acelerar() {
	if ((vehiculo->getCurrentSpeedKmHour() > velocidadMaxima && !turboActivado) || (turboActivado && vehiculo->getCurrentSpeedKmHour() > velocidadMaximaTurbo)) {
		limitadorVelocidad();
	}
	else {
		vehiculo->applyEngineForce(Fuerza, 0);
		vehiculo->applyEngineForce(Fuerza, 1);
		vehiculo->applyEngineForce(Fuerza, 2);
		vehiculo->applyEngineForce(Fuerza, 3);
	}
	vehiculo->setSteeringValue(btScalar(0), 0);
	vehiculo->setSteeringValue(btScalar(0), 1);

	estado->setEstadoMovimiento(EstadosJugador::estado_movimiento::AVANZA);
	if (!turboActivado) {
		estado->setDireccionMovimiento(EstadosJugador::direccion_movimiento::RECTO);
	}


	if (pitchMotor < 1) pitchMotor += 0.01f;
	else if (pitchMotor > 1) pitchMotor -= 0.005f;
}
void Corredor::frenar() {
	if (vehiculo->getCurrentSpeedKmHour() < velocidadMaximaAtras) {
		vehiculo->applyEngineForce(0, 0);
		vehiculo->applyEngineForce(0, 1);
		vehiculo->applyEngineForce(0, 2);
		vehiculo->applyEngineForce(0, 3);
	}
	else {
		vehiculo->applyEngineForce(FuerzaFrenado, 0);
		vehiculo->applyEngineForce(FuerzaFrenado, 1);
		vehiculo->applyEngineForce(FuerzaFrenado, 2);
		vehiculo->applyEngineForce(FuerzaFrenado, 3);
	}

	vehiculo->setSteeringValue(btScalar(0), 0);
	vehiculo->setSteeringValue(btScalar(0), 1);

	if (vehiculo->getCurrentSpeedKmHour() < 0) {
		estado->setEstadoMovimiento(EstadosJugador::estado_movimiento::MARCHA_ATRAS);
	}
	else {
		estado->setEstadoMovimiento(EstadosJugador::estado_movimiento::FRENA);
	}
	if (!turboActivado)
		estado->setDireccionMovimiento(EstadosJugador::direccion_movimiento::RECTO);
}

void Corredor::girarDerecha() {
	estado->setDireccionMovimiento(EstadosJugador::direccion_movimiento::DERECHA);
	if (vehiculo->getCurrentSpeedKmHour() < velocidadLimiteGiro) {
		FuerzaGiro = indiceGiroAlto;
	}
	else {
		FuerzaGiro = indiceGiroBajo;
	}

	vehiculo->setSteeringValue(-FuerzaGiro, 0);
	vehiculo->setSteeringValue(-FuerzaGiro, 1);
}

void Corredor::girarIzquierda() {
	estado->setDireccionMovimiento(EstadosJugador::direccion_movimiento::IZQUIERDA);
	if (vehiculo->getCurrentSpeedKmHour() < velocidadLimiteGiro) {
		FuerzaGiro = indiceGiroAlto;
	}
	else {
		FuerzaGiro = indiceGiroBajo;
	}
	vehiculo->setSteeringValue(FuerzaGiro, 0);
	vehiculo->setSteeringValue(FuerzaGiro, 1);

}

void Corredor::frenodemano(bool activo, bool objeto) {
	int friccion = 1.f;
	if (activo) {
		if (!aceiteActivado && !turboActivado) {
			if (vehiculo->getCurrentSpeedKmHour() > 300
				&& (estado->getDireccionMovimiento() == EstadosJugador::direccion_movimiento::IZQUIERDA ||
					estado->getDireccionMovimiento() == EstadosJugador::direccion_movimiento::DERECHA))
			{
				int i = vehiculo->getCurrentSpeedKmHour();
				if (i % 3 == 0)
					limite += 1;
			}
		}

		estado->setEstadoMovimiento(EstadosJugador::estado_movimiento::DERRAPA);
		FuerzaGiro = btScalar(0.45);

		vehiculo->applyEngineForce(FuerzaFrenado, 0);
		vehiculo->applyEngineForce(FuerzaFrenado, 1);
		vehiculo->applyEngineForce(FuerzaFrenado, 2);
		vehiculo->applyEngineForce(FuerzaFrenado, 3);

		vehiculo->getWheelInfo(0).m_frictionSlip = btScalar(friccion);
		vehiculo->getWheelInfo(1).m_frictionSlip = btScalar(friccion);

		vehiculo->getWheelInfo(2).m_frictionSlip = btScalar(friccion);
		vehiculo->getWheelInfo(3).m_frictionSlip = btScalar(friccion);
	}
	else {
		vehiculo->getWheelInfo(0).m_frictionSlip = btScalar(10000);
		vehiculo->getWheelInfo(1).m_frictionSlip = btScalar(10000);

		vehiculo->getWheelInfo(2).m_frictionSlip = btScalar(10000);
		vehiculo->getWheelInfo(3).m_frictionSlip = btScalar(10000);

		FuerzaGiro = btScalar(0.1);
	}
}

void Corredor::desacelerar() {
	estado->setEstadoMovimiento(EstadosJugador::estado_movimiento::DESACELERA);
	vehiculo->applyEngineForce(0, 0);
	vehiculo->applyEngineForce(0, 1);
	vehiculo->applyEngineForce(0, 2);
	vehiculo->applyEngineForce(0, 3);

	vehiculo->setSteeringValue(0, 0);
	vehiculo->setSteeringValue(0, 1);

	vehiculo->setBrake(200, 0);
	vehiculo->setBrake(200, 1);
	vehiculo->setBrake(200, 2);
	vehiculo->setBrake(200, 3);

	if (pitchMotor > 0.5f) pitchMotor -= 0.01f;
}

void Corredor::limitadorVelocidad() {
	vehiculo->applyEngineForce(0.0001, 0);
	vehiculo->applyEngineForce(0.0001, 1);
	vehiculo->applyEngineForce(0.0001, 2);
	vehiculo->applyEngineForce(0.0001, 3);
}

void Corredor::comprobarSueloRuedas() {

	int cont = 0;
	for (int i = 0; i < vehiculo->getNumWheels(); i++) {
		if (!vehiculo->getWheelInfo(i).m_raycastInfo.m_isInContact) {

			cont++;
			//cout<<"LA RUEDA :: "<< i << "NO ESTA EN CONTACTO"<<endl;
		}

	}

	if (cont == 4) {
		if (estado->getEstadoMovimiento() != EstadosJugador::estado_movimiento::DERRAPA) {
			CuerpoColisionChasis->setLinearVelocity(btVector3(CuerpoColisionChasis->getLinearVelocity().getX(), -30, CuerpoColisionChasis->getLinearVelocity().getZ()));
			CuerpoColisionChasis->setAngularVelocity(btVector3(0, 0, 0));
		}
	}

	if (cont == 4) {
		if (estado->getEstadoMovimiento() != EstadosJugador::estado_movimiento::DERRAPA) {
			CuerpoColisionChasis->setLinearVelocity(btVector3(CuerpoColisionChasis->getLinearVelocity().getX(), -20, CuerpoColisionChasis->getLinearVelocity().getZ()));
			CuerpoColisionChasis->setAngularVelocity(btVector3(0, 0, 0));
		}
	}

}

void Corredor::recolocarWaypoint() {
	btVector3 btPos = actual->getPosicion();

	btTransform trans;

	trans.setOrigin(btPos);
	btQuaternion quaternion;
	quaternion.setEulerZYX(actual->getRotation().getZ()* PI / 180, actual->getRotation().getY() * PI / 180, actual->getRotation().getX()* PI / 180);
	trans.setRotation(quaternion);

	CuerpoColisionChasis->setCenterOfMassTransform(trans);
	resetFuerzas();
}

//---------------------------------------//
//-----------METODOS UPDATE--------------//
//---------------------------------------//
void Corredor::update() {
	if (vehiculo->getCurrentSpeedKmHour() > velocidadMaxima) {
		limitadorVelocidad();
	}
	/*
	for (unsigned int i = 0; i< sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
			std::cout << "Joystick " << i << " is connected!" << std::endl;
	}
	*/

	comprobarTurbo();
	comprobarInmunidad();
	if (estado->getEstadoCarrera() == EstadosJugador::estado_carrera::CARRERA) {
		movimiento();// Esto ni existe
		updateEstado();

	}
	updateTimerObstaculos();
	updateEstado();
	comprobarSueloRuedas();
	actualizarRuedas();
	updateVectorDireccion();
	distanciaWaypoint = getDistanciaPunto(siguiente->getPosicion());
	distanciaWaypointActual = getDistanciaPunto(actual->getPosicion());

	if (vueltas > maxvueltas) {
		estado->setEstadoCarrera(EstadosJugador::estado_carrera::FIN);
	}
	setPosicionSources();

	//Modificamos el billboard en funcion de la posicion del corredor
	if (posicionCarrera == 1) {
		b->setImage("assets/HUD/juego/puesto_1.png");
	}
	else if (posicionCarrera == 2) {
		b->setImage("assets/HUD/juego/puesto_2.png");
	}
	else if (posicionCarrera == 3) {
		b->setImage("assets/HUD/juego/puesto_3.png");
	}
	else if (posicionCarrera == 4) {
		b->setImage("assets/HUD/juego/puesto_4.png");
	}
	else if (posicionCarrera == 5) {
		b->setImage("assets/HUD/juego/puesto_5.png");
	}
	else {
		b->setImage("assets/HUD/juego/puesto_6.png");
	}

	//Establecemos los parametros de las particulas
	// POSICION
	p->setPosition(cuboNodo->getPosition());
	// ORIENTACION
	p->setOrientation(glm::vec3(orientacion.getX(), orientacion.getY(), orientacion.getZ()));
	// COLOR Y TAMANYO
	if (estado->getEstadoCoche() == EstadosJugador::estado_coche::CON_TURBO) {
		p->setSize(0.75f); //Si el coche esta con turbo, el tamanyo es mas grande
		p->setColor(glm::vec3(1.0f, 1.0f, 0.0f)); //Se le pasa un color amarillo
	}
	else {
		if (estado->getEstadoMovimiento() == EstadosJugador::estado_movimiento::AVANZA) {
			p->setSize(0.25f); //Si el coche acelera, el tamanyo es mas grande
		}
		else {
			p->setSize(0.075f); //Si esta parado, el tamanyo es mas pequeño
		}
		p->setColor(glm::vec3(1.0f, 1.0f, 1.0f)); //Se le pasa un color blanco
	}
}

std::string Corredor::toString() {
	std::string resultado = "";

	resultado += "\nJugador " + to_string(this->getID());
	resultado += "\nVELOCIDAD: " + to_string((int)vehiculo->getCurrentSpeedKmHour()) + " km/h";
	if (direccionContraria != 0) {
		resultado += "\nVAS EN DIRECCION CONTRARIA, JUGADOR: " + to_string(this->getID()) + "\n";
	}

	resultado += "\nPOSICION: " + to_string(posicionCarrera);
	resultado += "\nVUELTAS: " + to_string(vueltas);
	if (vueltas > maxvueltas)
		resultado += "HA LLEGADO " + to_string(posicionCarrera) + "!\n";

	if (tiempoVuelta != 0)
		resultado += "\nTIEMPO DE VUELTA: " + to_string(tiempoVuelta);

	resultado += "\nOBJETO: " + to_string(EstadosJugador::estado_objeto(tipoObj));
	resultado += "\nHabilidad: " + to_string(limite) + "/100\n";



	return resultado;
}

void Corredor::updateTimerObstaculos() {
	if (aceiteActivado) {
		Timer *t2 = Timer::getInstancia();
		if (t2->getTimer() - timerAceite >= 0) {
			lanzarItemTeledirigido();
		}
		if (t2->getTimer() - timerAceite >= 1) {
			aceiteActivado = false;
		}
	}

}

void Corredor::actualizarRuedas() {
	float altura = 0.5;
	btVector3 zeroVector(0, 0, 0);
	glm::vec3 axis(0, 0, 0);
	float angle = 0;

	btTransform ruedas = vehiculo->getWheelTransformWS(0);

	//rueda1

	CuerpoColisionRueda1->setCenterOfMassTransform(ruedas);
	rueda1->setPosition(ruedas.getOrigin().getX(), ruedas.getOrigin().getY() + altura, ruedas.getOrigin().getZ());
	axis = glm::vec3(ruedas.getRotation().getAxis().getX(), ruedas.getRotation().getAxis().getY(), ruedas.getRotation().getAxis().getZ());
	angle = ruedas.getRotation().getAngle() * RADTODEG;
	rueda1->setRotation(axis, angle);
	//rueda1->setRotation(180,0,0);
	//rueda2
	ruedas = vehiculo->getWheelTransformWS(1);


	CuerpoColisionRueda2->setCenterOfMassTransform(ruedas);
	rueda2->setPosition(ruedas.getOrigin().getX(), ruedas.getOrigin().getY() + altura, ruedas.getOrigin().getZ());
	axis = glm::vec3(ruedas.getRotation().getAxis().getX(), ruedas.getRotation().getAxis().getY(), ruedas.getRotation().getAxis().getZ());
	angle = ruedas.getRotation().getAngle() * RADTODEG;
	rueda2->setRotation(axis, angle);


	//rueda3
	ruedas = vehiculo->getWheelTransformWS(2);


	//CuerpoColisionRueda4->setAngularVelocity(zeroVector);
	CuerpoColisionRueda4->setCenterOfMassTransform(ruedas);
	rueda4->setPosition(ruedas.getOrigin().getX(), ruedas.getOrigin().getY() + altura, ruedas.getOrigin().getZ());
	axis = glm::vec3(ruedas.getRotation().getAxis().getX(), ruedas.getRotation().getAxis().getY(), ruedas.getRotation().getAxis().getZ());
	angle = ruedas.getRotation().getAngle() * RADTODEG;
	rueda4->setRotation(axis, angle);


	//rueda4
	ruedas = vehiculo->getWheelTransformWS(3);


	//CuerpoColisionRueda3->setAngularVelocity(zeroVector);
	CuerpoColisionRueda3->setCenterOfMassTransform(ruedas);
	rueda3->setPosition(ruedas.getOrigin().getX(), ruedas.getOrigin().getY() + altura, ruedas.getOrigin().getZ());
	axis = glm::vec3(ruedas.getRotation().getAxis().getX(), ruedas.getRotation().getAxis().getY(), ruedas.getRotation().getAxis().getZ());
	angle = ruedas.getRotation().getAngle() * RADTODEG;
	rueda3->setRotation(axis, angle);


	if (estado->getEstadoMovimiento() != EstadosJugador::estado_movimiento::DERRAPA) {
		CuerpoColisionRueda1->clearForces();
		CuerpoColisionRueda1->setLinearVelocity(zeroVector);
		CuerpoColisionRueda2->clearForces();
		CuerpoColisionRueda2->setLinearVelocity(zeroVector);
		CuerpoColisionRueda3->clearForces();
		CuerpoColisionRueda3->setLinearVelocity(zeroVector);
		CuerpoColisionRueda4->clearForces();
		CuerpoColisionRueda4->setLinearVelocity(zeroVector);
	}

}

void Corredor::lanzarHabilidad() {
	if (getLimite() >= 100) {//puedo lanzar la habilidad
		Pista *pista = Pista::getInstancia();
		std::vector<Item *> items = pista->getItems();

		btVector3 posicion(cuboNodo->getPosition().x + orientacion.getX() * 20, cuboNodo->getPosition().y, cuboNodo->getPosition().z + orientacion.getZ() * 20);
		btVector3 escala(5, 5, 5);
		btScalar masa = 100;
		float tiempoDestruccion = 25;
		btVector3 tamanyoNodo(3, 3, 3);
		btScalar radio = 12;
		float alt = 10;

		Habilidad * habilidadJugador = nullptr;
		switch (tipojugador) {

		case PIRATA:

			masa = 100;
			radio = 12;
			alt = 10;

			posicion.setY(posicion.getY() + alt);
			habilidadJugador = new Habilidad(1, CuerpoColisionChasis, posicion, escala, masa, tiempoDestruccion, ESFERA, tamanyoNodo, radio, alt, cuboNodo->getID());
			habilidadJugador->lanzarItem(1, orientacion, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
			habilidadJugador->setLanzado(true);

			break;

		case VIKINGO:
			escala = btVector3(3, 3, 3);
			masa = 0;
			radio = 8;
			alt = 2;

			posicion.setY(posicion.getY() + alt);
			habilidadJugador = new Habilidad(2, CuerpoColisionChasis, posicion, escala, masa, tiempoDestruccion, ESFERA, tamanyoNodo, radio, alt, cuboNodo->getID());
			habilidadJugador->lanzarItem(1, orientacion, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
			habilidadJugador->setLanzado(true);
			break;

		case GLADIADOR:
			escala = btVector3(30, 2, 30);
			tiempoDestruccion = 3;
			habilidadJugador = new Habilidad(3, CuerpoColisionChasis, posicion, escala, masa, tiempoDestruccion, CILINDRO, tamanyoNodo, radio, alt, cuboNodo->getID());
			habilidadJugador->lanzarItem(1, orientacion, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
			habilidadJugador->setLanzado(true);
			break;

		case CHINO:
			posicion = btVector3(cuboNodo->getPosition().x, cuboNodo->getPosition().y, cuboNodo->getPosition().z);
			tamanyoNodo = btVector3(5, 5, 5);
			escala = btVector3(5, 7, 10);
			masa = 100;
			alt = 2;

			posicion.setY(posicion.getY() + alt);
			habilidadJugador = new Habilidad(4, CuerpoColisionChasis, posicion, escala, masa, tiempoDestruccion, CUBO, tamanyoNodo, radio, alt, cuboNodo->getID());
			habilidadJugador->lanzarItem(1, orientacion, CuerpoColisionChasis->getCenterOfMassTransform());// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
			habilidadJugador->setLanzado(true);
			setTurbo(true, true, FuerzaMaxima * 2, tiempoDestruccion / 3.6);
			break;
		}
		setHabilidad(true);
		items.push_back(habilidadJugador);

		pista->setItems(items);
		setLimite(0);
	}
	//else cout << "No puedes usar la habilidad si tu limite no es 100 o mas" << endl;

}

void Corredor::updateEstado() {
	if (vehiculo->getCurrentSpeedKmHour() < 0.5 && vehiculo->getCurrentSpeedKmHour() > -0.5)
		estado->setEstadoMovimiento(EstadosJugador::estado_movimiento::QUIETO);

	estado->setEstadoCoche(EstadosJugador::estado_coche::POR_DEFECTO);

	if (proteccion == true)
		estado->setEstadoCoche(EstadosJugador::estado_coche::CON_ESCUDO);

	if (turboActivado == true)
		estado->setEstadoCoche(EstadosJugador::estado_coche::CON_TURBO);

	//Objetos:
	estado->setEstadoObjeto(tipoObj);
	estado->update();
}

void Corredor::updateVectorDireccion() {
	/* Actualiza el vector direccion del corredor. */
	btQuaternion quaternion = CuerpoColisionChasis->getOrientation();
	//cameraThird->setRotation(quaternion.getX(), quaternion.getY(), quaternion.getZ());
	float anguloZ = quaternion.getAngle();
	float anguloX = cuboNodo->getRotation().y * PI / 180;
	//cout << " Orientacion Corredor(1378)" << endl;
	//cout << "Angulo X: " << cuboNodo->getRotation().x << endl;
	//cout << "Angulo Y: " << cuboNodo->getRotation().y << endl;
	//cout << "Angulo Y: " << cuboNodo->getRotation().z << endl;
	//cout << "--------------------------------------------------" << endl;
	orientacion = btVector3(sin(anguloX), 0, cos(anguloZ));

	orientacion.normalize();
	//cout<< "ORIENTACION XNORMAL COCHE=="<< orientacion.getX() << " ORIENTACION ZNORMAL COCHE=="<< orientacion.getZ()  << endl;
}


//------------------------------------------------------------------//
//---------------------ANIMACION------------------------------------//
//------------------------------------------------------------------//
TNodo *Corredor::getGiroDerIni() { return giroDerIni; }
TNodo *Corredor::getGiroDerFin() { return giroDerFin; }
TNodo *Corredor::getGiroIzqIni() { return giroIzqIni; }
TNodo *Corredor::getGiroIzqFin() { return giroIzqFin; }
TNodo *Corredor::getAnimQuieto() {
	static_cast<TMalla*>(quieto->getEntidad())->setVisible(true);
	return quieto;
}

/*
TNodo *Corredor::getLanzarObjeto() { return lanzarObjeto; }
TNodo *Corredor::getVacile() { return vacile; }
TNodo *Corredor::getHabilidadAnim() { return animHabilidad; }
*/
/*Metodo para cambiar entre animaciones cuando se pulsan los botones
mov = ESTADO DE LA ANIMACION
Previo = ANIMACION ANTERIOR A LA SOLICITADA, se parara y se deja de dibujar
previoGIRO =  animacion solo para los giros, en caso de existir las para. (necesario para controlar la animacion de los giros)
*/
void Corredor::iniciarAnimacion(movimiento_jugador mov, TNodo* previo, TNodo *previoGiro) {
	if (previo != NULL) {
		static_cast<TAnimacion*>(previo->getEntidad())->setVisible(false);
		static_cast<TAnimacion*>(previo->getEntidad())->ResetAnimation();
	}

	if (previoGiro != NULL) {
		static_cast<TAnimacion*>(previoGiro->getEntidad())->setVisible(false);
		static_cast<TAnimacion*>(previoGiro->getEntidad())->ResetAnimation();
	}

	if (previo == NULL) {
		static_cast<TMalla*>(cuboNodo->getNode()->getEntidad())->setVisible(false);
	}
	/*	if (mov != movimiento_jugador::PARADO) {
			static_cast<TMalla*>(quieto->getEntidad())->setVisible(false);
		}*/


	switch (mov) {
		/*case movimiento_jugador::PARADO:
			cuboNodo->setNode(quieto);
			break;*/
	case movimiento_jugador::GIRODERINI:
		cuboNodo->setNode(giroDerIni);
		break;
	case movimiento_jugador::GIRODERFIN:
		cuboNodo->setNode(giroDerFin);
		break;
	case movimiento_jugador::GIROIZQINI:
		cuboNodo->setNode(giroIzqIni);
		break;
	case movimiento_jugador::GIROIZQFIN:
		cuboNodo->setNode(giroIzqFin);
		break;
		/*case movimiento_jugador::HABILIDAD:
			cuboNodo->setNode(animHabilidad);
			break;

		case movimiento_jugador::LANZAROBJETO:
			cuboNodo->setNode(lanzarObjeto);
			break;
		case movimiento_jugador::VACILE:
				cuboNodo->setNode(vacile);
				break;
			*/
	}
	static_cast<TAnimacion*>(cuboNodo->getNode()->getEntidad())->setVisible(true);
	static_cast<TAnimacion*>(cuboNodo->getNode()->getEntidad())->setPlaying(true);
}

//---------------------------------------//
//--------------DESTRUCTOR---------------//
//---------------------------------------//
Corredor::~Corredor() {
	// Modelos 3D
	delete cuboNodo;
	delete rueda1;
	delete rueda2;
	delete rueda3;
	delete rueda4;

	// Animaciones
	giroDerIni = nullptr;
	giroDerFin = nullptr;
	giroIzqIni = nullptr;
	giroIzqFin = nullptr;
	quieto = nullptr;

	// Sonido
	delete fuenteMotor;
	delete fuenteItem;

	// Fisicas
	MotorFisicas::getInstancia()->getMundo()->removeCollisionObject(CuerpoColisionRueda1);
	MotorFisicas::getInstancia()->getMundo()->removeCollisionObject(CuerpoColisionRueda2);
	MotorFisicas::getInstancia()->getMundo()->removeCollisionObject(CuerpoColisionRueda3);
	MotorFisicas::getInstancia()->getMundo()->removeCollisionObject(CuerpoColisionRueda4);
	MotorFisicas::getInstancia()->getMundo()->removeCollisionObject(vehiculo->getRigidBody());

	
	delete vehiculo->getRigidBody()->getCollisionShape();
	delete CuerpoColisionRueda4->getCollisionShape();
	delete CuerpoColisionRueda3->getCollisionShape();
	delete CuerpoColisionRueda2->getCollisionShape();
	delete CuerpoColisionRueda1->getCollisionShape();
	
	delete vehiculo->getRigidBody()->getMotionState();
	delete CuerpoColisionRueda4->getMotionState();
	delete CuerpoColisionRueda3->getMotionState();
	delete CuerpoColisionRueda2->getMotionState();
	delete CuerpoColisionRueda1->getMotionState();
	

	delete vehiculo;
	delete RayCastVehiculo;
	// Se borrarn los cuerpos de colision en el destructor del motor de fisicas
	//	delete CuerpoColisionRueda4;
	//	delete CuerpoColisionRueda3;
	//	delete CuerpoColisionRueda2;
	//	delete CuerpoColisionRueda1;

	

	nombre = nullptr;
	delete estado;

	//BILLBOARD
	b = nullptr;

	//PARTICULAS HUMO
	p = nullptr;

	actual = nullptr;
	siguiente = nullptr;
	siguiente_aux = nullptr;
	//delete actual; // Punto Actual
	//delete siguiente; // Punto Siguiente
	//delete siguiente_aux; // Punto Siguiente
	/*	delete vacile;
		delete giroDerIni;
		delete lanzarObjeto;
		delete giroDerFin;
		delete giroIzqIni;
		delete giroIzqFin;
		delete animHabilidad;
		delete quieto;*/
}
