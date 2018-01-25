#include "CorredorIA.hpp"

CorredorIA::CorredorIA(stringw rutaObj, vector3df pos) : Corredor(rutaObj, pos)
{

	cuboNodo->setName("JugadorIA");
	caja = NULL;
	enemigo = NULL;
	turbo= NULL;
	distanciaCaja = 0;
	distanciaEnemigo = 0;
	distanciaTurbo = 0;
	
}

//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
void CorredorIA::movimiento()
{
	
	logicaDifusa();
	acelerar();
	
	//if(caja)
	//cout<<"VEO CAJA"<<endl;	

	//if(turbo)
	//cout<<"VEO TURBO"<<endl;	

	

	if(!caja || !turbo){
	//cout<< "sigoWAyPOINT"<< endl;
	calculoAnguloGiro(siguiente->getPosicion());
	
	}
	
	if(caja && turbo || caja && !turbo){
	//	cout<< "sigoCAJA"<< endl;
	calculoAnguloGiro(btVector3(caja->getPosition().X,caja->getPosition().Y,caja->getPosition().Z));	
	
	}
	
	if(turbo && !caja){
	//	cout<< "sigoTUrbo"<< endl;
	calculoAnguloGiro(btVector3(turbo->getPosition().X,turbo->getPosition().Y,turbo->getPosition().Z));	
	
	}


	if(noGiro){
	
	vehiculo->setSteeringValue(0, 0);
	vehiculo->setSteeringValue(0, 1);

	}

	if(giroFlojoDerecha || giroFuerteDerecha){
	//girarDerecha();
	
	vehiculo->setSteeringValue(0.1, 0);
	vehiculo->setSteeringValue(0.1, 1);
	
	}

	if(giroFlojoIzquierda || giroFuerteIzquierda){
	//girarIzquierda();
	
	vehiculo->setSteeringValue(-0.1, 0);
	vehiculo->setSteeringValue(-0.1, 1);

	}


}



void CorredorIA::calculoAnguloGiro(btVector3 posicion) {

	btVector3 orientacionCoche(orientacion.X,orientacion.Y,orientacion.Z);
	btVector3 direccion = btVector3(posicion.getX()-cuboNodo->getPosition().X,
	posicion.getY()-cuboNodo->getPosition().Y,
	posicion.getZ()-cuboNodo->getPosition().Z);
	
	direccion.normalize();
	anguloGiro = orientacionCoche.angle(direccion) *180/PI;
	
	btVector3 orientacionCocheGirada = orientacionCoche.rotate(btVector3(0, 1, 0),2*PI/180);

	btScalar angulo2 = orientacionCocheGirada.angle(direccion) *180/PI;
		
		if(angulo2>anguloGiro)
			anguloGiro = -anguloGiro;

	//cout<< "ANGULO:" << anguloGiro  << endl;
}


