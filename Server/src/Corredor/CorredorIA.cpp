#include "CorredorIA.hpp"
#include "Timer.hpp"
#include "MotorFisicas.hpp"
#include "math.h"
#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  
#endif

CorredorIA::CorredorIA(btVector3 pos, Corredor::tipo_jugador tipo) : Corredor(pos, tipo){
	nombre = "JugadorIA";
	cuboNodo->setName(nombre);
	caja = false;
	enemigo = false;
	turbo = false;
	Vision = false;
	Objeto = false;
	distanciaCerca = false;
	distanciaMedia = false;
	distanciaLejos = false;
	velocidadBaja = false;
	velocidadMedia = false;
	velocidadAlta = false;
	giroFuerteDerecha = false;
	giroFlojoDerecha = false;
	noGiro = false;
	giroFuerteIzquierda = false;
	giroFlojoIzquierda = false;
	enemigoatras = false;
	enemigolados = false;
	habilidad = false;
	debugFisicas = false;
	distanciaCaja = 0;
	distanciaEnemigo = 0;
	distanciaTurbo = 0;
	timerRecolocar = 0;
	pertenenciaCerca = 0;
	pertenenciaMedia = 0;
	pertenenciaLejos = 0;
	pertenenciaGiroFuerteDerecha = 0;
	pertenenciaGiroFlojoDerecha = 0;
	pertenenciaNoGiro = 0;
	pertenenciaGiroFuerteIzquierda = 0;
	pertenenciaGiroFlojoIzquierda = 0;
	pertenenciaVelocidadBaja = 0;
	pertenenciaVelocidadMedia = 0;
	pertenenciaVelocidadAlta = 0;
	valorGiroFuerte = 0.18;

	//control de animaciones
	animacionPrevia = getAnimQuieto();
	iniciarAnimacion(VACILE, animacionPrevia,NULL);
	animacionPrevia = getVacile();
	giroDer = false;
	giroIzq = false;


	arbolconduccion = new ArbolDecision();
	arbolconduccion->nuevoNodoDecision(0, 0, NADA2, NADA3, 0, false); // 0 NODO PADRE

	//NodosHijos
	arbolconduccion->nuevoNodoDecision(0, 1, IGUAL, GIROFUERTEDERECHA, 0, true); // 1 GiroFuertederecha
	arbolconduccion->nuevoNodoDecision(0, 2, IGUAL, GIROMEDIODERECHA, 0, true); // 2 GiroDerecha
	arbolconduccion->nuevoNodoDecision(0, 3, IGUAL, NOGIRO, 0, true); // 3 NoGiro
	arbolconduccion->nuevoNodoDecision(0, 4, IGUAL, GIROMEDIOIZQUIERDA, 0, true); // 4 GiroIzquierda
	arbolconduccion->nuevoNodoDecision(0, 5, IGUAL, GIROFUERTEIZQUIERDA, 0, true);  // 5 GiroFuerteIzquierda

	//GiroFuertederecha 1
	arbolconduccion->nuevoNodoDecision(1, 6, IGUAL, VELOCIDADBAJA, 0, true); // 6
	arbolconduccion->nuevoNodoDecision(1, 7, IGUAL, VELOCIDADMEDIA, 0, true); // 7
	arbolconduccion->nuevoNodoDecision(1, 8, IGUAL, VELOCIDADALTA, 0, true); // 8

	arbolconduccion->nuevoNodoAccion(6, 9, 5); //9
	arbolconduccion->nuevoNodoAccion(7, 10, 5); //10
	arbolconduccion->nuevoNodoAccion(8, 11, 7); //11

	//GiroDerecha 2
	arbolconduccion->nuevoNodoAccion(2, 12, 3); // 12
	//NoGiro 3
	arbolconduccion->nuevoNodoAccion(3, 13, 1); // 13
	//GiroIzquierda 4
	arbolconduccion->nuevoNodoAccion(4, 14, 4); // 14

	//GiroFuerteIzquierda 5
	arbolconduccion->nuevoNodoDecision(5, 15, IGUAL, VELOCIDADBAJA, 0, true); // 15
	arbolconduccion->nuevoNodoDecision(5, 16, IGUAL, VELOCIDADMEDIA, 0, true); // 16
	arbolconduccion->nuevoNodoDecision(5, 17, IGUAL, VELOCIDADALTA, 0, true); // 17

	arbolconduccion->nuevoNodoAccion(15, 18, 6); //18
	arbolconduccion->nuevoNodoAccion(16, 19, 6); //19
	arbolconduccion->nuevoNodoAccion(17, 20, 8); //20


	//numero de nodos 80
	arbolobjetos = new ArbolDecision();
	arbolobjetos->nuevoNodoDecision(0, 0, NADA2, NADA3, 0, false); // 0 NODO PADRE


	arbolobjetos->nuevoNodoDecision(0, 59, IGUAL, HABILIDADD, 0, true);
	arbolobjetos->nuevoNodoDecision(0, 60, IGUAL, HABILIDADD, 0, false);

	arbolobjetos->nuevoNodoDecision(59, 61, IGUAL, JUGADOR, 0, true); //GLADIADOR
	arbolobjetos->nuevoNodoDecision(59, 62, IGUAL, JUGADOR, 1, true); //PIRATA
	arbolobjetos->nuevoNodoDecision(59, 63, IGUAL, JUGADOR, 2, true); //VIKINGO	
	arbolobjetos->nuevoNodoDecision(59, 64, IGUAL, JUGADOR, 3, true); //CHINO

	arbolobjetos->nuevoNodoAccion(64, 65, 14);

	arbolobjetos->nuevoNodoDecision(62, 66, IGUAL, ENEMIGO, 0, true);
	arbolobjetos->nuevoNodoDecision(62, 72, IGUAL, ENEMIGO, 0, false);
	arbolobjetos->nuevoNodoDecision(66, 67, IGUAL, NOGIRO, 0, true);
	arbolobjetos->nuevoNodoDecision(66, 68, IGUAL, NOGIRO, 0, false);
	arbolobjetos->nuevoNodoAccion(67, 69, 14);
	arbolobjetos->nuevoNodoAccion(68, 70, 10);
	arbolobjetos->nuevoNodoSalto(72, 73, 60);

	arbolobjetos->nuevoNodoDecision(61, 71, IGUAL, ENEMIGOLADO, 0, true);
	arbolobjetos->nuevoNodoDecision(61, 74, IGUAL, ENEMIGOLADO, 0, false);
	arbolobjetos->nuevoNodoAccion(71, 75, 14);
	arbolobjetos->nuevoNodoSalto(74, 76, 60);

	arbolobjetos->nuevoNodoDecision(63, 77, IGUAL, ENEMIGOLADO, 0, true);
	arbolobjetos->nuevoNodoDecision(63, 78, IGUAL, ENEMIGOLADO, 0, false);
	arbolobjetos->nuevoNodoAccion(77, 79, 14);
	arbolobjetos->nuevoNodoSalto(78, 80, 60);



	//NodosHijos
	arbolobjetos->nuevoNodoDecision(60, 1, IGUAL, OBJBOOLEANO, 0, true); // 1 Objeto
	arbolobjetos->nuevoNodoDecision(60, 2, IGUAL, OBJBOOLEANO, 0, false); // 2 !Objeto

	//Objeto 1
	arbolobjetos->nuevoNodoDecision(1, 3, IGUAL, VISION, 0, true); // 3 Vision
	arbolobjetos->nuevoNodoDecision(1, 4, IGUAL, VISION, 0, false); // 4 !Vision

	//Vision 3
	arbolobjetos->nuevoNodoDecision(3, 5, IGUAL, ENEMIGO, 0, true); // 5 Enemigo
	arbolobjetos->nuevoNodoDecision(3, 6, IGUAL, TURBOO, 0, true); // 6 Turbo

	//Enemigo 5
	arbolobjetos->nuevoNodoDecision(5, 7, IGUAL, DISTANCIACERCA, 0, true); // 7 CERCA
	arbolobjetos->nuevoNodoDecision(5, 8, IGUAL, DISTANCIAMEDIA, 0, true); // 8 MEDIO
	arbolobjetos->nuevoNodoDecision(5, 9, IGUAL, DISTANCIALEJOS, 0, true); // 9 LEJOS

	//CERCA 7
	arbolobjetos->nuevoNodoDecision(7, 10, IGUAL, OBJNUMERO, 1, false); // 10 OBJ1
	arbolobjetos->nuevoNodoDecision(7, 11, IGUAL, OBJNUMERO, 6, false); // 11 OBJ6

	//OBJ1 10
	arbolobjetos->nuevoNodoDecision(10, 12, IGUAL, NOGIRO, 0, true); // 12 NOGIRO
	arbolobjetos->nuevoNodoDecision(10, 13, IGUAL, NOGIRO, 0, false);// 13 !NOGIRO

	//NOGIRO 12
	arbolobjetos->nuevoNodoAccion(12, 14, 13); // 14 Lanzo el objeto accion a concretar
	//!NOGIRO 13
	arbolobjetos->nuevoNodoAccion(13, 15, 10); // 15 Sigo al objetivo accion a concretar

	//OBJ6 11
	arbolobjetos->nuevoNodoDecision(11, 16, IGUAL, NOGIRO, 0, true); // 16 NOGIRO
	arbolobjetos->nuevoNodoDecision(11, 17, IGUAL, NOGIRO, 0, false);// 17 !NOGIRO

	//NOGIRO 16
	arbolobjetos->nuevoNodoAccion(16, 18, 13); // 18 Lanzo el objeto accion a concretar
	//!NOGIRO 17
	arbolobjetos->nuevoNodoAccion(17, 19, 10); // 19 Sigo al objetivo accion a concretar

	//MEDIO 8
	arbolobjetos->nuevoNodoDecision(8, 20, IGUAL, OBJNUMERO, 1, false); // 20 OBJ1
	arbolobjetos->nuevoNodoDecision(8, 21, IGUAL, OBJNUMERO, 6, false); // 21 OBJ6

	//OBJ1 20
	arbolobjetos->nuevoNodoDecision(20, 22, IGUAL, NOGIRO, 0, true); // 22 NOGIRO
	arbolobjetos->nuevoNodoDecision(20, 23, IGUAL, NOGIRO, 0, false);// 23 !NOGIRO

	//NOGIRO 22
	arbolobjetos->nuevoNodoAccion(22, 24, 13); // 24 Lanzo el objeto accion a concretar
	//!NOGIRO 23
	arbolobjetos->nuevoNodoAccion(23, 25, 10); // 25 Sigo al objetivo accion a concretar

	//OBJ6 21
	arbolobjetos->nuevoNodoDecision(21, 26, IGUAL, NOGIRO, 0, true); // 26 NOGIRO
	arbolobjetos->nuevoNodoDecision(21, 27, IGUAL, NOGIRO, 0, false);// 27 !NOGIRO

	//NOGIRO 26
	arbolobjetos->nuevoNodoAccion(26, 28, 13); // 28 Lanzo el objeto accion a concretar
	//!NOGIRO 27
	arbolobjetos->nuevoNodoAccion(27, 29, 10); // 29 Sigo al objetivo accion a concretar

	//LEJOS 9
	arbolobjetos->nuevoNodoAccion(9, 30, 12); // 30 Sigo al waypoint a concretar


	//TURBO 6
	arbolobjetos->nuevoNodoDecision(6, 31, IGUAL, DISTANCIACERCA, 0, true); // 31 CERCA
	arbolobjetos->nuevoNodoDecision(6, 32, IGUAL, DISTANCIAMEDIA, 0, true); // 32 MEDIO
	arbolobjetos->nuevoNodoDecision(6, 33, IGUAL, DISTANCIALEJOS, 0, true); // 33 LEJOS

	//CERCA 31
	arbolobjetos->nuevoNodoAccion(31, 34, 11); // 34 Sigo al TURBO a concretar
	//MEDIO 32
	arbolobjetos->nuevoNodoAccion(32, 35, 11); // 35 Sigo al TURBO a concretar
	//LEJOS 33
	arbolobjetos->nuevoNodoAccion(33, 36, 12); // 36 Sigo al waypoint a concretar


	//!VISION 4
	arbolobjetos->nuevoNodoDecision(4, 37, IGUAL, OBJNUMERO, 1, true); // 37 OBJ1


	//!Objeto 2
	arbolobjetos->nuevoNodoDecision(2, 38, IGUAL, VISION, 0, true); // 38 Vision
	arbolobjetos->nuevoNodoDecision(2, 39, IGUAL, VISION, 0, false); // 39 !Vision

	//Vision 38
	arbolobjetos->nuevoNodoDecision(38, 40, IGUAL, CAJA, 0, true); // 40 Caja
	arbolobjetos->nuevoNodoDecision(38, 41, IGUAL, TURBOO, 0, true); // 41 Turbo

	//Caja 40
	arbolobjetos->nuevoNodoDecision(40, 42, IGUAL, DISTANCIACERCA, 0, true); // 42 CERCA
	arbolobjetos->nuevoNodoDecision(40, 43, IGUAL, DISTANCIAMEDIA, 0, true); // 43 MEDIO
	arbolobjetos->nuevoNodoDecision(40, 44, IGUAL, DISTANCIALEJOS, 0, true); // 44 LEJOS

	//CERCA 42
	arbolobjetos->nuevoNodoAccion(42, 45, 9); // 45 Sigo caja a concretar

	//MEDIO 43
	arbolobjetos->nuevoNodoAccion(43, 46, 9); // 46 Sigo caja a concretar

	//LEJOS 44
	arbolobjetos->nuevoNodoAccion(44, 47, 12); // 47 Sigo WAYPOINT a concretar

	//Turbo 41	
	arbolobjetos->nuevoNodoDecision(41, 48, IGUAL, DISTANCIACERCA, 0, true); // 48 CERCA
	arbolobjetos->nuevoNodoDecision(41, 49, IGUAL, DISTANCIAMEDIA, 0, true); // 49 MEDIO
	arbolobjetos->nuevoNodoDecision(41, 50, IGUAL, DISTANCIALEJOS, 0, true); // 50 LEJOS

	//CERCA 48
	arbolobjetos->nuevoNodoAccion(48, 51, 11); // 51 Sigo TURBO a concretar

	//MEDIO 49
	arbolobjetos->nuevoNodoAccion(49, 52, 11); // 52 Sigo TURBO a concretar

	//LEJOS 50
	arbolobjetos->nuevoNodoAccion(50, 53, 12); // 53 Sigo WAYPOINT a concretar

	//!VISION 39
	arbolobjetos->nuevoNodoAccion(39, 54, 12); // 54 Sigo WAYPOINT a concretar


	arbolobjetos->nuevoNodoDecision(4, 55, IGUAL, OBJNUMERO, 6, true); // 55 OBJ6
	arbolobjetos->nuevoNodoAccion(4, 56, 13); // 56 Lanzo el objeto accion a concretar


	arbolobjetos->nuevoNodoAccion(37, 57, 12); //57
	arbolobjetos->nuevoNodoAccion(55, 58, 12); //58

}

