#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include <string>
#include <time.h>
#include "Pista.hpp"

//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*
Corredor::Corredor(stringw rutaObj, vector3df pos)
{
	cargador = 0;
	tipoObj = 0;
	turboActivado = false;
	timerTurbo = 0;
	Motor3d *m = Motor3d::getInstancia();
	ISceneManager *smgr = m->getScene();
	coche = smgr->getMesh(rutaObj);
	cuboNodo = smgr->addMeshSceneNode(coche, 0);
	//cuboNodo = smgr->addCubeSceneNode(1.5f);
	if (cuboNodo) {
		cuboNodo->setName("Jugador");
		//cambiar a color rojo del coche
		smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
		cuboNodo->setScale(vector3df(1, 1, 1.5));
		// Desactivar la iluminacion del cubo
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(pos);
		cuboNodo->setRotation(vector3df(0.0f, 90.0f, 0.0f));
	}

	estado = new EstadosJugador();
	vueltas = 1;

	posicion.setX(pos.X);
	posicion.setY(pos.Y);
	posicion.setZ(pos.Z);
	//escudo
	escudo = new Escudo(pos, getNodo());
	setProteccion(false);
	//-------------bullet----------------
	rueda1 = smgr->addCubeSceneNode(1.f);
	rueda2 = smgr->addCubeSceneNode(1.f);
	rueda3 = smgr->addCubeSceneNode(1.f);
	rueda4 = smgr->addCubeSceneNode(1.f);

	//establecemos el primer waypoint del mapa
	Pista *mapa = Pista::getInstancia();
	actual = mapa->getArrayWaypoints()[0];
	siguiente = actual->getNextWaypoint();
	caja = NULL;
	enemigo = NULL;
	turbo= NULL;
	distanciaCaja = 0;
	distanciaEnemigo = 0;
	distanciaTurbo = 0;

	//smgr->getMeshManipulator()->setVertexColors(rueda1->getMesh(),SColor(255, 255, 0, 0));
	rueda1->setMaterialFlag(EMF_LIGHTING, false);
	rueda2->setMaterialFlag(EMF_LIGHTING, false);
	rueda3->setMaterialFlag(EMF_LIGHTING, false);
	rueda4->setMaterialFlag(EMF_LIGHTING, false);

	direccionRuedas = btVector3(0, -1, 0);
	rotacionRuedas = btVector3(-1, 0, 0);
	suspension = btScalar(0.9); // Este valor tiene que ser ese... sino peta
	FuerzaMaxima = btScalar(8000);
	Fuerza = FuerzaMaxima;
	anchoRueda = btScalar(0.4);			  //0.4
	radioRueda = btScalar(0.5);			  //No menor de 0.4 sino ni se mueve (ruedas pequenyas)
	alturaConexionChasis = btScalar(1.2); //influye mucho en la acceleracion de salida
	Masa = btScalar(800);
	FuerzaFrenado = btScalar(-10000);
	FuerzaGiro = btScalar(0.1); //manejo a la hora de girar
	FuerzaFrenoMano = btScalar(800);
	FuerzaFrenadoReposo = btScalar(60);

	if (cuboNodo)
		InicializarFisicas();
}

