#include "CorredorIA.hpp"

CorredorIA::CorredorIA(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo) : Corredor(rutaObj, pos, tipo)
{

	cuboNodo->setName("JugadorIA");
	caja = false;
	enemigo = false;
	turbo= false;
	distanciaCaja = 0;
	distanciaEnemigo = 0;
	distanciaTurbo = 0;
	timerRecolocar=0;

	arbolconduccion = new ArbolDecision();
	arbolconduccion->nuevoNodoDecision(0,NADA2,NADA3,0,false); // 0
	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROFUERTEDERECHA,0,true); // 1
	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROMEDIODERECHA,0,true); // 2
	arbolconduccion->nuevoNodoDecision(0,IGUAL,NOGIRO,0,true); // 3
	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROMEDIOIZQUIERDA,0,true); // 4
	arbolconduccion->nuevoNodoDecision(0,IGUAL,GIROFUERTEIZQUIERDA,0,true);  // 5

	arbolconduccion->nuevoNodoDecision(1,IGUAL,VELOCIDADBAJA,0,true); // 6
	arbolconduccion->nuevoNodoDecision(1,IGUAL,VELOCIDADMEDIA,0,true); // 7
	arbolconduccion->nuevoNodoDecision(1,IGUAL,VELOCIDADALTA,0,true); // 8

	arbolconduccion->nuevoNodoAccion(6,1); //9
	arbolconduccion->nuevoNodoAccion(7,1); //10
	arbolconduccion->nuevoNodoAccion(8,2); //11

	arbolconduccion->nuevoNodoAccion(2,1); //12

	arbolconduccion->nuevoNodoAccion(2,1); // 13
	arbolconduccion->nuevoNodoAccion(3,1); // 14
	arbolconduccion->nuevoNodoAccion(4,1); // 15

	arbolconduccion->nuevoNodoAccion(4,1); //16


	arbolconduccion->nuevoNodoDecision(5,IGUAL,VELOCIDADBAJA,0,true); // 17
	arbolconduccion->nuevoNodoDecision(5,IGUAL,VELOCIDADMEDIA,0,true); // 18
	arbolconduccion->nuevoNodoDecision(5,IGUAL,VELOCIDADALTA,0,true); // 19

	arbolconduccion->nuevoNodoAccion(17,1); //20
	arbolconduccion->nuevoNodoAccion(18,1); //21
	arbolconduccion->nuevoNodoAccion(19,2); //22

}

//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
void CorredorIA::movimiento()
{
	//LLmamos a la logica difusa para que nos de los valores de entrada
	logicaDifusa();

	bool arraybooleanos[16] = { 
	distanciaCerca,distanciaMedia,distanciaLejos, // del 0 al 2 distancias
	velocidadBaja,velocidadMedia,velocidadAlta, // del 3 al 5 velocidades
	noGiro,giroFlojoDerecha,giroFlojoIzquierda,giroFuerteDerecha,giroFuerteIzquierda, // del 6 al 10 conduccion
	caja,turbo,enemigo,Vision,Objeto}; // del 11 al 15 vision 
	
	

		

		switch(arbolconduccion->recorrerArbol(arraybooleanos,tipoObj)){ //lo que devuelva el arbol conduccion

        case NADA1:
        break;

        case ACELERAR:
		acelerar();
        break;

        case FRENAR:
        frenar();
        break;

        case GIRARDERECHA:
        girarDerecha();
        break;

        case GIRARIZQUIERDA:
        girarIzquierda();
        break;

        case GIRARFUERTEDERECHA:
        vehiculo->setSteeringValue(0.15, 0);
		vehiculo->setSteeringValue(0.15, 1);
        break;

        case GIRARFUERTEIZQUIERDA:
        vehiculo->setSteeringValue(0.-15, 0);
		vehiculo->setSteeringValue(0.-15, 1);
        break;


    }

/*
		switch(parametro){ //lo que devuelva el arbol de lanzar objeto
        case NADA1:
        break;

        case SEGUIRWAYPOINT:
		seguirWaypoint();
        break;

        case DISTANCIAOBJETIVOENEMIGO:
        calculoAnguloGiro(posicionEnemigo);
        break;

        case DISTANCIAOBJETIVOTURBO:
       	calculoAnguloGiro(posicionTurbo);
        break;

        case DISTANCIAOBJETIVOCAJA:
        calculoAnguloGiro(posicionCaja);
        break;

    }

	if(noGiro){
	acelerar();
	vehiculo->setSteeringValue(0, 0);
	vehiculo->setSteeringValue(0, 1);
	//frenodemano(false);
	}

	if(giroFlojoIzquierda){
	acelerar();
	girarIzquierda();
	//frenodemano(false);
	}
	

	if(giroFlojoDerecha){
	acelerar();
	girarDerecha();
	//frenodemano(false);
	}


	if(giroFuerteDerecha){
	
	if(velocidadBaja)
	acelerar();
	else if(velocidadMedia)
	acelerar();
	else if(velocidadMaxima)
	frenar();

	vehiculo->setSteeringValue(0.15, 0);
	vehiculo->setSteeringValue(0.15, 1);
	//frenodemano(true);

	}

	if(giroFuerteIzquierda){
	
	if(velocidadBaja)
	acelerar();
	else if(velocidadMedia)
	acelerar();
	else if(velocidadMaxima)
	frenar();

	vehiculo->setSteeringValue(-0.15, 0);
	vehiculo->setSteeringValue(-0.15, 1);
	//frenodemano(true);
	}

*/
	//objetos

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

	
	reposicionar();
}





void CorredorIA::seguirWaypoint(){

	btScalar distanciaCentro=0;
	btScalar distanciaLado1=0;
	btScalar distanciaLado2=0;

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
		if (abs(abs(posicion_aux.getX())-abs(posicion.getX()))<2 && abs(abs(posicion_aux.getZ())-abs(posicion.getZ()))<2){
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


	//DISTANCIA
	pertenenciaCerca = FuncionTrapezoidal(distanciaObjetivo, 0, 0, 1000, 3000);
	pertenenciaMedia = FuncionTrapezoidal(distanciaObjetivo, 2000, 4000, 5000, 6000);
	pertenenciaLejos = FuncionTrapezoidal(distanciaObjetivo, 5000, 8000, 9000, 100000);

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
					logicaDifusa();
					enemigo=true;
					turbo=false;
				}else{
					distanciaObjetivo = distanciaTurbo;
					logicaDifusa();
					enemigo=false;
					turbo=true;
				}
					Vision=true;
	}else{
					if(enemigo){			
					distanciaObjetivo = distanciaEnemigo;
					logicaDifusa();
					caja=false;
					turbo=false;
					enemigo=true;
					Vision=true;
					}else if(turbo){
					distanciaObjetivo = distanciaTurbo;
					logicaDifusa();
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
					logicaDifusa();
					caja=true;
					turbo=false;
					enemigo=false;
				}else{
					distanciaObjetivo = distanciaTurbo;
					logicaDifusa();
					caja=false;
					turbo=true;
					enemigo=false;
				}
				Vision=true;
		}else {
					if(caja){			
					distanciaObjetivo = distanciaCaja;
					logicaDifusa();
					caja=true;
					turbo=false;
					enemigo=false;
					Vision=true;
					}else if(turbo){
					distanciaObjetivo = distanciaTurbo;
					logicaDifusa();
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