//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
void CorredorIA::movimiento()
{


	int accion = 0;
	bool arraybooleanos[18] = {
	distanciaCerca,distanciaMedia,distanciaLejos, // del 0 al 2 distancias
	velocidadBaja,velocidadMedia,velocidadAlta, // del 3 al 5 velocidades
	noGiro,giroFlojoDerecha,giroFlojoIzquierda,giroFuerteDerecha,giroFuerteIzquierda, // del 6 al 10 conduccion
	caja,turbo,enemigo,Vision,Objeto,enemigolados,habilidad }; // del 11 al 15 vision 



	accion = arbolobjetos->recorrerArbol(arraybooleanos, tipoObj, tipojugador);
	accionActualObjetos = accion;
	switch (accion) { //lo que devuelva el arbol conduccion

	case NADA1:
		usarObjetos();
		break;

	case DISTANCIAOBJETIVOCAJA: //9
		calculoAnguloGiro(posicionCaja);

		break;

	case DISTANCIAOBJETIVOENEMIGO: //10
		calculoAnguloGiro(posicionEnemigo);

		break;

	case DISTANCIAOBJETIVOTURBO://11
		calculoAnguloGiro(posicionTurbo);

		break;

	case SEGUIRWAYPOINT://12
		seguirWaypoint();

		break;

	case USAROBJETO://13

		usarObjetos();
		if (proteccion)
			seguirWaypoint();

		break;

	case USARHABILIDAD: //14
		lanzarHabilidad();
		limite = 0;
		break;

	case ACELERAR: //1
		acelerar();
		break;

	}

	accion = arbolconduccion->recorrerArbol(arraybooleanos, tipoObj, tipojugador);
	accionActualConduccion = accion;
	switch (accion) { //lo que devuelva el arbol conduccion

	case NADA1:
		acelerar();
		//finAnimacion();
		break;

	case ACELERAR: //1
		acelerar();
		vehiculo->setSteeringValue(0, 0);
		vehiculo->setSteeringValue(0, 1);
		//finAnimacion();
		break;

	case FRENAR: //2
		frenar();
		//finAnimacion();
		break;

	case ACELERARGIRARDERECHA://3
		acelerar();
		girarIzquierda();
		animacionIzquierda();
		break;

	case ACELERARGIRARIZQUIERDA://4
		acelerar();
		girarDerecha();
		animacionDerecha();
		break;

	case ACELERARGIRARFUERTEDERECHA://5
		acelerar();
		vehiculo->setSteeringValue(valorGiroFuerte, 0);
		vehiculo->setSteeringValue(valorGiroFuerte, 1);
		animacionIzquierda();
		break;

	case ACELERARGIRARFUERTEIZQUIERDA://6
		acelerar();
		vehiculo->setSteeringValue(-valorGiroFuerte, 0);
		vehiculo->setSteeringValue(-valorGiroFuerte, 1);
		animacionDerecha();
		break;

	case FRENARGIRARFUERTEDERECHA://7
		desacelerar();
		vehiculo->setSteeringValue(valorGiroFuerte, 0);
		vehiculo->setSteeringValue(valorGiroFuerte, 1);
		animacionIzquierda();
		break;

	case FRENARGIRARFUERTEIZQUIERDA://8
		desacelerar();
		vehiculo->setSteeringValue(-valorGiroFuerte, 0);
		vehiculo->setSteeringValue(-valorGiroFuerte, 1);
		animacionDerecha();
		break;
	}
	finAnimacion();
	reposicionar();
}