void Corredor::InicializarFisicas()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();

	//posicion inicial
	btTransform transCoche;
	transCoche.setIdentity();
	btVector3 posTransCoche = btVector3(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	transCoche.setOrigin(posTransCoche);
	btQuaternion quaternion;
	quaternion.setEulerZYX(cuboNodo->getRotation().Z* PI / 180, cuboNodo->getRotation().Y * PI / 180, cuboNodo->getRotation().X* PI / 180);
	transCoche.setRotation(quaternion);

	//Motionstate
	motionStateCoche = new btDefaultMotionState(transCoche); //motionState = interpolacion

	//establecemos su centro de gravedad
	btTransform localTransform;
	localTransform.setIdentity();
	localTransform.setOrigin(btVector3(0, 1.7, 0));
	CentroGravedad = new btCompoundShape();

	//Forma Colision
	btVector3 TamanyoFormaColision(cuboNodo->getScale().X, cuboNodo->getScale().Y, cuboNodo->getScale().Z);
	//btVector3 TamanyoFormaColision(1,btScalar(0.5),2);
	FormaColision = new btBoxShape(TamanyoFormaColision);
	//masa coche
	btVector3 Inercia(0, 0, 0);
	FormaColision->calculateLocalInertia(Masa, Inercia);

	CentroGravedad->addChildShape(localTransform, FormaColision);

	//rigidbody del coche
	CuerpoColisionChasis = new btRigidBody(Masa, motionStateCoche, CentroGravedad, Inercia);

	CuerpoColisionChasis->setUserPointer((void *)(cuboNodo));

	//RaycastDel Coche
	RayCastVehiculo = new btDefaultVehicleRaycaster(mundo);
	btRaycastVehicle::btVehicleTuning tuning;

	vehiculo = new btRaycastVehicle(tuning, CuerpoColisionChasis, RayCastVehiculo);

	CuerpoColisionChasis->setActivationState(DISABLE_DEACTIVATION);
	mundo->addVehicle(vehiculo);
	//vehiculo->setActivationState(DISABLE_DEACTIVATION);
	//almacenar el puntero al nodo irrlicht  para poder actualizar sus valores

	objetos.push_back(CuerpoColisionChasis);

	CrearRuedas(vehiculo, tuning);
	mundo->addRigidBody(CuerpoColisionChasis);
	bullet->setObjetos(objetos);
	//rigidBody->applyGravity();

	// luego declaramos sus ruedas

	// inicializamos la posicion de las ruedas

}

void Corredor::setPosicion(float *pos, float *ori){
	btVector3 btPos(pos[0], pos[1], pos[2]);

	btTransform trans;
	trans.setOrigin(btPos);
	btQuaternion quaternion;
	quaternion.setEulerZYX(ori[0]* PI / 180, ori[1] * PI / 180, ori[2] * PI / 180);
	trans.setRotation(quaternion);

	CuerpoColisionChasis->setCenterOfMassTransform(trans);

}

void Corredor::BorrarFisicas()
{
}

void Corredor::CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning)
{

	//btVector3 TamanyoFormaColision(1,btScalar(0.5),2);
	btVector3 puntoConexionChasis(cuboNodo->getScale().X - radioRueda, alturaConexionChasis, cuboNodo->getScale().Z - anchoRueda);

	vehiculo->setCoordinateSystem(0, 1, 2); // 0, 1, 2

	// Agrega las ruedas delanteras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, 2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, 2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	// Agrega las ruedas traseras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, -2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);

	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, -2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);

	for (int i = 0; i < vehiculo->getNumWheels(); i++)
	{
		btWheelInfo &wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = 40;    //tambaleo de las ruedas (se mueve como si fuera por terreno con baches). A mayor valor mayor tambaleo 
		wheel.m_wheelsDampingCompression = 2.4f;//btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f; 
		wheel.m_wheelsDampingRelaxation = 2.3f;  //btScalar(0.5)*2*btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f; 
		wheel.m_frictionSlip = btScalar(10000);  //100;  //conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover 
		wheel.m_rollInfluence = 0.03;       //0.1f;  //Empieza a rodar muy loco, si el valor es alto 
		//wheel.m_maxSuspensionForce = 40000.f;  //A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa) 
		wheel.m_maxSuspensionTravelCm = 800.f; //Nose muy bien que funcion tiene, pero si el valor es muy bajo el coche no avanza 


		/*
	 * PARAMETROS EN RUEDAS DISPONIBLES
	 * wheel.m_chassisConnectionCS = connectionPointCS;
	 * wheel.m_wheelDirectionCS = wheelDirectionCS0;
	 * wheel.m_wheelAxleCS = wheelAxleCS;
	 * wheel.m_suspensionRestLength = suspensionRestLength;
	 * wheel.m_wheelRadius = wheelRadius;
	 * wheel.m_suspensionStiffness = tuning.m_suspensionStiffness;
	 * wheel.m_wheelsDampingCompression = tuning.m_suspensionCompression;
	 * wheel.m_wheelsDampingRelaxation = tuning.m_suspensionDamping;
	 * wheel.m_frictionSlip = tuning.m_frictionSlip;
	 * wheel.m_bIsFrontWheel = isFrontWheel;
	 * wheel.m_maxSuspensionTravelCm = tuning.m_maxSuspensionTravelCm;
	 * wheel.m_maxSuspensionForce = tuning.m_maxSuspensionForce;
	*/
	}
}