void CorredorIA::logicaDifusa() {
	//GIRO DEL COCHE
	pertenenciaCerca = FuncionTrapezoidal(distanciaWaypoint, 0, 0, 500, 1000);
	pertenenciaMedia = FuncionTrapezoidal(distanciaWaypoint, 1500, 4000, 6000, 7000);
	pertenenciaLejos = FuncionTrapezoidal(distanciaWaypoint, 6000, 7000, 9000, 100000);

		pertenenciaNoGiro= FuncionTriangular(anguloGiro,-8,0,8);
		pertenenciaGiroFlojoDerecha=FuncionTriangular(anguloGiro,9,40,90);
		pertenenciaGiroFuerteDerecha=FuncionTriangular(anguloGiro,91,140,180);
		pertenenciaGiroFuerteIzquierda=FuncionTriangular(anguloGiro,-91,-140,-180);
		pertenenciaGiroFlojoIzquierda=FuncionTriangular(anguloGiro,-9,-40,-90);

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

		if(pertenenciaNoGiro!=0){
			giroFuerteDerecha=false;
			giroFlojoDerecha=false;
			noGiro=true;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=false;
		}

		if(pertenenciaGiroFlojoDerecha!=0){
			giroFuerteDerecha=false;
			giroFlojoDerecha=true;
			noGiro=false;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=false;
		}
		if(pertenenciaGiroFuerteDerecha!=0){
			giroFuerteDerecha=true;
			giroFlojoDerecha=false;
			noGiro=false;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=false;
		}	

		if(pertenenciaGiroFlojoIzquierda!=0){
			giroFuerteDerecha=false;
			giroFlojoDerecha=false;
			noGiro=false;
			giroFuerteIzquierda=false;
			giroFlojoIzquierda=true;
		}
		if(pertenenciaGiroFuerteIzquierda!=0){
			giroFuerteDerecha=false;
			giroFlojoDerecha=false;
			noGiro=false;
			giroFuerteIzquierda=true;
			giroFlojoIzquierda=false;
		}	

		TextoPantalla * texto = TextoPantalla::getInstancia(); 
		texto->agregar("ACCION 1: "); 
		std::string agrega; 
		if(distanciaLejos)
   			agrega = "ACELERA A TOPE"; 	
		if(distanciaMedia)    
			agrega = "Reduce velocidad"; 
		if(distanciaCerca)
    		agrega = "Echa el freno fiera"; 
		
		texto->agregar(agrega+"\n"); 
		
		texto->agregar("ACCION 2: "); 
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

		texto->agregar(agrega+"\n"); 

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
	float distanciaRaycast = 100; // longitud del rayo
	float distanciaCoche = 10; // distancia entre el rayo y el coche, donde empieza
	float Raycast23 = 20; // distancia entre raycast 2 y 3
	float Raycast45 = 40; // distancia entre raycast 4 y 5

	// Raycast central1
	btVector3 inicio(cuboNodo->getPosition().X, cuboNodo->getPosition().Y + 1, cuboNodo->getPosition().Z);
	btVector3 fin(cuboNodo->getPosition().X + orientacion.X*distanciaRaycast, cuboNodo->getPosition().Y + 1, cuboNodo->getPosition().Z + orientacion.Z *distanciaRaycast);


	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast1(inicio, fin);
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast1.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast1);
	// Raycast central2 derecha
	//inicio = btVector3(3*orientacion.Z+cuboNodo->getPosition().X,cuboNodo->getPosition().Y+1,orientacion.X*-3+cuboNodo->getPosition().Z);
	fin = btVector3(Raycast23*orientacion.Z + cuboNodo->getPosition().X + orientacion.X*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.X*-Raycast23 + cuboNodo->getPosition().Z + orientacion.Z *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast2(inicio, fin);
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast2.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast2);


	// Raycast central3 izquierda
	//inicio = btVector3((cuboNodo->getPosition().X)+orientacion.X*distanciaCoche,cuboNodo->getPosition().Y+1,(-3+cuboNodo->getPosition().Z)+orientacion.Z*distanciaCoche);
	fin = btVector3(-Raycast23 * orientacion.Z + cuboNodo->getPosition().X + orientacion.X*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.X*Raycast23 + cuboNodo->getPosition().Z + orientacion.Z *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast3(inicio, fin);
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast3.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast3);


	// Raycast central4 derecha
	//inicio = btVector3((-7+cuboNodo->getPosition().X)+orientacion.X*distanciaCoche,cuboNodo->getPosition().Y+1,(-7+cuboNodo->getPosition().Z)+orientacion.Z*distanciaCoche);
	fin = btVector3(Raycast45*orientacion.Z + cuboNodo->getPosition().X + orientacion.X*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.X*-Raycast45 + cuboNodo->getPosition().Z + orientacion.Z *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast4(inicio, fin);
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast4.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast4);

	// Raycast central5 izquierda
	//inicio = btVector3((cuboNodo->getPosition().X+7)+orientacion.X,cuboNodo->getPosition().Y+1,(cuboNodo->getPosition().Z+7)+orientacion.Z*distanciaCoche);
	fin = btVector3(-Raycast45 * orientacion.Z + cuboNodo->getPosition().X + orientacion.X*distanciaRaycast, cuboNodo->getPosition().Y + 1, orientacion.X*Raycast45 + cuboNodo->getPosition().Z + orientacion.Z *distanciaRaycast);

	mundo->getDebugDrawer()->drawLine(inicio, fin, btVector4(0, 0, 1, 1));
	btCollisionWorld::AllHitsRayResultCallback RayCast5(inicio, fin);
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	RayCast5.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	mundo->rayTest(inicio, fin, RayCast5);

	turbo=NULL;
	distanciaTurbo=0;

	caja=NULL;
	distanciaCaja=0;


	if (RayCast1.hasHit())
	{

		for (int i=0;i<RayCast1.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast1.m_collisionObjects[i]->getUserPointer());
		if (Node) {
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 && Node->isVisible() || strcmp(Node->getName(),"Turbo")==0){
			vision(RayCast1.m_hitFractions[i],Node);
			//cout<<"RaycastCentral::"<<Node->getName()<<endl;
			//cout<<"Distancia::"<<RayCast1.m_hitFractions[i] << endl;
			//if(distanciaCaja<RayCast1.m_hitFractions[i])
			//distanciaCaja = RayCast1.m_hitFractions[i];
			}
		}
			}

	}
	if (RayCast2.hasHit()) {

		for (int i=0;i<RayCast2.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast2.m_collisionObjects[i]->getUserPointer());
		if (Node) {
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 && Node->isVisible() || strcmp(Node->getName(),"Turbo")==0){
			vision(RayCast2.m_hitFractions[i],Node);
			//cout<<"RaycastDerecha1::"<<Node->getName()<<endl;
			//cout<<"Distancia::"<<RayCast2.m_hitFractions[i] << endl;
			
			}
		}
			}

	}
	if (RayCast3.hasHit()) {

		for (int i=0;i<RayCast3.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast3.m_collisionObjects[i]->getUserPointer());
		if (Node) {
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 && Node->isVisible() || strcmp(Node->getName(),"Turbo")==0){
			vision(RayCast3.m_hitFractions[i],Node);
			//cout<<"RaycastIzquierda1::"<<Node->getName()<<endl;
			//cout<<"Distancia::"<<RayCast3.m_hitFractions[i] << endl;
			}
		}
			}
	}
	if (RayCast4.hasHit()) {

		for (int i=0;i<RayCast4.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast4.m_collisionObjects[i]->getUserPointer());
		if (Node) {
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 && Node->isVisible() || strcmp(Node->getName(),"Turbo")==0){
			vision(RayCast4.m_hitFractions[i],Node);
			//cout<<"RaycastDerecha2::"<<Node->getName()<<endl;
			//cout<<"Distancia::"<<RayCast4.m_hitFractions[i] << endl;
			}
		}
			}
	}

	if (RayCast5.hasHit()) {
		for (int i=0;i<RayCast5.m_hitFractions.size();i++)
			{
		ISceneNode *Node = static_cast<ISceneNode *>(RayCast5.m_collisionObjects[i]->getUserPointer());
		if (Node) {
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 && Node->isVisible() || strcmp(Node->getName(),"Turbo")==0){
			
			vision(RayCast5.m_hitFractions[i],Node);
			//cout<<"RaycastIzquierda2::"<<Node->getName()<<endl;
			//cout<<"Distancia::"<<RayCast5.m_hitFractions[i] << endl;
			}
		}
			}
	}



}



void CorredorIA::vision(btScalar distancia,ISceneNode *nodo){

	if(strcmp(nodo->getName(),"Caja")==0){

		if(!caja)
		caja=nodo;

	}

	if(strcmp(nodo->getName(),"Turbo")==0){

		if(!turbo)
		turbo=nodo;
		
	}


}