void CorredorIA::seguirWaypoint() {

	btScalar distanciaCentro = btScalar(0);
	btScalar distanciaLado1 = btScalar(0);
	btScalar distanciaLado2 = btScalar(0);
	btScalar distanciaLado3 = btScalar(0);
	btScalar distanciaLado4 = btScalar(0);
	int valor;	
	//vector 1 y 4 son los puntos de los extremos
	btVector3 posCoche(cuboNodo->getPosition().x, cuboNodo->getPosition().y, cuboNodo->getPosition().z);
	distanciaCentro = getDistanciaPunto(siguiente->getPosicion());
	distanciaLado1 = getDistanciaPunto(siguiente->getVector1());
	distanciaLado2 = getDistanciaPunto(siguiente->getVector2());
	distanciaLado3 = getDistanciaPunto(siguiente->getVector3());
	distanciaLado4 = getDistanciaPunto(siguiente->getVector4());
	

	
	if (distanciaCentro < distanciaLado1 && distanciaCentro < distanciaLado2 && distanciaCentro < distanciaLado3 && distanciaCentro < distanciaLado4) {
		//if(posCoche.distance(siguiente->getPosicion())>50)
		if(!enemigo)
		valor=0;
		else{
			int random = rand() % 100 + 1;
			if(random>50)
			valor=1;
			else
			valor=2;
		}
	}
	
		/*
		if (distanciaLado1 < distanciaCentro && distanciaLado1 < distanciaLado2 && distanciaLado1 < distanciaLado3 && distanciaLado1 < distanciaLado4) {
			calculoAnguloGiro(siguiente->getVector1());
		}*/
		if (distanciaLado2 < distanciaCentro /*&& distanciaLado2 < distanciaLado1*/ && distanciaLado2 < distanciaLado3 /*&& distanciaLado2 < distanciaLado4*/) {
			//if(posCoche.distance(siguiente->getVector2())>50)	
			valor =1;
		}
		if (distanciaLado3 < distanciaCentro && distanciaLado3 < distanciaLado1 /*&& distanciaLado3 < distanciaLado2 && distanciaLado3 < distanciaLado4*/) {
			//if(posCoche.distance(siguiente->getVector3())>50)
			valor=2;
		}/*
		if (distanciaLado4 < distanciaCentro && distanciaLado4 < distanciaLado1 && distanciaLado4 < distanciaLado3 && distanciaLado4 < distanciaLado2) {
			calculoAnguloGiro(siguiente->getVector4());

		}*/
	switch(valor){

		case 0:
		if(posCoche.distance(siguiente->getPosicion())>20)
		calculoAnguloGiro(siguiente->getPosicion());
			
		//cout<<posCoche.distance(siguiente->getPosicion())<<endl;
		break;
		case 1:
		if(posCoche.distance(siguiente->getVector2())>20)
		calculoAnguloGiro(siguiente->getVector2());

		//cout<<posCoche.distance(siguiente->getVector2())<<endl;
		break;
		case 2:
		if(posCoche.distance(siguiente->getVector3())>20)
		calculoAnguloGiro(siguiente->getVector3());

		//cout<<posCoche.distance(siguiente->getVector3())<<endl;
		break;

	}

}
void CorredorIA::reposicionar() {
	Timer *time = Timer::getInstancia();
	//cout<<"TIEMPO: "<<time->getTimer()-timerRecolocar<<endl;
	if (time->getTimer() - timerRecolocar == 1) {
		posicion_aux = btVector3(cuboNodo->getPosition().x, cuboNodo->getPosition().y, cuboNodo->getPosition().z);

	}
	if (time->getTimer() - timerRecolocar > 3) {
		if ((unsigned)((unsigned)(posicion_aux.getX()) - (unsigned)(cuboNodo->getPosition().x)) < 3 && (unsigned)((unsigned)(posicion_aux.getZ()) - (unsigned)(cuboNodo->getPosition().z)) < 3) {
			recolocarWaypoint();
			timerRecolocar = time->getTimer();
			//cout<<"recolocado"<<endl;
		}
		else {
			//cout<<"sigue"<<endl;
		}
	}
	if (time->getTimer() - timerRecolocar > 4) {
		timerRecolocar = time->getTimer();
	}
}

