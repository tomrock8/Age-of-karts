#include "CorredorIA.hpp"

CorredorIA::CorredorIA(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo) : Corredor(rutaObj, pos, tipo)
{
	nombre="JugadorIA";
	cuboNodo->setName(nombre);
	caja = false;
	enemigo = false;
	turbo= false;
	Vision=false;
	Objeto=false;
	distanciaCerca=false;
	distanciaMedia=false;
	distanciaLejos=false;
	velocidadBaja=false;
	velocidadMedia=false;
	velocidadAlta=false;
	giroFuerteDerecha=false;
	giroFlojoDerecha=false;
	noGiro=false;
	giroFuerteIzquierda=false;
	giroFlojoIzquierda=false;
	distanciaCaja = 0;
	distanciaEnemigo = 0;
	distanciaTurbo = 0;
	timerRecolocar=0;
	pertenenciaCerca=0;
	pertenenciaMedia=0;
	pertenenciaLejos=0;
	pertenenciaGiroFuerteDerecha=0;
	pertenenciaGiroFlojoDerecha=0;
	pertenenciaNoGiro=0;
	pertenenciaGiroFuerteIzquierda=0;
	pertenenciaGiroFlojoIzquierda=0;
	pertenenciaVelocidadBaja=0;
	pertenenciaVelocidadMedia=0;
	pertenenciaVelocidadAlta=0;




	arbolconduccion = new ArbolDecision();
	arbolconduccion->nuevoNodoDecision(0,NADA2,NADA3,0,false); // 0 NODO PADRE
	
	//NodosHijos
	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROFUERTEDERECHA,0,true); // 1 GiroFuertederecha
	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROMEDIODERECHA,0,true); // 2 GiroDerecha
	arbolconduccion->nuevoNodoDecision(0,IGUAL,NOGIRO,0,true); // 3 NoGiro
	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROMEDIOIZQUIERDA,0,true); // 4 GiroIzquierda
 	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROFUERTEIZQUIERDA,0,true);  // 5 GiroFuerteIzquierda

	//GiroFuertederecha 1
	arbolconduccion->nuevoNodoDecision(1,IGUAL,VELOCIDADBAJA,0,true); // 6
	arbolconduccion->nuevoNodoDecision(1,IGUAL,VELOCIDADMEDIA,0,true); // 7
	arbolconduccion->nuevoNodoDecision(1,IGUAL,VELOCIDADALTA,0,true); // 8

	arbolconduccion->nuevoNodoAccion(6,5); //9
	arbolconduccion->nuevoNodoAccion(7,5); //10
	arbolconduccion->nuevoNodoAccion(8,7); //11

	//GiroDerecha 2
	arbolconduccion->nuevoNodoAccion(2,3); // 12
	//NoGiro 3
	arbolconduccion->nuevoNodoAccion(3,1); // 13
	//GiroIzquierda 4
	arbolconduccion->nuevoNodoAccion(4,4); // 14

	//GiroFuerteIzquierda 5
	arbolconduccion->nuevoNodoDecision(5,IGUAL,VELOCIDADBAJA,0,true); // 15
	arbolconduccion->nuevoNodoDecision(5,IGUAL,VELOCIDADMEDIA,0,true); // 16
	arbolconduccion->nuevoNodoDecision(5,IGUAL,VELOCIDADALTA,0,true); // 17

	arbolconduccion->nuevoNodoAccion(15,6); //18
	arbolconduccion->nuevoNodoAccion(16,6); //19
	arbolconduccion->nuevoNodoAccion(17,8); //20



	arbolobjetos = new ArbolDecision();
	arbolobjetos->nuevoNodoDecision(0,NADA2,NADA3,0,false); // 0 NODO PADRE

	//NodosHijos
	arbolobjetos->nuevoNodoDecision(0,IGUAL,OBJBOOLEANO,0,true); // 1 Objeto
	arbolobjetos->nuevoNodoDecision(0,IGUAL,OBJBOOLEANO,0,false); // 2 !Objeto

	//Objeto 1
	arbolobjetos->nuevoNodoDecision(1,IGUAL,VISION,0,true); // 3 Vision
	arbolobjetos->nuevoNodoDecision(1,IGUAL,VISION,0,false); // 4 !Vision

	//Vision 3
	arbolobjetos->nuevoNodoDecision(3,IGUAL,ENEMIGO,0,true); // 5 Enemigo
	arbolobjetos->nuevoNodoDecision(3,IGUAL,TURBOO,0,true); // 6 Turbo

	//Enemigo 5
	arbolobjetos->nuevoNodoDecision(5,IGUAL,DISTANCIACERCA,0,true); // 7 CERCA
	arbolobjetos->nuevoNodoDecision(5,IGUAL,DISTANCIAMEDIA,0,true); // 8 MEDIO
	arbolobjetos->nuevoNodoDecision(5,IGUAL,DISTANCIALEJOS,0,true); // 9 LEJOS

	//CERCA 7
	arbolobjetos->nuevoNodoDecision(7,IGUAL,OBJNUMERO,1,false); // 10 OBJ1
	arbolobjetos->nuevoNodoDecision(7,IGUAL,OBJNUMERO,6,false); // 11 OBJ6

	//OBJ1 10
	arbolobjetos->nuevoNodoDecision(10,IGUAL,NOGIRO,0,true); // 12 NOGIRO
	arbolobjetos->nuevoNodoDecision(10,IGUAL,NOGIRO,0,false);// 13 !NOGIRO

	//NOGIRO 12
	arbolobjetos->nuevoNodoAccion(12,13); // 14 Lanzo el objeto accion a concretar
	//!NOGIRO 13
	arbolobjetos->nuevoNodoAccion(13,10); // 15 Sigo al objetivo accion a concretar

	//OBJ6 11
	arbolobjetos->nuevoNodoDecision(11,IGUAL,NOGIRO,0,true); // 16 NOGIRO
	arbolobjetos->nuevoNodoDecision(11,IGUAL,NOGIRO,0,false);// 17 !NOGIRO

	//NOGIRO 16
	arbolobjetos->nuevoNodoAccion(16,13); // 18 Lanzo el objeto accion a concretar
	//!NOGIRO 17
	arbolobjetos->nuevoNodoAccion(17,10); // 19 Sigo al objetivo accion a concretar

	//MEDIO 8
	arbolobjetos->nuevoNodoDecision(8,IGUAL,OBJNUMERO,1,false); // 20 OBJ1
	arbolobjetos->nuevoNodoDecision(8,IGUAL,OBJNUMERO,6,false); // 21 OBJ6

	//OBJ1 20
	arbolobjetos->nuevoNodoDecision(20,IGUAL,NOGIRO,0,true); // 22 NOGIRO
	arbolobjetos->nuevoNodoDecision(20,IGUAL,NOGIRO,0,false);// 23 !NOGIRO

	//NOGIRO 22
	arbolobjetos->nuevoNodoAccion(22,13); // 24 Lanzo el objeto accion a concretar
	//!NOGIRO 23
	arbolobjetos->nuevoNodoAccion(23,10); // 25 Sigo al objetivo accion a concretar

	//OBJ6 21
	arbolobjetos->nuevoNodoDecision(21,IGUAL,NOGIRO,0,true); // 26 NOGIRO
	arbolobjetos->nuevoNodoDecision(21,IGUAL,NOGIRO,0,false);// 27 !NOGIRO

	//NOGIRO 26
	arbolobjetos->nuevoNodoAccion(26,13); // 28 Lanzo el objeto accion a concretar
	//!NOGIRO 27
	arbolobjetos->nuevoNodoAccion(27,10); // 29 Sigo al objetivo accion a concretar

	//LEJOS 9
	arbolobjetos->nuevoNodoAccion(9,12); // 30 Sigo al waypoint a concretar


	//TURBO 6
	arbolobjetos->nuevoNodoDecision(6,IGUAL,DISTANCIACERCA,0,true); // 31 CERCA
	arbolobjetos->nuevoNodoDecision(6,IGUAL,DISTANCIAMEDIA,0,true); // 32 MEDIO
	arbolobjetos->nuevoNodoDecision(6,IGUAL,DISTANCIALEJOS,0,true); // 33 LEJOS

	//CERCA 31
	arbolobjetos->nuevoNodoAccion(31,11); // 34 Sigo al TURBO a concretar
	//MEDIO 32
	arbolobjetos->nuevoNodoAccion(32,11); // 35 Sigo al TURBO a concretar
	//LEJOS 33
	arbolobjetos->nuevoNodoAccion(33,12); // 36 Sigo al waypoint a concretar


	//!VISION 4
	arbolobjetos->nuevoNodoDecision(4,IGUAL,OBJNUMERO,1,true); // 37 OBJ1
	

	//!Objeto 2
	arbolobjetos->nuevoNodoDecision(2,IGUAL,VISION,0,true); // 38 Vision
	arbolobjetos->nuevoNodoDecision(2,IGUAL,VISION,0,false); // 39 !Vision

	//Vision 38
	arbolobjetos->nuevoNodoDecision(38,IGUAL,CAJA,0,true); // 40 Caja
	arbolobjetos->nuevoNodoDecision(38,IGUAL,TURBOO,0,true); // 41 Turbo
	
	//Caja 40
	arbolobjetos->nuevoNodoDecision(40,IGUAL,DISTANCIACERCA,0,true); // 42 CERCA
	arbolobjetos->nuevoNodoDecision(40,IGUAL,DISTANCIAMEDIA,0,true); // 43 MEDIO
	arbolobjetos->nuevoNodoDecision(40,IGUAL,DISTANCIALEJOS,0,true); // 44 LEJOS

	//CERCA 42
	arbolobjetos->nuevoNodoAccion(42,9); // 45 Sigo caja a concretar

	//MEDIO 43
	arbolobjetos->nuevoNodoAccion(43,9); // 46 Sigo caja a concretar

	//LEJOS 44
	arbolobjetos->nuevoNodoAccion(44,12); // 47 Sigo WAYPOINT a concretar

	//Turbo 41	
	arbolobjetos->nuevoNodoDecision(41,IGUAL,DISTANCIACERCA,0,true); // 48 CERCA
	arbolobjetos->nuevoNodoDecision(41,IGUAL,DISTANCIAMEDIA,0,true); // 49 MEDIO
	arbolobjetos->nuevoNodoDecision(41,IGUAL,DISTANCIALEJOS,0,true); // 50 LEJOS

	//CERCA 48
	arbolobjetos->nuevoNodoAccion(48,11); // 51 Sigo TURBO a concretar

	//MEDIO 49
	arbolobjetos->nuevoNodoAccion(49,11); // 52 Sigo TURBO a concretar

	//LEJOS 50
	arbolobjetos->nuevoNodoAccion(50,12); // 53 Sigo WAYPOINT a concretar

	//!VISION 39
	arbolobjetos->nuevoNodoAccion(39,12); // 54 Sigo WAYPOINT a concretar


	arbolobjetos->nuevoNodoDecision(4,IGUAL,OBJNUMERO,6,true); // 55 OBJ6
	arbolobjetos->nuevoNodoAccion(4,13); // 56 Lanzo el objeto accion a concretar


	arbolobjetos->nuevoNodoAccion(37,12);
	arbolobjetos->nuevoNodoAccion(55,12);

}

