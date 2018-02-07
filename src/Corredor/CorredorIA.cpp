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
}

//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
void CorredorIA::movimiento()
{
	//LLmamos a la logica difusa para que nos de los valores de entrada
	logicaDifusa();
	acelerarGirar();
	

	
	//si no veo nada con el raycast sigo los waypoints	
	if(!caja && !turbo && !enemigo){
	//cout<< "sigoWAyPOINT"<< endl;
	seguirWaypoint();
	
	}
	


	if(tipoObj == 0){
		//llamar a vision
		visionArbol();
	}else{

		if(tipoObj == 1 || tipoObj == 6){ // si es un proyectil
			//llamar a vision
			visionArbol();
		}else{

			if(tipoObj == 3 || tipoObj == 7){ // si es un turbo

				tipoObj=0;
				
			}else{
				usarObjetos();

			}

		}
	}


	/*
	//COJO LA caja mas cercana porq la veo
	if(caja && turbo || caja && !turbo){
    //cout<< "sigoCAJA"<< endl;
	if(getDistanciaPunto(btVector3(caja->getPosition().X,caja->getPosition().Y,caja->getPosition().Z))<4000)
    calculoAnguloGiro(btVector3(caja->getPosition().X,caja->getPosition().Y,caja->getPosition().Z));

	if(getDistanciaPunto(btVector3(caja->getPosition().X,caja->getPosition().Y,caja->getPosition().Z))<50){
		if(tipoObj==2 || tipoObj==4) // si son cajas falsas lo mejor es ponerlas junto a las de verdad
		usarObjetos();	
	}

    }

	//cojo turbo
	if(turbo && !caja){
	//cout<< "sigoTUrbo"<< endl;
	if(getDistanciaPunto(btVector3(turbo->getPosition().X,turbo->getPosition().Y,turbo->getPosition().Z))<2500)
	calculoAnguloGiro(btVector3(turbo->getPosition().X,turbo->getPosition().Y,turbo->getPosition().Z));	
	
	}


	//lanzamiento de objetos sin necesidad de ver al jugador
	if(tipoObj!=0){
		
		if(tipoObj==3)// falta condicion para usarlo en rectas 
		tipoObj=0;
		if(tipoObj==5)//el escudo se usa al instante
		usarObjetos();
		if(tipoObj==7)//el escudo se usa al instante
		usarObjetos();
		if(tipoObj==8)// poner condicion para que lo lance en rectas y a parte no los tire todos seguidos
		tipoObj=0;
		if(tipoObj==9)
		tipoObj==0;
	
	}

	//veo al enemigo necesito orientamer a el para lanzarle proyectiles no dirigidos
	if(enemigo){
	//cout<<"veo enemigo"<< endl;	
		if(tipoObj==1 || tipoObj == 6){
			//hay que orientarse al jugador, tener en cuenta su distancia se puede usar logica difusa para esot pero de momento no
			if(getDistanciaPunto(btVector3(enemigo->getPosition().X,enemigo->getPosition().Y,enemigo->getPosition().Z))<3000){	
			calculoAnguloGiro(btVector3(enemigo->getPosition().X,enemigo->getPosition().Y,enemigo->getPosition().Z));
			
			if(noGiro)	
			usarObjetos();

			}
			//cout<<"disparo"<<endl;		
		}else{
		// no tengo objeto pero alomejor me interesa esquivarlo	

		}
	}


	*/
	
	

}

void CorredorIA::visionArbol(){

	if(!enemigo && !caja && !turbo){

		seguirWaypoint();

	}else{

		if(pertenenciaCerca){

			if(caja){

				if(tipoObj!=0){

					if(tipoObj == 2 || tipoObj == 4){

						usarObjetos();

					}else{
					
						seguirWaypoint();
					
					}

				}else{
					
					calculoAnguloGiro(posicionCaja);

				}

			}else if(enemigo){

				if(tipoObj!=0){

					if(tipoObj == 1 || tipoObj == 6){

						calculoAnguloGiro(posicionEnemigo);
						if(noGiro)
						usarObjetos();
							
					}else{
						//condicion para adelantar
						seguirWaypoint();
					
					}

				}else{
					//condicion para adelantar
					seguirWaypoint();
					
				}

			}else if(turbo){

				calculoAnguloGiro(posicionTurbo);
			}


		}else if(pertenenciaMedia){

			if(caja){

				if(tipoObj!=0){

						seguirWaypoint();

				}else{
					
					calculoAnguloGiro(posicionCaja);

				}

			}else if(enemigo){

				if(tipoObj!=0){

					if(tipoObj == 1 || tipoObj == 6){

						calculoAnguloGiro(posicionEnemigo);
						if(noGiro)
						usarObjetos();
							
					}else{
						//condicion para adelantar
						seguirWaypoint();
					
					}

				}else{
					//condicion para adelantar
					seguirWaypoint();
					
				}

			}else if(turbo){

				calculoAnguloGiro(posicionTurbo);
			}

		}else if(pertenenciaLejos){

			seguirWaypoint();

		}

	}

}