void CorredorIA::calculoAnguloGiro(btVector3 posicion) {
	double MPI = 3.14159265358979323846;
	btVector3 orientacionCoche(orientacion.getX(), orientacion.getY(), orientacion.getZ());
	btVector3 direccion = btVector3(posicion.getX() - cuboNodo->getPosition().x,
		posicion.getY() - cuboNodo->getPosition().y,
		posicion.getZ() - cuboNodo->getPosition().z);

	//direccion.normalize();
	anguloGiro = orientacionCoche.angle(direccion) * 180.0f / MPI;

	btVector3 orientacionCocheGirada = orientacionCoche.rotate(btVector3(0, 1, 0), 2 * MPI / 180);

	btScalar angulo2 = orientacionCocheGirada.angle(direccion) * 180 / MPI;

	if (angulo2 > anguloGiro)
		anguloGiro = -anguloGiro;

	//cout<<anguloGiro<<endl;

}


void CorredorIA::logicaDifusa() {




	//GIRO
	pertenenciaNoGiro = FuncionTriangular(anguloGiro, -35, 0, 35);
	pertenenciaGiroFlojoDerecha = FuncionTriangular(anguloGiro, 5, 30, 40);
	pertenenciaGiroFlojoIzquierda = FuncionTriangular(anguloGiro, -5, -30, -40);
	pertenenciaGiroFuerteDerecha = FuncionTriangular(anguloGiro, 40, 90, 180);
	pertenenciaGiroFuerteIzquierda = FuncionTriangular(anguloGiro, -40, -90, -180);

	//VELOCIDAD
	pertenenciaVelocidadBaja = FuncionTrapezoidal(vehiculo->getCurrentSpeedKmHour(), 0, 0, 50, 150);
	pertenenciaVelocidadMedia = FuncionTrapezoidal(vehiculo->getCurrentSpeedKmHour(), 100, 150, 200, 250);
	pertenenciaVelocidadAlta = FuncionTrapezoidal(vehiculo->getCurrentSpeedKmHour(), 200, 250, 300, 450);

	//DISTANCIA
	pertenenciaCerca = FuncionTrapezoidal(distanciaObjetivo, 0, 0, 1000, 3000);
	pertenenciaMedia = FuncionTrapezoidal(distanciaObjetivo, 2000, 4000, 5000, 6000);
	pertenenciaLejos = FuncionTrapezoidal(distanciaObjetivo, 5000, 8000, 9000, 100000);


	if (pertenenciaVelocidadBaja > pertenenciaVelocidadAlta && pertenenciaVelocidadBaja > pertenenciaVelocidadMedia) {
		velocidadBaja = true;
		velocidadAlta = false;
		velocidadMedia = false;

	}

	if (pertenenciaVelocidadMedia > pertenenciaVelocidadAlta && pertenenciaVelocidadBaja < pertenenciaVelocidadMedia) {
		velocidadBaja = false;
		velocidadAlta = false;
		velocidadMedia = true;

	}

	if (pertenenciaVelocidadBaja<pertenenciaVelocidadAlta && pertenenciaVelocidadAlta > pertenenciaVelocidadMedia) {
		velocidadBaja = false;
		velocidadAlta = true;
		velocidadMedia = false;

	}



	if (pertenenciaCerca > pertenenciaMedia && pertenenciaCerca > pertenenciaLejos) {
		distanciaCerca = true;
		distanciaLejos = false;
		distanciaMedia = false;
	}

	if (pertenenciaLejos > pertenenciaMedia && pertenenciaLejos > pertenenciaCerca) {
		distanciaCerca = false;
		distanciaLejos = true;
		distanciaMedia = false;

	}

	if (pertenenciaMedia > pertenenciaCerca && pertenenciaMedia > pertenenciaLejos) {
		distanciaCerca = false;
		distanciaLejos = false;
		distanciaMedia = true;

	}

	if (pertenenciaNoGiro > pertenenciaGiroFlojoDerecha && pertenenciaNoGiro > pertenenciaGiroFlojoIzquierda
		&&pertenenciaNoGiro > pertenenciaGiroFuerteDerecha && pertenenciaNoGiro > pertenenciaGiroFuerteIzquierda) {
		giroFuerteDerecha = false;
		giroFlojoDerecha = false;
		noGiro = true;
		giroFuerteIzquierda = false;
		giroFlojoIzquierda = false;
		//cout<<"NO"<<endl;
	}

	if (pertenenciaGiroFlojoDerecha > pertenenciaNoGiro && pertenenciaGiroFlojoDerecha > pertenenciaGiroFuerteDerecha
		&& pertenenciaGiroFlojoDerecha > pertenenciaGiroFlojoIzquierda && pertenenciaGiroFlojoDerecha > pertenenciaGiroFuerteIzquierda) {
		giroFuerteDerecha = false;
		giroFlojoDerecha = true;
		noGiro = false;
		giroFuerteIzquierda = false;
		giroFlojoIzquierda = false;
		//cout<<"GIROFlojo"<<endl;
	}
	if (pertenenciaGiroFuerteDerecha > pertenenciaNoGiro && pertenenciaGiroFuerteDerecha > pertenenciaGiroFlojoDerecha
		&& pertenenciaGiroFuerteDerecha > pertenenciaGiroFlojoIzquierda && pertenenciaGiroFuerteDerecha > pertenenciaGiroFuerteIzquierda) {
		giroFuerteDerecha = true;
		giroFlojoDerecha = false;
		noGiro = false;
		giroFuerteIzquierda = false;
		giroFlojoIzquierda = false;
		//cout<<"GIROFUERTE"<<endl;
	}

	if (pertenenciaGiroFlojoIzquierda > pertenenciaNoGiro &&  pertenenciaGiroFlojoIzquierda > pertenenciaGiroFuerteIzquierda
		&& pertenenciaGiroFlojoIzquierda > pertenenciaGiroFlojoDerecha && pertenenciaGiroFlojoIzquierda > pertenenciaGiroFuerteDerecha) {
		giroFuerteDerecha = false;
		giroFlojoDerecha = false;
		noGiro = false;
		giroFuerteIzquierda = false;
		giroFlojoIzquierda = true;
		//cout<<"GIROFlojo"<<endl;
	}
	if (pertenenciaGiroFuerteIzquierda > pertenenciaNoGiro && pertenenciaGiroFuerteIzquierda > pertenenciaGiroFlojoIzquierda
		&& pertenenciaGiroFuerteIzquierda > pertenenciaGiroFlojoDerecha && pertenenciaGiroFuerteIzquierda > pertenenciaGiroFuerteDerecha) {
		giroFuerteDerecha = false;
		giroFlojoDerecha = false;
		noGiro = false;
		giroFuerteIzquierda = true;
		giroFlojoIzquierda = false;
		//cout<<"GIROFUERTE"<<endl;
	}



}