//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
void CorredorIA::movimiento()
{
	//LLmamos a la logica difusa para que nos de los valores de entrada
	//seguirWaypoint();
	ActualizarRaytest();
	logicaDifusa();
	

	bool arraybooleanos[16] = { 
	distanciaCerca,distanciaMedia,distanciaLejos, // del 0 al 2 distancias
	velocidadBaja,velocidadMedia,velocidadAlta, // del 3 al 5 velocidades
	noGiro,giroFlojoDerecha,giroFlojoIzquierda,giroFuerteDerecha,giroFuerteIzquierda, // del 6 al 10 conduccion
	caja,turbo,enemigo,Vision,Objeto}; // del 11 al 15 vision 
	

		switch(arbolobjetos->recorrerArbol(arraybooleanos,tipoObj)){ //lo que devuelva el arbol conduccion

        case NADA1:
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
        
		break;

		case ACELERAR: //1
		acelerar();
        break;

    }

		switch(arbolconduccion->recorrerArbol(arraybooleanos,tipoObj)){ //lo que devuelva el arbol conduccion

        case NADA1:
        break;

        case ACELERAR: //1
		acelerar();
		vehiculo->setSteeringValue(0, 0);
		vehiculo->setSteeringValue(0, 1);	
        break;

        case FRENAR: //2
        frenar();
        break;

        case ACELERARGIRARDERECHA://3
		acelerar();
        girarDerecha();
        break;

        case ACELERARGIRARIZQUIERDA://4
        acelerar();
		girarIzquierda();
        break;

        case ACELERARGIRARFUERTEDERECHA://5
		acelerar();
        vehiculo->setSteeringValue(0.15, 0);
		vehiculo->setSteeringValue(0.15, 1);
        break;

        case ACELERARGIRARFUERTEIZQUIERDA://6
		acelerar();
        vehiculo->setSteeringValue(-0.15, 0);
		vehiculo->setSteeringValue(-0.15, 1);
        break;

		case FRENARGIRARFUERTEDERECHA://7
		frenar();
        vehiculo->setSteeringValue(0.15, 0);
		vehiculo->setSteeringValue(0.15, 1);
        break;

		case FRENARGIRARFUERTEIZQUIERDA://8
		frenar();
        vehiculo->setSteeringValue(-0.15, 0);
		vehiculo->setSteeringValue(-0.15, 1);
        break;

		
    }


	
/*
	if(Objeto){

		if(Vision){

			if(enemigo){
				
				if(distanciaCerca){

					if(tipoObj == 1 || tipoObj == 6){
						calculoAnguloGiro(posicionEnemigo);
						if(noGiro)
						usarObjetos();
					}

				}else if(distanciaMedia){

					if(tipoObj == 1 || tipoObj == 6){
						calculoAnguloGiro(posicionEnemigo);
						if(noGiro)
						usarObjetos();
					}


				}else if(distanciaLejos){

					seguirWaypoint();
				}

			}else if(turbo){

				
					if(distanciaCerca){

					calculoAnguloGiro(posicionTurbo);
				}else if(distanciaMedia){

					calculoAnguloGiro(posicionTurbo);

				}else if(distanciaLejos){

					seguirWaypoint();
				}

			}

		}else{

			if(tipoObj==3 || tipoObj ==8){

				//poner condicion de las rectas para usar turbos
				tipoObj=0;
				
			}else if (tipoObj==1 || tipoObj ==6){

				seguirWaypoint();
			}else{

				usarObjetos();
			}

		}


	}else{

		if(Vision){

			if(caja){
				
				if(distanciaCerca){

					calculoAnguloGiro(posicionCaja);
					

				}else if(distanciaMedia){

					calculoAnguloGiro(posicionCaja);

				}else if(distanciaLejos){

					seguirWaypoint();
				}

			}else if(turbo){

				
					if(distanciaCerca){

					calculoAnguloGiro(posicionTurbo);
				}else if(distanciaMedia){

					calculoAnguloGiro(posicionTurbo);

				}else if(distanciaLejos){

					seguirWaypoint();
				}

			}


	}else{
		seguirWaypoint();
	}

	}

*/	
	reposicionar();
	
}