void CorredorIA::acelerarGirar(){

	acelerar();

	if(noGiro){
	
	vehiculo->setSteeringValue(0, 0);
	vehiculo->setSteeringValue(0, 1);
	//frenodemano(false);
	}

	if(giroFlojoDerecha){
	girarDerecha();
	//frenodemano(false);
	//vehiculo->setSteeringValue(0.09, 0);
	//vehiculo->setSteeringValue(0.09, 1);

	}

	if(giroFuerteDerecha){
	//girarDerecha();
	//frenar();
	vehiculo->setSteeringValue(0.15, 0);
	vehiculo->setSteeringValue(0.15, 1);
	//frenodemano(true);

	}

	if(giroFlojoIzquierda){
	girarIzquierda();
	//frenodemano(true);
	//vehiculo->setSteeringValue(-0.09, 0);
	//vehiculo->setSteeringValue(-0.09, 1);
	//frenodemano(false);
	}

	if(giroFuerteIzquierda){
	//girarIzquierda();
	//frenar();
	vehiculo->setSteeringValue(-0.15, 0);
	vehiculo->setSteeringValue(-0.15, 1);
	//frenodemano(true);
	
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

	//cout<< "ANGULO:" << anguloGiro  << endl;
}


void CorredorIA::logicaDifusa() {
	
	//DISTANCIA
	pertenenciaCerca = FuncionTrapezoidal(distanciaObjetivo, 0, 0, 500, 2000);
	pertenenciaMedia = FuncionTrapezoidal(distanciaObjetivo, 1500, 2500, 3500, 4500);
	pertenenciaLejos = FuncionTrapezoidal(distanciaObjetivo, 4000, 6000, 8000, 100000);

	//GIRO
		pertenenciaNoGiro= FuncionTriangular(anguloGiro,-40,0,40);
		pertenenciaGiroFlojoDerecha=FuncionTriangular(anguloGiro,5,20,35);
		pertenenciaGiroFlojoIzquierda=FuncionTriangular(anguloGiro,-5,-20,-35);
		pertenenciaGiroFuerteDerecha=FuncionTriangular(anguloGiro,30,80,180);
		pertenenciaGiroFuerteIzquierda=FuncionTriangular(anguloGiro,-30,-80,-180);
		

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
	float Raycast23 = 25; // distancia entre raycast 2 y 3
	float Raycast45 = 35; // distancia entre raycast 4 y 5

	// Raycast central1
	btVector3 inicio(cuboNodo->getPosition().X + orientacion.getX()*distanciaCoche, cuboNodo->getPosition().Y , cuboNodo->getPosition().Z+ orientacion.getZ()*distanciaCoche);
	btVector3 fin(cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y + 1, cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);


	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);
	// Raycast central2 derecha
	//inicio = btVector3(3*orientacion.Z+cuboNodo->getPosition().X,cuboNodo->getPosition().Y+1,orientacion.X*-3+cuboNodo->getPosition().Z);
	fin = btVector3(Raycast23*orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.getX()*-Raycast23 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast2(inicio, fin);
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast2);


	// Raycast central3 izquierda
	//inicio = btVector3((cuboNodo->getPosition().X)+orientacion.X*distanciaCoche,cuboNodo->getPosition().Y+1,(-3+cuboNodo->getPosition().Z)+orientacion.Z*distanciaCoche);
	fin = btVector3(-Raycast23 * orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.getX()*Raycast23 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast3(inicio, fin);
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast3);


	// Raycast central4 derecha
	//inicio = btVector3((-7+cuboNodo->getPosition().X)+orientacion.X*distanciaCoche,cuboNodo->getPosition().Y+1,(-7+cuboNodo->getPosition().Z)+orientacion.Z*distanciaCoche);
	fin = btVector3(Raycast45*orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.getX()*-Raycast45 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast4(inicio, fin);
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast4);

	// Raycast central5 izquierda
	//inicio = btVector3((cuboNodo->getPosition().X+7)+orientacion.X,cuboNodo->getPosition().Y+1,(cuboNodo->getPosition().Z+7)+orientacion.Z*distanciaCoche);
	fin = btVector3(-Raycast45 * orientacion.getZ() + cuboNodo->getPosition().X + orientacion.getX()*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.getX()*Raycast45 + cuboNodo->getPosition().Z + orientacion.getZ() *distanciaRaycast);

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
			if(strcmp(Node->getName(),"Mapa")==0 && RayCast2.m_hitFractions[i]<0.7){
				
				calculoAnguloGiro(siguiente->getPosicion());
				//posicionObjetivo= siguiente->getPosicion();
			}
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
			if(strcmp(Node->getName(),"Mapa")==0 && RayCast3.m_hitFractions[i]<0.7){
				
				calculoAnguloGiro(siguiente->getPosicion());
				//posicionObjetivo= siguiente->getPosicion();
			}
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
			if(strcmp(Node->getName(),"Mapa")==0 && RayCast4.m_hitFractions[i]<0.7){
				
				calculoAnguloGiro(siguiente->getPosicion());
				//posicionObjetivo= siguiente->getPosicion();
			}
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
			if(strcmp(Node->getName(),"Mapa")==0 && RayCast5.m_hitFractions[i]<0.7){
				
				calculoAnguloGiro(siguiente->getPosicion());
				//sposicionObjetivo= siguiente->getPosicion();
			}
			if(strcmp(Node->getName(),"Caja")==0 && Node->isVisible() 
			|| strcmp(Node->getName(),"Turbo")==0 
			|| strcmp(Node->getName(),"Jugador")==0
			|| strcmp(Node->getName(),"JugadorIA")==0){

			vision(RayCast5.m_hitFractions[i],Node);
			
			}
		}
			}
	}


	int distaux1=0;
	int distaux2=0;

	if((caja && enemigo && turbo) && tipoObj==0){
		distanciaObjetivo = getDistanciaPunto(posicionCaja);
	}else if(((!caja && enemigo && turbo) && tipoObj!=0) || ((caja && enemigo && turbo) && tipoObj!=0)){
		distaux1=getDistanciaPunto(posicionEnemigo);
		distaux2=getDistanciaPunto(posicionTurbo);
			if(distaux1<distaux2){
				distanciaObjetivo = getDistanciaPunto(posicionEnemigo);
			}else{
				distanciaObjetivo = getDistanciaPunto(posicionTurbo);
			}	
		
	}else if(!caja && !turbo && enemigo){
		distanciaObjetivo = getDistanciaPunto(posicionEnemigo);
	}

}