double CorredorIA::FuncionTrapezoidal(double valor, double a, double b, double c, double d) {

	double resultado = 0;

	if (a <= valor && valor < b)
		resultado = (valor - a) / (b - a);
	else if (b <= valor && valor <= c)
		resultado = 1;
	else if (c < valor && valor <= d)
		resultado = (d - valor) / (d - c);

	return resultado;

}

double CorredorIA::FuncionTriangular(double valor, double a, double b, double c) {

	double resultado = 0;

	if (valor >= 0)
	{
		if (a <= valor && valor < b)
			resultado = (valor - a) / (b - a);
		else if (valor == b)
			resultado = 1;
		else if (b < valor && valor <= c)
			resultado = (c - valor) / (c - b);

	}
	else {

		if (a >= valor && valor > b)
			resultado = (valor - a) / (b - a);
		else if (valor == b)
			resultado = 1;
		else if (b > valor && valor >= c)
			resultado = (c - valor) / (c - b);

	}
	return resultado;


}

void CorredorIA::ActualizarRaytest() {
	double MPI = 3.14159265358979323846;
	MotorFisicas *mun = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = mun->getMundo();
	mundo->updateAabbs();
	mundo->computeOverlappingPairs();

	//cout<< orientacion.X << "   "<< orientacion.Z<<endl;
	float distanciaRaycast = 150; // longitud del rayo
	float distanciaRaycastLados = 50;
	float distanciaRaycastTraseros = 50;
	float distanciaCoche = 2; // distancia entre el rayo y el coche, donde empieza
	float Raycast23 = 5; // distancia entre raycast 2 y 3
	float Raycast45 = 10; // distancia entre raycast 4 y 5
	float Raycastlados = 10;
	float Raycastatras = 10;
	float RaycastladosY = 3;
	btVector3 fingirado(0, 0, 0);



	// Raycast central1
	btVector3 inicio(cuboNodo->getPosition().x + orientacion.getX()*distanciaCoche, cuboNodo->getPosition().y - 1, cuboNodo->getPosition().z + orientacion.getZ()*distanciaCoche);
	btVector3 fin(cuboNodo->getPosition().x + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().y - 1, cuboNodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);
	// Raycast central2 derecha
	//inicio = btVector3(3*orientacion.Z+cuboNodo->getPosition().x,cuboNodo->getPosition().y+1,orientacion.X*-3+cuboNodo->getPosition().z);
	inicio = btVector3(Raycast23*orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX(), cuboNodo->getPosition().y - 1, orientacion.getX()*-Raycast23 + cuboNodo->getPosition().z + orientacion.getZ());
	fin = btVector3(Raycast23*orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().y - 1, orientacion.getX()*-Raycast23 + cuboNodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast2(inicio, fin);
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast2);


	// Raycast central3 izquierda
	inicio = btVector3(-Raycast23 * orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX(), cuboNodo->getPosition().y - 1, orientacion.getX()*Raycast23 + cuboNodo->getPosition().z + orientacion.getZ());
	fin = btVector3(-Raycast23 * orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().y - 1, orientacion.getX()*Raycast23 + cuboNodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast3(inicio, fin);
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast3);


	// Raycast central4 derecha
	inicio = btVector3(Raycast45*orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX(), cuboNodo->getPosition().y - 1, orientacion.getX()*-Raycast45 + cuboNodo->getPosition().z + orientacion.getZ());
	fin = btVector3(Raycast45*orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().y - 1, orientacion.getX()*-Raycast45 + cuboNodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast4(inicio, fin);
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast4);

	// Raycast central5 izquierda
	inicio = btVector3(-Raycast45 * orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX(), cuboNodo->getPosition().y - 1, orientacion.getX()*Raycast45 + cuboNodo->getPosition().z + orientacion.getZ());
	fin = btVector3(-Raycast45 * orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().y - 1, orientacion.getX()*Raycast45 + cuboNodo->getPosition().z + orientacion.getZ() *distanciaRaycast);

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast5(inicio, fin);
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast5);

	// Raycast izquierda1
	inicio = btVector3(-Raycastlados * orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX() * RaycastladosY, cuboNodo->getPosition().y - 1, orientacion.getX()*Raycastlados + cuboNodo->getPosition().z + orientacion.getZ()*RaycastladosY);
	fingirado = orientacion.rotate(btVector3(0, 1, 0), 90 * MPI / 180);
	fingirado.setX(fingirado.getX()*-distanciaRaycastLados + cuboNodo->getPosition().x + orientacion.getX() * RaycastladosY);
	fingirado.setZ(fingirado.getZ()*-distanciaRaycastLados + cuboNodo->getPosition().z + orientacion.getZ() * RaycastladosY);
	fingirado.setY(inicio.getY());

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fingirado, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast6(inicio, fingirado);
	RayCast6.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast6.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fingirado, RayCast6);


	// Raycast izquierda2 
	inicio = btVector3(-Raycastlados * orientacion.getZ() + cuboNodo->getPosition().x - orientacion.getX() * RaycastladosY, cuboNodo->getPosition().y - 1, orientacion.getX()*Raycastlados + cuboNodo->getPosition().z - orientacion.getZ() * RaycastladosY);
	fingirado = orientacion.rotate(btVector3(0, 1, 0), 90 * MPI / 180);
	fingirado.setX(fingirado.getX()*-distanciaRaycastLados + cuboNodo->getPosition().x - orientacion.getX() * RaycastladosY);
	fingirado.setZ(fingirado.getZ()*-distanciaRaycastLados + cuboNodo->getPosition().z - orientacion.getZ() * RaycastladosY);
	fingirado.setY(inicio.getY());

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fingirado, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast7(inicio, fingirado);
	RayCast7.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast7.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fingirado, RayCast7);

	// Raycast derecha1 
	inicio = btVector3(Raycastlados * orientacion.getZ() + cuboNodo->getPosition().x + orientacion.getX() * RaycastladosY, cuboNodo->getPosition().y - 1, orientacion.getX()*-Raycastlados + cuboNodo->getPosition().z + orientacion.getZ() * RaycastladosY);
	fingirado = orientacion.rotate(btVector3(0, 1, 0), 90 * MPI / 180);
	fingirado.setX(fingirado.getX()*distanciaRaycastLados + cuboNodo->getPosition().x + orientacion.getX() * RaycastladosY);
	fingirado.setZ(fingirado.getZ()*distanciaRaycastLados + cuboNodo->getPosition().z + orientacion.getZ() * RaycastladosY);
	fingirado.setY(inicio.getY());

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fingirado, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast8(inicio, fingirado);
	RayCast8.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast8.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fingirado, RayCast8);

	// Raycast derecha2 
	inicio = btVector3(Raycastlados * orientacion.getZ() + cuboNodo->getPosition().x - orientacion.getX() * RaycastladosY, cuboNodo->getPosition().y - 1, orientacion.getX()*-Raycastlados + cuboNodo->getPosition().z - orientacion.getZ() * RaycastladosY);
	fingirado = orientacion.rotate(btVector3(0, 1, 0), 90 * MPI / 180);
	fingirado.setX(fingirado.getX()*distanciaRaycastLados + cuboNodo->getPosition().x - orientacion.getX() * RaycastladosY);
	fingirado.setZ(fingirado.getZ()*distanciaRaycastLados + cuboNodo->getPosition().z - orientacion.getZ() * RaycastladosY);
	fingirado.setY(inicio.getY());

	if (debugFisicas)
		mundo->getDebugDrawer()->drawLine(inicio, fingirado, btVector4(0, 0, 1, 1));

	btCollisionWorld::AllHitsRayResultCallback RayCast9(inicio, fingirado);
	RayCast9.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast9.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fingirado, RayCast9);





	caja = false;
	distanciaCaja = 0;
	turbo = false;
	distanciaTurbo = 0;
	enemigo = false;
	distanciaEnemigo = 0;
	Vision = false;
	enemigolados = false;



	if (RayCast1.hasHit()) {
		for (int i = 0; i < RayCast1.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast1.m_collisionObjects[i]->getUserPointer());
			if (Node) {

				if (strcmp(Node->getName(), "Caja") == 0 && Node->isVisible()
					|| strcmp(Node->getName(), "Turbo") == 0
					|| strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					vision(RayCast1.m_hitFractions[i], Node);

				}
			}
		}

	}
	if (RayCast2.hasHit()) {

		for (int i = 0; i < RayCast2.m_hitFractions.size(); i++)
		{
			obj3D *Node = static_cast<obj3D *>(RayCast2.m_collisionObjects[i]->getUserPointer());
			if (Node) {


				if (strcmp(Node->getName(), "Caja") == 0 && Node->isVisible()
					|| strcmp(Node->getName(), "Turbo") == 0
					|| strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					vision(RayCast2.m_hitFractions[i], Node);


				}
			}
		}

	}
	if (RayCast3.hasHit()) {
		for (int i = 0; i < RayCast3.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast3.m_collisionObjects[i]->getUserPointer());
			if (Node) {

				if (strcmp(Node->getName(), "Caja") == 0 && Node->isVisible()
					|| strcmp(Node->getName(), "Turbo") == 0
					|| strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					vision(RayCast3.m_hitFractions[i], Node);

				}
			}
		}
	}
	if (RayCast4.hasHit()) {

		for (int i = 0; i < RayCast4.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast4.m_collisionObjects[i]->getUserPointer());
			if (Node) {



				if (strcmp(Node->getName(), "Caja") == 0 && Node->isVisible()
					|| strcmp(Node->getName(), "Turbo") == 0
					|| strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					vision(RayCast4.m_hitFractions[i], Node);

				}
			}
		}
	}

	if (RayCast5.hasHit()) {
		for (int i = 0; i < RayCast5.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast5.m_collisionObjects[i]->getUserPointer());
			if (Node) {
				if (strcmp(Node->getName(), "Caja") == 0 && Node->isVisible()
					|| strcmp(Node->getName(), "Turbo") == 0
					|| strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					vision(RayCast5.m_hitFractions[i], Node);

				}
			}
		}
	}

	if (RayCast6.hasHit()) {

		for (int i = 0; i < RayCast6.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast6.m_collisionObjects[i]->getUserPointer());
			if (Node) {
				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					enemigolados = true;

				}
			}
		}
	}

	if (RayCast7.hasHit()) {
		for (int i = 0; i < RayCast7.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast7.m_collisionObjects[i]->getUserPointer());
			if (Node) {
				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					enemigolados = true;

				}
			}
		}

	}

	if (RayCast8.hasHit()) {

		for (int i = 0; i < RayCast8.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast8.m_collisionObjects[i]->getUserPointer());
			if (Node) {
				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					enemigolados = true;

				}
			}
		}

	}

	if (RayCast9.hasHit()) {
		for (int i = 0; i < RayCast9.m_hitFractions.size(); i++) {
			obj3D *Node = static_cast<obj3D *>(RayCast9.m_collisionObjects[i]->getUserPointer());
			if (Node) {
				if (strcmp(Node->getName(), "Jugador") == 0
					|| strcmp(Node->getName(), "JugadorIA") == 0) {

					enemigolados = true;

				}
			}
		}
	}

	if (Objeto) {
		if (enemigo && turbo) {
			if (distanciaEnemigo < distanciaTurbo) {
				distanciaObjetivo = distanciaEnemigo;
				enemigo = true;
				turbo = false;
			}
			else {
				distanciaObjetivo = distanciaTurbo;
				enemigo = false;
				turbo = true;
			}
			Vision = true;
		}
		else {
			if (enemigo) {
				distanciaObjetivo = distanciaEnemigo;
				caja = false;
				turbo = false;
				enemigo = true;
				Vision = true;
			}
			else if (turbo) {
				distanciaObjetivo = distanciaTurbo;
				caja = false;
				turbo = true;
				enemigo = false;
				Vision = true;
			}
		}

	}
	else {

		if (caja && turbo) {

			if (distanciaCaja < distanciaTurbo) {
				distanciaObjetivo = distanciaCaja;
				caja = true;
				turbo = false;
				enemigo = false;
			}
			else {
				distanciaObjetivo = distanciaTurbo;
				caja = false;
				turbo = true;
				enemigo = false;
			}
			Vision = true;
		}
		else {
			if (caja) {
				distanciaObjetivo = distanciaCaja;
				caja = true;
				turbo = false;
				enemigo = false;
				Vision = true;
			}
			else if (turbo) {
				distanciaObjetivo = distanciaTurbo;
				caja = false;
				turbo = true;
				enemigo = false;
				Vision = true;
			}
		}
		if(enemigo){
			enemigo=true;
		}

	}


}