void CorredorIA::seguirWaypoint(){

	btScalar distanciaCentro=btScalar(0);
	btScalar distanciaLado1=btScalar(0);
	btScalar distanciaLado2=btScalar(0);

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	distanciaCentro = getDistanciaPunto(siguiente->getPosicion());
	distanciaLado1 = getDistanciaPunto(siguiente->getVector1());
	distanciaLado2 = getDistanciaPunto(siguiente->getVector2());
	
	if(distanciaCentro<distanciaLado1 && distanciaCentro<distanciaLado2){
	calculoAnguloGiro(siguiente->getPosicion());
	
	}
	else{
 	if(distanciaLado1<distanciaCentro && distanciaLado1<distanciaLado2){
	calculoAnguloGiro(siguiente->getVector1());
	
	}
	if(distanciaLado2<distanciaCentro && distanciaLado2<distanciaLado1){
	calculoAnguloGiro(siguiente->getVector2());
	
	}
	}

}
void CorredorIA::reposicionar(){
	Timer *time = Timer::getInstancia();
	//cout<<"TIEMPO: "<<time->getTimer()-timerRecolocar<<endl;
	if (time->getTimer()-timerRecolocar==1){
		posicion_aux=posicion;
		
	}
	if (time->getTimer()-timerRecolocar>4){
		if ((unsigned)((unsigned)(posicion_aux.getX())-(unsigned)(posicion.getX()))<2 && (unsigned)((unsigned)(posicion_aux.getZ())-(unsigned)(posicion.getZ()))<2){
			btVector3 btPos = actual->getPosicion();

			btTransform trans;
			trans.setOrigin(btPos);
			btQuaternion quaternion;
			quaternion.setEulerZYX(cuboNodo->getRotation().Z* PI / 180, cuboNodo->getRotation().Y * PI / 180, cuboNodo->getRotation().X* PI / 180);
			trans.setRotation(quaternion);

			CuerpoColisionChasis->setCenterOfMassTransform(trans);
			timerRecolocar=time->getTimer();
			//cout<<"recolocado"<<endl;
		}else{
			//cout<<"sigue"<<endl;
		}
	}
	if (time->getTimer()-timerRecolocar>5){
		timerRecolocar=time->getTimer();
	}
}