void CorredorIA::vision(btScalar distancia,ISceneNode *nodo){



	if(strcmp(nodo->getName(),"Caja")==0){


		if(distanciaCaja==0){
		distanciaCaja=distancia;
		caja=true;
		posicionCaja = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaObjetivo = getDistanciaPunto(posicionCaja);
		}

		if(distancia<distanciaCaja){
		distanciaCaja=distancia;
		caja=true;
		posicionCaja = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaObjetivo = getDistanciaPunto(posicionCaja);
		}

	}
	

	if((strcmp(nodo->getName(),"Jugador")==0
	|| strcmp(nodo->getName(),"JugadorIA")==0)){

		if(distanciaEnemigo==0){
		distanciaEnemigo=distancia;
		enemigo=true;
		posicionEnemigo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaObjetivo = getDistanciaPunto(posicionEnemigo);
		}

		if(distancia<distanciaEnemigo){
		distanciaEnemigo=distancia;
		enemigo=true;
		posicionEnemigo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);	
		distanciaObjetivo = getDistanciaPunto(posicionEnemigo);
		}
	}


	if(strcmp(nodo->getName(),"Turbo")==0){

		if(distanciaTurbo==0){
		distanciaTurbo=distancia;
		turbo=true;
		posicionTurbo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaObjetivo = getDistanciaPunto(posicionTurbo);
		}

		if(distancia<distanciaTurbo){
		distanciaTurbo=distancia;
		turbo=true;
		posicionTurbo = btVector3(nodo->getPosition().X,nodo->getPosition().Y,nodo->getPosition().Z);
		distanciaObjetivo = getDistanciaPunto(posicionTurbo);	
		}
	}

	



}