void CorredorIA::vision(btScalar distancia, obj3D *nodo) {
	if (strcmp(nodo->getName(), "Caja") == 0) {
		btScalar aux = getDistanciaPunto(btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z));

		if (distanciaCaja == 0) {
			caja = true;
			posicionCaja = btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z);
			distanciaCaja = aux;
		}

		if (aux < distanciaCaja) {
			caja = true;
			posicionCaja = btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z);
			distanciaCaja = aux;

		}
	}


	if ((strcmp(nodo->getName(), "Jugador") == 0
		|| strcmp(nodo->getName(), "JugadorIA") == 0)) {

		btScalar aux = getDistanciaPunto(btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z));

		if (distanciaEnemigo == 0) {
			enemigo = true;
			posicionEnemigo = btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z);
			distanciaEnemigo = aux;

		}

		if (aux < distanciaEnemigo) {
			enemigo = true;
			posicionEnemigo = btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z);
			distanciaEnemigo = aux;

		}
	}


	if (strcmp(nodo->getName(), "Turbo") == 0) {

		btScalar aux = getDistanciaPunto(btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z));

		if (distanciaTurbo == 0) {
			turbo = true;
			posicionTurbo = btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z);
			distanciaTurbo = aux;

		}

		if (aux < distanciaEnemigo) {
			turbo = true;
			posicionTurbo = btVector3(nodo->getPosition().x, nodo->getPosition().y, nodo->getPosition().z);
			distanciaTurbo = aux;

		}
	}

}