void CorredorIA::calculoAnguloGiro(btVector3 posicion) {

	btVector3 orientacionCoche(orientacion.getX(),orientacion.getY(),orientacion.getZ());
	btVector3 direccion = btVector3(posicion.getX()-cuboNodo->getPosition().X,
	posicion.getY()-cuboNodo->getPosition().Y,
	posicion.getZ()-cuboNodo->getPosition().Z);
	
	//direccion.normalize();
	anguloGiro = orientacionCoche.angle(direccion) *180/PI;
	
	btVector3 orientacionCocheGirada = orientacionCoche.rotate(btVector3(0, 1, 0),2*PI/180);

	btScalar angulo2 = orientacionCocheGirada.angle(direccion) *180/PI;
		
		if(angulo2>anguloGiro)
			anguloGiro = -anguloGiro;


}


void CorredorIA::logicaDifusa() {
	

	if(tipoObj!=0){

		Objeto=true;
	}else{
		Objeto=false;
	}



	//GIRO
	pertenenciaNoGiro= FuncionTriangular(anguloGiro,-30,0,30);
	pertenenciaGiroFlojoDerecha=FuncionTriangular(anguloGiro,3,20,35);
	pertenenciaGiroFlojoIzquierda=FuncionTriangular(anguloGiro,-3,-20,-35);
	pertenenciaGiroFuerteDerecha=FuncionTriangular(anguloGiro,30,80,180);
	pertenenciaGiroFuerteIzquierda=FuncionTriangular(anguloGiro,-30,-80,-180);

	//VELOCIDAD
	pertenenciaVelocidadBaja= FuncionTrapezoidal(vehiculo->getCurrentSpeedKmHour(), 0, 0, 50, 150);
	pertenenciaVelocidadMedia=FuncionTrapezoidal(vehiculo->getCurrentSpeedKmHour(), 100, 150, 200, 250);
	pertenenciaVelocidadAlta=FuncionTrapezoidal(vehiculo->getCurrentSpeedKmHour(), 200, 250, 300, 450);

		//DISTANCIA
	pertenenciaCerca = FuncionTrapezoidal(distanciaObjetivo, 0, 0, 1000, 3000);
	pertenenciaMedia = FuncionTrapezoidal(distanciaObjetivo, 2000, 4000, 5000, 6000);
	pertenenciaLejos = FuncionTrapezoidal(distanciaObjetivo, 5000, 8000, 9000, 100000);


	if(pertenenciaVelocidadBaja>pertenenciaVelocidadAlta && pertenenciaVelocidadBaja > pertenenciaVelocidadMedia){
		velocidadBaja = true;
		velocidadAlta = false;
		velocidadMedia = false;

	}

	if(pertenenciaVelocidadMedia>pertenenciaVelocidadAlta && pertenenciaVelocidadBaja < pertenenciaVelocidadMedia){
		velocidadBaja = false;
		velocidadAlta = false;
		velocidadMedia = true;

	}

	if(pertenenciaVelocidadBaja<pertenenciaVelocidadAlta && pertenenciaVelocidadAlta > pertenenciaVelocidadMedia){
		velocidadBaja = false;
		velocidadAlta = true;
		velocidadMedia = false;

	}



	if(pertenenciaCerca > pertenenciaMedia && pertenenciaCerca > pertenenciaLejos){
		distanciaCerca=true;
		distanciaLejos=false;
		distanciaMedia=false;
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

		if(pertenenciaNoGiro>pertenenciaGiroFlojoDerecha && pertenenciaNoGiro>pertenenciaGiroFlojoIzquierda
		&&pertenenciaNoGiro>pertenenciaGiroFuerteDerecha && pertenenciaNoGiro>pertenenciaGiroFuerteIzquierda){
			giroFuerteDerecha=false;
			giroFlojoDerecha=false;
			noGiro=true;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=false;
		}

		if(pertenenciaGiroFlojoDerecha>pertenenciaNoGiro && pertenenciaGiroFlojoDerecha>pertenenciaGiroFuerteDerecha
			&& pertenenciaGiroFlojoDerecha>pertenenciaGiroFlojoIzquierda && pertenenciaGiroFlojoDerecha>pertenenciaGiroFuerteIzquierda){
			giroFuerteDerecha=false;
			giroFlojoDerecha=true;
			noGiro=false;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=false;
		}
		if(pertenenciaGiroFuerteDerecha>pertenenciaNoGiro && pertenenciaGiroFuerteDerecha>pertenenciaGiroFlojoDerecha
		&& pertenenciaGiroFuerteDerecha>pertenenciaGiroFlojoIzquierda && pertenenciaGiroFuerteDerecha>pertenenciaGiroFuerteIzquierda){
			giroFuerteDerecha=true;
			giroFlojoDerecha=false;
			noGiro=false;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=false;
		}	

		if(pertenenciaGiroFlojoIzquierda>pertenenciaNoGiro &&  pertenenciaGiroFlojoIzquierda>pertenenciaGiroFuerteIzquierda
		&& pertenenciaGiroFlojoIzquierda > pertenenciaGiroFlojoDerecha && pertenenciaGiroFlojoIzquierda > pertenenciaGiroFuerteDerecha){
			giroFuerteDerecha=false;
			giroFlojoDerecha=false;
			noGiro=false;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=true;
		}
		if(pertenenciaGiroFuerteIzquierda>pertenenciaNoGiro && pertenenciaGiroFuerteIzquierda>pertenenciaGiroFlojoIzquierda
		&& pertenenciaGiroFuerteIzquierda>pertenenciaGiroFlojoDerecha && pertenenciaGiroFuerteIzquierda>pertenenciaGiroFuerteDerecha){
			giroFuerteDerecha=false;
			giroFlojoDerecha=false;
			noGiro=false;
			giroFuerteIzquierda=true;
			giroFlojoIzquierda=false;
		}	

		TextoPantalla * texto = TextoPantalla::getInstancia(); 
		//texto->agregar("ACCION 1: "); 
		std::string agrega; 
		if(distanciaLejos)
   			agrega = "ACELERA A TOPE"; 	
		if(distanciaMedia)    
			agrega = "Reduce velocidad"; 
		if(distanciaCerca)
    		agrega = "Echa el freno fiera"; 
		
		//texto->agregar(agrega+"\n"); 
		
		//texto->agregar("ACCION 2: "); 
		if(noGiro)
		agrega = "No GIRO";
		if(giroFlojoDerecha)
		agrega = "Giro POCO D";
		if(giroFuerteDerecha)
		agrega = "Giro a tope D";
		if(giroFlojoIzquierda)
		agrega = "Giro POCO I";
		if(giroFuerteIzquierda)
		agrega = "Giro a tope I";

		//texto->agregar(agrega+"\n"); 

}

double CorredorIA::FuncionTrapezoidal(double valor,double a,double b,double c, double d){

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

	if (valor >=0)
	{
	if(a<=valor && valor < b)
		resultado = (valor-a)/(b-a);
	else if(valor == b)
		resultado = 1;
	else if (b < valor && valor <= c)
		resultado = (c - valor) / (c - b);

	}else{
	
	if(a>=valor && valor > b)
		resultado = (valor-a)/(b-a);
	else if(valor == b)
		resultado = 1;
	else if(b>valor && valor >=c)
		resultado = (c-valor)/(c-b);

	}
	return resultado;


}

void CorredorIA::ActualizarRaytest() {

	MotorFisicas *mun = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = mun->getMundo();
	mundo->updateAabbs();
	mundo->computeOverlappingPairs();

	//cout<< orientacion.X << "   "<< orientacion.Z<<endl;
	float distanciaRaycast = 150; // longitud del rayo
	float distanciaCoche = 2; // distancia entre el rayo y el coche, donde empieza
	float Raycast23 = 10; // distancia entre raycast 2 y 3
	float Raycast45 = 20; // distancia entre raycast 4 y 5

	// Raycast central1
	btVector3 inicio(cuboNodo->getPosition().X + orientacion.getX()*distanciaCoche, cuboNodo->getPosition().Y -1, cuboNodo->getPosition().Z+ orientacion.getZ()*distanciaCoche);
	btVector3 fin(cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y -1, cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);


	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);
	// Raycast central2 derecha
	//inicio = btVector3(3*orientacion.Z+cuboNodo->getPosition().X,cuboNodo->getPosition().Y+1,orientacion.X*-3+cuboNodo->getPosition().Z);
	inicio = btVector3(Raycast23*orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX(), cuboNodo->getPosition().Y - 1, orientacion.getX()*-Raycast23 + cuboNodo->getPosition().Z + orientacion.getZ() );
	fin = btVector3(Raycast23*orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y - 1, orientacion.getX()*-Raycast23 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast2(inicio, fin);
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast2);


	// Raycast central3 izquierda
	inicio = btVector3(-Raycast23 * orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX(), cuboNodo->getPosition().Y - 1, orientacion.getX()*Raycast23 + cuboNodo->getPosition().Z + orientacion.getZ() );
	fin = btVector3(-Raycast23 * orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y - 1, orientacion.getX()*Raycast23 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast3(inicio, fin);
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast3);


	// Raycast central4 derecha
	inicio = btVector3(Raycast45*orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX(), cuboNodo->getPosition().Y - 1, orientacion.getX()*-Raycast45 + cuboNodo->getPosition().Z + orientacion.getZ() );
	fin = btVector3(Raycast45*orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y - 1, orientacion.getX()*-Raycast45 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast4(inicio, fin);
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast4);

	// Raycast central5 izquierda
	inicio = btVector3(-Raycast45 * orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX(), cuboNodo->getPosition().Y - 1, orientacion.getX()*Raycast45 + cuboNodo->getPosition().Z + orientacion.getZ());
	fin = btVector3(-Raycast45 * orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y - 1, orientacion.getX()*Raycast45 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast5(inicio, fin);
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast5);

	caja=false;	
	distanciaCaja=0;
	turbo=false;	
	distanciaTurbo=0;
	enemigo=false;
	distanciaEnemigo=0;
	Vision=false;

	

	if (RayCast1.hasHit())
	{

		for (int i=0;i<RayCast1.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast1.m_collisionObjects[i]->getUserPointer());
		if (Node) {


			if(strcmp(Node->getName(),"Caja")==0 &&  Node->isVisible() 
			|| strcmp(Node->getName(),"Turbo")==0 
			|| strcmp(Node->getName(),"Jugador") ==0
			|| strcmp(Node->getName(),"JugadorIA")==0){

			vision(RayCast1.m_hitFractions[i],Node);
			
			}
		}
			}

	}
	if (RayCast2.hasHit()) {

		for (int i=0;i<RayCast2.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast2.m_collisionObjects[i]->getUserPointer());
		if (Node) {
		

			if(strcmp(Node->getName(),"Caja")==0 &&  Node->isVisible() 
			|| strcmp(Node->getName(),"Turbo")==0 
			|| strcmp(Node->getName(),"Jugador")==0
			|| strcmp(Node->getName(),"JugadorIA")==0){

			vision(RayCast2.m_hitFractions[i],Node);
			
			
			}
		}
			}

	}
	if (RayCast3.hasHit()) {

		for (int i=0;i<RayCast3.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast3.m_collisionObjects[i]->getUserPointer());
		if (Node) {
	
			if(strcmp(Node->getName(),"Caja")==0 &&  Node->isVisible() 
			|| strcmp(Node->getName(),"Turbo")==0 
			|| strcmp(Node->getName(),"Jugador")==0
			|| strcmp(Node->getName(),"JugadorIA")==0){

			vision(RayCast3.m_hitFractions[i],Node);
		
			}
		}
			}
	}
	if (RayCast4.hasHit()) {

		for (int i=0;i<RayCast4.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast4.m_collisionObjects[i]->getUserPointer());
		if (Node) {
			

		
			if(strcmp(Node->getName(),"Caja")==0 &&  Node->isVisible() 
			|| strcmp(Node->getName(),"Turbo")==0 
			|| strcmp(Node->getName(),"Jugador")==0 
			|| strcmp(Node->getName(),"JugadorIA")==0){

			vision(RayCast4.m_hitFractions[i],Node);
		
			}
		}
			}
	}

	if (RayCast5.hasHit()) {
		for (int i=0;i<RayCast5.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast5.m_collisionObjects[i]->getUserPointer());
		if (Node) {
			

			if(strcmp(Node->getName(),"Caja")==0 && Node->isVisible() 
			|| strcmp(Node->getName(),"Turbo")==0 
			|| strcmp(Node->getName(),"Jugador")==0
			|| strcmp(Node->getName(),"JugadorIA")==0){

			vision(RayCast5.m_hitFractions[i],Node);
			
			}
		}
			}
	}



	if(Objeto){

		if(enemigo && turbo){

			if(distanciaEnemigo<distanciaTurbo){
					distanciaObjetivo = distanciaEnemigo;
					//logicaDifusa();
					enemigo=true;
					turbo=false;
				}else{
					distanciaObjetivo = distanciaTurbo;
					//logicaDifusa();
					enemigo=false;
					turbo=true;
				}
					Vision=true;
	}else{
					if(enemigo){			
					distanciaObjetivo = distanciaEnemigo;
					//logicaDifusa();
					caja=false;
					turbo=false;
					enemigo=true;
					Vision=true;
					}else if(turbo){
					distanciaObjetivo = distanciaTurbo;
					//logicaDifusa();
					caja=false;
					turbo=true;
					enemigo=false;
					Vision=true;	
				}
	}

	}else{

		if(caja && turbo){

			if(distanciaCaja<distanciaTurbo){
					distanciaObjetivo = distanciaCaja;
					//logicaDifusa();
					caja=true;
					turbo=false;
					enemigo=false;
				}else{
					distanciaObjetivo = distanciaTurbo;
					//logicaDifusa();
					caja=false;
					turbo=true;
					enemigo=false;
				}
				Vision=true;
		}else {
					if(caja){			
					distanciaObjetivo = distanciaCaja;
					//logicaDifusa();
					caja=true;
					turbo=false;
					enemigo=false;
					Vision=true;
					}else if(turbo){
					distanciaObjetivo = distanciaTurbo;
					//logicaDifusa();
					caja=false;
					turbo=true;
					enemigo=false;
					Vision=true;	
					}
			}

	}

	
}