void Corredor::actualizarRuedas()
{
	btTransform ruedas = vehiculo->getWheelTransformWS(0);

	rueda1->setPosition(vector3df(ruedas.getOrigin().getX(), ruedas.getOrigin().getY(), ruedas.getOrigin().getZ()));
	rueda1->setRotation(vector3df(ruedas.getRotation().getX(), ruedas.getRotation().getY(), ruedas.getRotation().getZ()));
	ruedas = vehiculo->getWheelTransformWS(1);

	rueda2->setPosition(vector3df(ruedas.getOrigin().getX(), ruedas.getOrigin().getY(), ruedas.getOrigin().getZ()));
	rueda2->setRotation(vector3df(ruedas.getRotation().getX(), ruedas.getRotation().getY(), ruedas.getRotation().getZ()));
	ruedas = vehiculo->getWheelTransformWS(2);

	rueda3->setPosition(vector3df(ruedas.getOrigin().getX(), ruedas.getOrigin().getY(), ruedas.getOrigin().getZ()));
	rueda3->setRotation(vector3df(ruedas.getRotation().getX(), ruedas.getRotation().getY(), ruedas.getRotation().getZ()));
	ruedas = vehiculo->getWheelTransformWS(3);

	rueda4->setPosition(vector3df(ruedas.getOrigin().getX(), ruedas.getOrigin().getY(), ruedas.getOrigin().getZ()));
	rueda4->setRotation(vector3df(ruedas.getRotation().getX(), ruedas.getRotation().getY(), ruedas.getRotation().getZ()));
}

int Corredor::getCargador() { return cargador; };
int Corredor::getTipoObj() { return tipoObj; };
void Corredor::incCargador() { cargador++; };
void Corredor::decCargador() { cargador--; };

void Corredor::ActualizarRaytest() {
	MotorFisicas *mun = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = mun->getMundo();
	mundo->updateAabbs();
	mundo->computeOverlappingPairs();

	//cout<< orientacion.X << "   "<< orientacion.Z<<endl;
	float distanciaRaycast = 100; // longitud del rayo
	float distanciaCoche = 2; // distancia entre el rayo y el coche, donde empieza
	float Raycast23 = 10; // distancia entre raycast 2 y 3
	float Raycast45 = 20; // distancia entre raycast 4 y 5

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
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 || strcmp(Node->getName(),"Turbo")==0){
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
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 || strcmp(Node->getName(),"Turbo")==0){
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
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 || strcmp(Node->getName(),"Turbo")==0){
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
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 || strcmp(Node->getName(),"Turbo")==0){
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
			if(strcmp(Node->getName(),"Caja")==0 && tipoObj == 0 || strcmp(Node->getName(),"Turbo")==0){
			
			vision(RayCast5.m_hitFractions[i],Node);
			//cout<<"RaycastIzquierda2::"<<Node->getName()<<endl;
			//cout<<"Distancia::"<<RayCast5.m_hitFractions[i] << endl;
			}
		}
			}
	}



}


void Corredor::vision(btScalar distancia,ISceneNode *nodo){

	if(strcmp(nodo->getName(),"Caja")==0){

		if(!caja)
		caja=nodo;

	}

	if(strcmp(nodo->getName(),"Turbo")==0){

		if(!turbo)
		turbo=nodo;
		
	}


}

void Corredor::calculoDistanciaPunto() {

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(siguiente->getPosicion().getX(), siguiente->getPosicion().getY(), siguiente->getPosicion().getZ());

	distanciaWaypoint = posCoche.distance2(posWaypoint);

	//calulamos la distancia hasta el waypoint 
	//cout << "WAYPOINT ACTUAL:" <<actual->getWaypoint()->getID() << endl;
	//cout << "WAYPOINT SIGUIENTE:" << siguiente->getWaypoint()->getID() << endl;
	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("DISTANCIA: ");
	texto->agregar(to_string(distanciaWaypoint)+"\n");

	texto->agregar("WAYPOINT ACTUAL: ");
	texto->agregar(to_string( actual->getWaypoint()->getID()-6)+"\n");
	texto->agregar("WAYPOINT SIGUIENTE: ");
	texto->agregar(to_string( siguiente->getWaypoint()->getID()-6)+"\n");
	texto->agregar("VUELTAS: ");
	texto->agregar(to_string(vueltas)+"\n");
	

}