void CorredorIA::updateHijos() {

	debugIA();
	//if (!debugFisicas)
		ActualizarRaytest();

	if (tipoObj != 0) {
		Objeto = true;
	}
	else {
		Objeto = false;
	}

	if (limite >= 100) {
		habilidad = true;
	}
	else {
		habilidad = false;
	}

	logicaDifusa();

}

void CorredorIA::deleteArbol() {

	arbolconduccion->deleteNodosArbol();
	delete arbolconduccion;
	arbolobjetos->deleteNodosArbol();
	delete arbolobjetos;

}

void CorredorIA::actualizarItem() {

}

CorredorIA::~CorredorIA() {

}


void CorredorIA::setDebugFisicas(bool activo) {
	debugFisicas = activo;
}

bool CorredorIA::getDebugFisicas() {
	return debugFisicas;
}


void CorredorIA::debugIA() {

	
	std::string agrega;
	texto="";
	texto+=("WAYPOINT ACTUAL: " + to_string(actual->getID() - 7));
	texto+=("\n");
	texto+="WAYPOINT SIGUIENTE: " + to_string(siguiente->getID() - 7);
	texto+="\n";

	if (noGiro)
		agrega = "NO GIRO // ";
	else if (giroFlojoDerecha)
		agrega = "GIRO IZQUIERDA // ";
	else if (giroFlojoIzquierda)
		agrega = "GIRO DERECHA // ";
	else if (giroFuerteDerecha)
		agrega = "GIRO FUERTE IZQUIERDA //";
	else if (giroFuerteIzquierda)
		agrega = "GIRO FUERTE DERECHA //";

	switch (accionActualConduccion) {

	case NADA1:
		break;

	case ACELERAR: //1
		agrega += "ACELERAR";
		break;

	case FRENAR: //2
		agrega += "FRENAR";
		break;

	case ACELERARGIRARDERECHA://3
		agrega += "ACELERAR";
		break;

	case ACELERARGIRARIZQUIERDA://4
		agrega += "ACELERAR";
		break;

	case ACELERARGIRARFUERTEDERECHA://5
		agrega += "ACELERAR";
		break;

	case ACELERARGIRARFUERTEIZQUIERDA://6
		agrega += "ACELERAR";
		break;

	case FRENARGIRARFUERTEDERECHA://7
		agrega += "FRENAR";
		break;

	case FRENARGIRARFUERTEIZQUIERDA://8
		agrega += "FRENAR";
		break;


	}

	texto+="ACCION CONDUCCION: " + agrega;
	texto+="\n";

	switch (accionActualObjetos) {

	case NADA1:
		break;

	case DISTANCIAOBJETIVOCAJA: //9
		agrega = "VEO CAJA";
		break;

	case DISTANCIAOBJETIVOENEMIGO: //10
		agrega = "VEO ENEMIGO";
		break;

	case DISTANCIAOBJETIVOTURBO://11
		agrega = "VEO TURBO";
		break;

	case SEGUIRWAYPOINT://12
		agrega = "SIGO WAYPOINT";
		break;

	case USAROBJETO://13

		switch (tipoObj) {
		case 0:

			break;
		case 1:
			agrega = "USAR OBJETO // FLECHA";
			break;
		case 2:
			agrega = "USAR OBJETO // CAJA FALSA";
			break;
		case 3:
			agrega = "USAR OBJETO // TURBO";
			break;
		case 4:
			agrega = "USAR OBJETO // ACEITE";
			break;
		case 5:
			agrega = "USAR OBJETO // ESCUDO";
			break;
		case 6:
			agrega = "USAR OBJETO // FLECHA TRIPLE";
			break;
		case 7:
			agrega = "USAR OBJETO // FLECHA TELEDIRIGIDA";
			break;
		case 8:
			agrega = "USAR OBJETO // TURBO TRIPLE";
			break;

		}

		break;

	case USARHABILIDAD: //14
		agrega = "USAR HABILIDAD ";
		break;



	}
	texto+="ACCION DECISION ACTUAL: " + agrega;
	texto+="\n";
}

std::string CorredorIA::getDebugIA(){
	return texto;
}


void CorredorIA::animacionIzquierda(){
		iniciarAnimacion(GIROIZQINI, animacionPrevia, getGiroIzqFin());
		//animacionPrevia = getGiroIzqIni();
		giroIzq = true;
}
void CorredorIA::animacionDerecha(){
		iniciarAnimacion(GIRODERINI, animacionPrevia, getGiroDerFin());
		
		giroDer = true;
}
void CorredorIA::finAnimacion(){
	if (giroDer) {
	
		iniciarAnimacion(GIRODERFIN, animacionPrevia,getGiroDerIni());
		animacionPrevia = getGiroDerFin();
		giroDer = false;
	}
	if (giroIzq) {
	
		iniciarAnimacion(GIROIZQFIN, animacionPrevia, getGiroIzqIni());
		animacionPrevia = getGiroIzqFin();
		giroIzq = false;
	}
}