void CorredorIA::vision(btScalar distancia,ISceneNode *nodo){



	if(strcmp(nodo->getName(),"Caja")==0){

		btScalar aux= getDistanciaPunto(btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z));

		if(distanciaCaja==0){
		caja=true;
		posicionCaja = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaCaja=aux;
		
		}

		if(aux<distanciaCaja){
		caja=true;
		posicionCaja = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaCaja=aux;
		
		}

	}
	

	if((strcmp(nodo->getName(),"Jugador")==0
	|| strcmp(nodo->getName(),"JugadorIA")==0)){

		btScalar aux= getDistanciaPunto(btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z));

		if(distanciaEnemigo==0){
		enemigo=true;
		posicionEnemigo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaEnemigo=aux;
		
		}

		if(aux<distanciaEnemigo){
		enemigo=true;
		posicionEnemigo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaEnemigo=aux;
		
		}
	}


	if(strcmp(nodo->getName(),"Turbo")==0){

		btScalar aux= getDistanciaPunto(btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z));

		if(distanciaTurbo==0){
		turbo=true;
		posicionTurbo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaTurbo=aux;
		
		}

		if(aux<distanciaEnemigo){
		turbo=true;
		posicionTurbo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaTurbo=aux;
		
		}
	}

}


void CorredorIA::comprobarDireccion(ISceneNode *nodo){


	


}



void CorredorIA::updateHijos(){

}


void CorredorIA::actualizarItem(){
	
}