void Corredor::calculoDistanciaPuntoActual() {

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(actual->getPosicion().getX(), actual->getPosicion().getY(), actual->getPosicion().getZ());

	distanciaWaypointActual = posCoche.distance2(posWaypoint);

	//calulamos la distancia hasta el waypoint 
	//cout << "WAYPOINT ACTUAL:" <<actual->getWaypoint()->getID() << endl;
	//cout << "WAYPOINT SIGUIENTE:" << siguiente->getWaypoint()->getID() << endl;
	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("DISTANCIA ACTUAL: ");
	texto->agregar(to_string(distanciaWaypointActual)+"\n");

}

void Corredor::calculoAnguloGiro(btVector3 posicion) {

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

void Corredor::setWaypointActual(ISceneNode *nodo)
{
	//de momento lo pongo así, no da la segunda vuelta pero habria que mirar cuales se han visitado y cuales no
	//mas adelante se ve, Las IDS no funcionan bien tengo que preguntarle a santi a si que de momento lo comento para que
	//se puedan coger las cajas.
	
	if(nodo->getID() != actual->getWaypoint()->getID()){
		if(nodo->getID() != siguiente->getWaypoint()->getID()){
			//cout<<"TE HAS SALTADO EL WAYPOINT: "<<siguiente->getWaypoint()->getID()<<endl;
		}else{
			if (siguiente->getWaypoint()->getID()-6==0){
				vueltas++;
				//cout<<"--------------"<<vueltas<<" VUELTA SUPERADA-------------: "<<endl;

			}
			actual = actual->getNextWaypoint();
			siguiente = actual->getNextWaypoint();
		

			
			//cout<<"HAS PASADO POR EL WAYPOINT: "<<actual->getWaypoint()->getID()<<" SIGUIENTE WAYPOINT: "<<siguiente->getWaypoint()->getID()<<endl;
		}
		
	}else{
		//cout<<"ESTAS EN EL WAYPOINTACTUAL: "<<actual->getWaypoint()->getID()<<endl;
	}

	//cout<< "NODO ACTUAL:"<< actual->getWaypoint()->getID() <<endl;
	//cout<< "NODO SIGUIENTE:"<< siguiente->getWaypoint()->getID() <<endl;
}


void Corredor::logicaDifusa() {
	//GIRO DEL COCHE
	pertenenciaCerca = FuncionTrapezoidal(distanciaWaypoint, 0, 0, 500, 1000);
	pertenenciaMedia = FuncionTrapezoidal(distanciaWaypoint, 1500, 4000, 6000, 7000);
	pertenenciaLejos = FuncionTrapezoidal(distanciaWaypoint, 6000, 7000, 9000, 100000);

		pertenenciaNoGiro= FuncionTriangular(anguloGiro,-10,0,10);
		pertenenciaGiroFlojoDerecha=FuncionTriangular(anguloGiro,11,40,90);
		pertenenciaGiroFuerteDerecha=FuncionTriangular(anguloGiro,91,140,180);
		pertenenciaGiroFuerteIzquierda=FuncionTriangular(anguloGiro,-91,-140,-180);
		pertenenciaGiroFlojoIzquierda=FuncionTriangular(anguloGiro,-11,-40,-90);

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


void Corredor::movimientoIA(){
	
	calculoDistanciaPunto();
	//calculoAnguloGiro();
	logicaDifusa();
	acelerar();

	if(caja)
	cout<<"VEO CAJA"<<endl;	

	if(turbo)
	cout<<"VEO TURBO"<<endl;	

	if(!caja || !turbo){
	calculoAnguloGiro(siguiente->getPosicion());
	
	}
	
	if(caja && turbo || caja && !turbo){

	calculoAnguloGiro(btVector3(caja->getPosition().X,caja->getPosition().Y,caja->getPosition().Z));	
	
	}
	
	if(turbo && !caja){

	calculoAnguloGiro(btVector3(turbo->getPosition().X,turbo->getPosition().Y,turbo->getPosition().Z));	
	
	}


	if(giroFlojoDerecha || giroFuerteDerecha)
	girarDerecha();
	else if(giroFlojoIzquierda || giroFuerteIzquierda)
	girarIzquierda();

	//FuerzaMaxima


}

double Corredor::FuncionTrapezoidal(double valor,double a,double b,double c, double d){

	double resultado = 0;

	if (a <= valor && valor < b)
		resultado = (valor - a) / (b - a);
	else if (b <= valor && valor <= c)
		resultado = 1;
	else if (c < valor && valor <= d)
		resultado = (d - valor) / (d - c);

	return resultado;

}

double Corredor::FuncionTriangular(double valor, double a, double b, double c) {

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

void Corredor::setTipoObj()
{
	srand(time(NULL));
	tipoObj = rand() % 4 + 1;
	//cout << "Random ------>" << tipoObj << endl;
}

void Corredor::setTipoObj(int i)
{
	tipoObj = i;
	//cout << "Random ------>" << tipoObj << endl;
}

/*
	Recibe un item de jugadorCorredor o jugadorIA y le aplica una velocidad para lanzarlo
	direccionItem: 1=Delante
				  -1=Atras
*/
void Corredor::lanzarItem(Proyectil *item, int direccionItem)
{

	btRigidBody *rigidItem = item->inicializarFisicas();
	if (direccionItem == 1)
		item->getRigidBody()->setLinearVelocity(btVector3(orientacion.X * 100, 5.0f, orientacion.Z * 100));
	else if (direccionItem == -1)
		item->getRigidBody()->setLinearVelocity(btVector3(-orientacion.X * 100, 5.0f, -orientacion.Z * 100));
	//std::cout << "Disparo " << std::endl;
	tipoObj = 0;
	decCargador();
}

void Corredor::soltarItem()
{
	tipoObj = 0;
}

void Corredor::setTurbo(bool activo, bool objeto,int valor) {
	turboActivado = activo;
	if (activo) {
		Motor3d *m = Motor3d::getInstancia();
		SetFuerzaVelocidad(valor);
		acelerar();
		Timer *time = Timer::getInstancia();
		timerTurbo = time->getTimer();
		if (objeto) tipoObj = 0;
	}
	else {
		SetFuerzaVelocidad(1000);
	}
}

bool Corredor::getTurbo() {
	return turboActivado;
}

void Corredor::SetFuerzaVelocidad(int turbo)
{
	Fuerza = turbo;
}
void Corredor::acelerar()
{
	if (vehiculo->getCurrentSpeedKmHour()>300 && !turboActivado){
		if (vehiculo->getCurrentSpeedKmHour()>350){
			Fuerza =btScalar(0);
		}else{
			Fuerza =btScalar(200);		//limitador de velocidad
		}	
		
	}
	if (vehiculo->getCurrentSpeedKmHour()<370){
		vehiculo->applyEngineForce(Fuerza, 0);
		vehiculo->applyEngineForce(Fuerza, 1);
		vehiculo->applyEngineForce(Fuerza, 2);
		vehiculo->applyEngineForce(Fuerza, 3);
		vehiculo->setSteeringValue(btScalar(0), 0);
		vehiculo->setSteeringValue(btScalar(0), 1);
	}else{
		vehiculo->applyEngineForce(600, 0);
		vehiculo->applyEngineForce(200, 1);
		vehiculo->applyEngineForce(200, 2);
		vehiculo->applyEngineForce(200, 3);
	}
	estado->setEstadoMovimiento(AVANZA);
	estado->setDireccionMovimiento(RECTO);
}

void Corredor::frenar()
{
	if (vehiculo->getCurrentSpeedKmHour() > 1) {
		FuerzaFrenado = btScalar(-9000);		//freno potente si esta en marcha el coche
	}
	else {
		if (vehiculo->getCurrentSpeedKmHour() > -200) {
			FuerzaFrenado = btScalar(-2500);	//sino, marcha atras lenta
		}
		else {
			FuerzaFrenado = btScalar(0);	//limitador de velocidad al frenar
		}
	}
	vehiculo->applyEngineForce(FuerzaFrenado, 0);
	vehiculo->applyEngineForce(FuerzaFrenado, 1);
	vehiculo->applyEngineForce(FuerzaFrenado, 2);
	vehiculo->applyEngineForce(FuerzaFrenado, 3);
	vehiculo->setSteeringValue(btScalar(0), 0);
	vehiculo->setSteeringValue(btScalar(0), 1);
	if (vehiculo->getCurrentSpeedKmHour()<0){
		estado->setEstadoMovimiento(MARCHA_ATRAS);
	}else{
		estado->setEstadoMovimiento(FRENA);
	}
}

void Corredor::girarDerecha()
{
	estado->setDireccionMovimiento(DERECHA);
	vehiculo->setSteeringValue(FuerzaGiro, 0);
	vehiculo->setSteeringValue(FuerzaGiro, 1);
}

void Corredor::girarIzquierda()
{
	estado->setDireccionMovimiento(IZQUIERDA);
	vehiculo->setSteeringValue(-FuerzaGiro, 0);
	vehiculo->setSteeringValue(-FuerzaGiro, 1);
}
void Corredor::frenodemano(bool activo)
{
	int friccion = 2.0f;
	if (activo) {
		estado->setEstadoMovimiento(DERRAPA);
		FuerzaGiro = btScalar(0.36);

		vehiculo->getWheelInfo(0).m_frictionSlip = btScalar(friccion + 0.5);
		vehiculo->getWheelInfo(1).m_frictionSlip = btScalar(friccion + 0.5);

		vehiculo->getWheelInfo(2).m_frictionSlip = btScalar(friccion);
		vehiculo->getWheelInfo(3).m_frictionSlip = btScalar(friccion);
		/*for (int i=0;i<50;i++){
			acelerar();
		}*/
		for (int i = 0; i < vehiculo->getNumWheels(); i++)
		{
			btWheelInfo &wheel = vehiculo->getWheelInfo(i);
			wheel.m_suspensionStiffness = 20;    //tambaleo de las ruedas (se mueve como si fuera por terreno con baches). A mayor valor mayor tambaleo 
			wheel.m_wheelsDampingCompression = btScalar(0.2) * 2 * btSqrt(wheel.m_suspensionStiffness);;//btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f; 
			wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.5)*2*btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f; 
			wheel.m_rollInfluence = 1;       //0.1f;  //Empieza a rodar muy loco, si el valor es alto 
		}

	}
	else {
		for (int i = 0; i < vehiculo->getNumWheels(); i++)
		{
			btWheelInfo &wheel = vehiculo->getWheelInfo(i);
			wheel.m_suspensionStiffness = 40;    //tambaleo de las ruedas (se mueve como si fuera por terreno con baches). A mayor valor mayor tambaleo 
			wheel.m_wheelsDampingCompression = 2.4f;//btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f; 
			wheel.m_wheelsDampingRelaxation = 2.3f;  //btScalar(0.5)*2*btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f; 
			wheel.m_frictionSlip = btScalar(10000.0f);  //100;  //conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover 
			wheel.m_rollInfluence = 0.03;       //0.1f;  //Empieza a rodar muy loco, si el valor es alto 
			//wheel.m_maxSuspensionForce = 40000.f;  //A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa) 
		}
		FuerzaGiro = btScalar(0.1);
		vehiculo->getWheelInfo(0).m_frictionSlip = btScalar(10000.0f);
		vehiculo->getWheelInfo(1).m_frictionSlip = btScalar(10000.0f);

		vehiculo->getWheelInfo(2).m_frictionSlip = btScalar(10000.0f);
		vehiculo->getWheelInfo(3).m_frictionSlip = btScalar(10000.0f);
	}
}
void Corredor::desacelerar()
{
	vehiculo->applyEngineForce(0, 0);
	vehiculo->applyEngineForce(0, 1);
	vehiculo->applyEngineForce(0, 2);
	vehiculo->applyEngineForce(0, 3);

	vehiculo->setSteeringValue(0, 0);
	vehiculo->setSteeringValue(0, 1);

	vehiculo->setBrake(60, 0);
	vehiculo->setBrake(60, 1);
	vehiculo->setBrake(60, 2);
	vehiculo->setBrake(60, 3);
}

std::string Corredor::toString()
{
	std::string text = " -- CORREDOR -- ";
	//Mostrar la Posicion y Velocidad actuales.
	text += "\n Posicion [";
	text += to_string(getNodo()->getPosition().X);
	text += ", ";
	text += to_string(getNodo()->getPosition().Y);
	text += ", ";
	text += to_string(getNodo()->getPosition().Z);
	text += "]\n";
	text += " Direccion: ";
	text += getDireccion();
	text += " [ ";
	text += to_string(getDireccionGrados());
	text += " ]";
	text += "\n Vector direccion(Orientacion) X[ " + to_string(orientacion.X) +
		" ] Y[ " + to_string(orientacion.Z) + "]";
	text += "\n Velocidad (km/h): " + to_string(vehiculo->getCurrentSpeedKmHour());
	text += "\n Fuerza Motor: " + to_string(vehiculo->getWheelInfo(0).m_engineForce);


	text += "\nTurbo: ";
	if (turboActivado) text += "Activado";
	else text += "Desactivado";

	return text;
}

void Corredor::setFriccion(btScalar valor) {
	for (int i = 0; i < vehiculo->getNumWheels(); i++) {
		vehiculo->getWheelInfo(i).m_frictionSlip = btScalar(valor);  //100;	//conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover	
	}
}
void Corredor::aplicarAceite(){
	//CuerpoColisionChasis->setAngularFactor(btScalar(120*PI/180));
	//CuerpoColisionChasis->applyCentralForce(btVector3(100, 5.0f,100));
	frenodemano(true);
	CuerpoColisionChasis->setAngularVelocity(btVector3(0, 40, 0));
	for (int i=0;i<100;i++){
		girarIzquierda();
	}
	Fuerza=220;
}

//	----------------------------------------------
//		METODOS GET
//	----------------------------------------------

btRaycastVehicle *Corredor::getVehiculo()
{
	return vehiculo;
}

btRigidBody *Corredor::getRigidBody()
{
	return CuerpoColisionChasis;
}

IMeshSceneNode *Corredor::getNodo()
{
	return cuboNodo;
}

int Corredor::getDireccionGrados()
{
	return direccionGrados;
}

vector3df Corredor::getVectorDireccion()
{
	return orientacion;
}

//	----------------------------------------------
//		METODOS UPDATE
//	----------------------------------------------

void Corredor::update()
{
	if (turboActivado) {
		Motor3d *mundo = Motor3d::getInstancia();
		Timer *time = Timer::getInstancia();
		if (time->getTimer() - timerTurbo >= 1) {
			//cout << "Se acaba el turbo\n";
			desacelerar();
			setTurbo(false, false,0);
		}
	}
	else if (vehiculo->getCurrentSpeedKmHour() > 1)
		Fuerza = btScalar(FuerzaMaxima * (50 / vehiculo->getCurrentSpeedKmHour()));
	if (Fuerza < 1800) {
		FuerzaGiro = btScalar(0.001);
	}
	else {
		FuerzaGiro = btScalar(0.1);
	}

	updateEstado();
	movimiento();
	posicion.setX(cuboNodo->getPosition().X);
	posicion.setY(cuboNodo->getPosition().Y);
	posicion.setZ(cuboNodo->getPosition().Z);
	actualizarRuedas();
	updateDireccion();

	//borrar
	
	
	//ActualizarRaytest();
	
}


void Corredor::updateEstado(){
	if (vehiculo->getCurrentSpeedKmHour() < 0.5 && vehiculo->getCurrentSpeedKmHour() > -0.5){
		estado->setEstadoMovimiento(QUIETO);
	}
	estado->setEstadoCoche(POR_DEFECTO);
	if (proteccion==true){
		estado->setEstadoCoche(CON_ESCUDO);
	}
	if (turboActivado==true){
		estado->setEstadoCoche(CON_TURBO);
	}


	//Objetos:
	switch (tipoObj){
		// {NADA, FLECHA, FLECHA_TRIPLE, ESCUDO, ACEITE, CAJA_FALSA, TURBO} 
		case 0:
		estado->setEstadoObjeto(NADA);
		break;
		case 1:
		estado->setEstadoObjeto(FLECHA);
		break;
		case 2:
		estado->setEstadoObjeto(CAJA_FALSA);
		break;
		case 3:
		estado->setEstadoObjeto(TURBO);
		break;
		case 4:
		estado->setEstadoObjeto(ACEITE);
		break;
		case 5:
		estado->setEstadoObjeto(ESCUDO);
		break;
	}
	estado->update();
}
/**
 * Actualiza el vector direccion del corredor.
 */
void Corredor::updateVectorDireccion()
{
	btQuaternion quaternion = CuerpoColisionChasis->getOrientation();
	float anguloZ = quaternion.getAngle();
	float anguloX = cuboNodo->getRotation().Y * PI / 180;

	//cout<< "Rotacion en Y=="<< anguloZ  * 180/PI << endl;
	orientacion = vector3df(sin(anguloX), 0, cos(anguloZ));

	orientacion.normalize();
	//cout<< "ORIENTACION XNORMAL=="<< orientacion.X << " ORIENTACION ZNORMAL=="<< orientacion.Z  << endl;
}

/**
 * Actualiza la posicion hacia la que mira el corredor
 * Tabla de grados -
 * Norte    - 000 [341-20]
 * Noreste  - 045 [21-70]
 * Este	 - 090 [71-110]
 * SurEste  - 135 [111-160]
 * Sur		 - 180 [161-200]
 * SurOeste - 225 [201-250]
 * Oeste    - 270 [251-290]
 * NorOeste - 315 [291-340]
*/
void Corredor::updateDireccion()
{
	updateVectorDireccion();
	float orientacionZ = round(orientacion.Z);
	float orientacionX = round(orientacion.X);

	// NORTE
	if (orientacion.Z <= 0.98 && orientacionX == 0)
	{
		norte = true;
		sur = false;
		este = false;
		oeste = false;
	}
	else
	{
		// NORESTE
		if (orientacion.X >= 0.1 && orientacion.Z >= 0.1)
		{
			norte = true;
			sur = false;
			este = true;
			oeste = false;
		}
		else
		{
			// ESTE
			if (orientacion.X >= 0.98 && orientacionZ == 0)
			{
				norte = false;
				sur = false;
				oeste = false;
				este = true;
			}
			else
			{
				// SURESTE
				if (orientacion.X >= 0.1 && orientacion.Z <= -0.1)
				{
					norte = false;
					sur = true;
					este = true;
					oeste = false;
				}
				else
				{
					// SUR
					if (orientacionX == 0 && orientacion.Z <= -0.98)
					{
						norte = false;
						sur = true;
						este = false;
						oeste = false;
					}
					else
					{
						// SUROESTE
						if (orientacion.X <= -0.1 && orientacion.Z <= -0.1)
						{
							norte = false;
							sur = true;
							este = false;
							oeste = true;
						}
						else
						{
							// OESTE
							if (orientacion.X <= -0.95 && orientacionZ == 0)
							{
								norte = false;
								sur = false;
								este = false;
								oeste = true;
							}
							else
							{
								// NOROESTE
								if (orientacion.X <= -0.1 && orientacion.Z >= 0.1)
								{
									norte = true;
									sur = false;
									este = false;
									oeste = true;
								}
							}
						}
					}
				}
			}
		}
	}
}

/**
 * Mediante los boleanos de la clase, se obtiene la direccion
 * 	respecto de los ejes
*/
std::string Corredor::getDireccion()
{
	if (norte)
	{
		if (este)
		{
			return "noreste";
		}
		else
		{
			if (oeste)
			{
				return "noroeste";
			}
			else
			{
				return "norte";
			}
		}
	}

	if (sur)
	{
		if (este)
		{
			return "sureste";
		}
		else
		{
			if (oeste)
			{
				return "suroeste";
			}
			else
			{
				return "sur";
			}
		}
	}

	if (este)
	{
		if (norte)
		{
			return "noreste";
		}
		else
		{
			if (sur)
			{
				return "sureste";
			}
			else
			{
				return "este";
			}
		}
	}

	if (oeste)
	{
		if (norte)
		{
			return "noroeste";
		}
		else
		{
			if (sur)
			{
				return "suroeste";
			}
			else
			{
				return "oeste";
			}
		}
	}

	return "No Direccion";
}
//destructor
Corredor::~Corredor() {
	delete CuerpoColisionChasis->getMotionState();
	delete CentroGravedad;
	delete FormaColision;
	delete RayCastVehiculo;
	delete vehiculo;
	delete CuerpoColisionChasis;

}
bool Corredor::getProteccion() {
	return proteccion;
}

void Corredor::setProteccion(bool s) {
	escudo->getNodo()->setVisible(s);
	proteccion = s;
}