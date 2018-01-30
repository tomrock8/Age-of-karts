#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include <string>
#include <time.h>
#include "Pista.hpp"
#include "Client.hpp"


//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*
Corredor::Corredor(stringw rutaObj, btVector3 pos)
{
	cargador = 0;
	tipoObj = 0;
	turboActivado = false;
	objetivoFijado=false;
	timerTurbo = 0;
	limite = 0;
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
		cuboNodo->setPosition(vector3df(pos.getX(),pos.getY(),pos.getZ()));
		cuboNodo->setRotation(vector3df(0.0f, 90.0f, 0.0f));
	}

	estado = new EstadosJugador();
	vueltas = 1;

	posicion.setX(pos.getX());
	posicion.setY(pos.getY());
	posicion.setZ(pos.getZ());
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
	anterior = mapa->getArrayWaypoints()[mapa->getTamArrayWaypoints()-1];
	actual = mapa->getArrayWaypoints()[0];
	siguiente = actual->getNextWaypoint();
	siguiente_aux=siguiente;

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

	if (cuboNodo) InicializarFisicas();

	//HABILIDADES
	h = new Habilidad(4, this->getNodo());
	h->getNodo()->setVisible(false);
	h->setPosicion(pos);
	h->setPadre(this->getNodo());
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
		wheel.m_frictionSlip = btScalar(100000);  //100;  //conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover 
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
//-----------------------------//
//----------FISICAS------------//
//-----------------------------//
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
void Corredor::BorrarFisicas()
{
}
//-----------------------------//
//---------METODOS GET---------//
//-----------------------------//
int Corredor::getCargador() { return cargador; };
int Corredor::getTipoObj() { return tipoObj; };
int Corredor::getPosicionCarrera() {
	return posicionCarrera;
}
bool Corredor::getTurbo() {
	return turboActivado;
}
btRaycastVehicle *Corredor::getVehiculo()
{
	return vehiculo;
}
btRigidBody *Corredor::getRigidBody()
{
	return CuerpoColisionChasis;
}
IMeshSceneNode *Corredor::getNodo() {
	return cuboNodo;
}
btVector3 Corredor::getVectorDireccion()
{
	return orientacion;
}
bool Corredor::getProteccion() {
	return proteccion;
}
Waypoint *Corredor::getWaypointActual() {
	return actual;
}
Waypoint *Corredor::getWaypointSiguiente() {
	return siguiente;
}
Waypoint *Corredor::getWaypointAnterior() {
	return anterior;
}
btScalar Corredor::getdistanciaWaypoint() {
	return distanciaWaypoint;
}
btScalar Corredor::getdistanciaWaypointActual() {
	return distanciaWaypointActual;
}
btScalar Corredor::getdistanciaWaypointAnterior() {
	return distanciaWaypointAnterior;
}
int Corredor::getVueltas() {
	return vueltas;
}
int Corredor::getLimite() {
	return limite;
}
bool Corredor::getCheckItem() {
	return checkItem;
}
EstadosJugador *Corredor::getEstados() {
	return estado;
}
//-----------------------------//
//---------METODOS SET---------//
//-----------------------------//
void Corredor::setPosicion(float *pos, float *ori) {
	btVector3 btPos(pos[0], pos[1], pos[2]);

	btTransform trans;
	trans.setOrigin(btPos);
	btQuaternion quaternion;
	quaternion.setEulerZYX(ori[0] * PI / 180, ori[1] * PI / 180, ori[2] * PI / 180);
	trans.setRotation(quaternion);

	CuerpoColisionChasis->setCenterOfMassTransform(trans);

}
void Corredor::setPosicionCarrera(int i) {
	posicionCarrera = i;
}
void Corredor::setTipoObj()
{
	srand(time(NULL));
	tipoObj = rand() % 4 + 1;
	Client *c = Client::getInstancia();
	if (c->getConnected())
		c->PlayerSetObject(tipoObj);
	//cout << "Random ------>" << tipoObj << endl;
}
void Corredor::setTipoObj(int i)
{
	tipoObj = i;
	cout << "el objeto --- " << i << " ---" << endl;
	//cout << "Random ------>" << tipoObj << endl;
}
void Corredor::setWaypointActual(ISceneNode *nodo)
{
	//de momento lo pongo así, no da la segunda vuelta pero habria que mirar cuales se han visitado y cuales no
	//mas adelante se ve, Las IDS no funcionan bien tengo que preguntarle a santi a si que de momento lo comento para que
	//se puedan coger las cajas.


	if (nodo->getID() > actual->getWaypoint()->getID() && nodo->getID() == siguiente_aux->getWaypoint()->getID()) {

		actual = actual->getNextWaypoint();
		siguiente = actual->getNextWaypoint();
		if (actual->getWaypoint()->getID() == siguiente_aux->getWaypoint()->getID())
			siguiente_aux = siguiente;

	}

	if (nodo->getID() - 6 == 0) { //caso de vuelta completa

		if (siguiente_aux->getWaypoint()->getID() - 6 == 0) {
			//cout<<"HAS DADO UNA VUELTA"<<endl;
			vueltas++;
			actual = siguiente_aux;
			siguiente = actual->getNextWaypoint();
			siguiente_aux = actual->getNextWaypoint();
			//cout<<"--------------"<<vueltas<<" VUELTA SUPERADA-------------: "<<endl;
		}

	}

	if (nodo->getID()<actual->getWaypoint()->getID() && nodo->getID() - 6 != 0) {

		if (cuboNodo->getID() == 0) {
			//cout<< "VOY MARCHA ATRAS"<<endl;
			//cout<<endl;
		}
	}

	//cout<< "NODO ACTUAL:"<< actual->getWaypoint()->getID() <<endl;
	//cout<< "NODO SIGUIENTE:"<< siguiente->getWaypoint()->getID() <<endl;
}
void Corredor::setTurbo(bool activo, bool objeto, int valor) {
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
void Corredor::SetFuerzaVelocidad(int turbo)
{
	Fuerza = turbo;
}
void Corredor::setFriccion(btScalar valor) {
	for (int i = 0; i < vehiculo->getNumWheels(); i++) {
		vehiculo->getWheelInfo(i).m_frictionSlip = btScalar(valor);  //100;	//conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover	
	}
}
void Corredor::setProteccion(bool s) {
	escudo->getNodo()->setVisible(s);
	proteccion = s;
}
void Corredor::setLimite(int s) {
	limite = s;
}
void Corredor::setCheckItem(bool s) {
	checkItem = s;
}
void Corredor::setPosDisparo(btVector3 s) {
	posDisparo = s;
}
//-------------------------------------//
//-------TRATAMIENTOS OBJETOS----------//
//-------------------------------------//
void Corredor::lanzarItem(Proyectil *item, int direccionItem)
{
	/*
	Recibe un item de jugadorCorredor o jugadorIA y le aplica una velocidad para lanzarlo
	direccionItem: 1=Delante//
	-1=Atras	//
	*/
	btRigidBody *rigidItem = item->inicializarFisicas();
	if (direccionItem == 1)
		item->getRigidBody()->setLinearVelocity(btVector3(orientacion.getX() * 100, 5.0f, orientacion.getZ() * 100));
	else if (direccionItem == -1)
		item->getRigidBody()->setLinearVelocity(btVector3(-orientacion.getX() * 100, 5.0f, -orientacion.getZ() * 100));
	tipoObj = 0;
	decCargador();
}
void Corredor::lanzarItemTeledirigido()
{	
		
		vehiculo->applyEngineForce(0,0);
		vehiculo->applyEngineForce(0,1);
		vehiculo->applyEngineForce(0,2);
		vehiculo->applyEngineForce(0,3);
		vehiculo->setBrake(100,0);
		vehiculo->setBrake(100,1);
		vehiculo->setBrake(100,2);
		vehiculo->setBrake(100,3);
		aplicarAceite();
		//tipoObj = 0;
}
void Corredor::setObjetivoTelederigido(){
	objetivoFijado=true;
			Timer *time = Timer::getInstancia();
		timerTeledirigido=time->getTimer();
}

void Corredor::soltarItem()
{
	setTipoObj(0);
	setLimite(getLimite() + 2);
	cout << "El limite se ha incrementado, ahora es : " << getLimite() << endl;
	if (h->getHabilidadActiva() == true) {
		setLimite(0);
	}
}
void Corredor::usarObjetos() {
	Pista *pista = Pista::getInstancia();
	core::list<Item *> items = pista->getItems();
	if (getTipoObj() == 1)
	{
		pro = new Proyectil(posDisparo);
		lanzarItem(pro, 1);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		items.push_back(pro);
		soltarItem();
	}
	else if (getTipoObj() == 2)
	{
		posDisparo.setX(cuboNodo->getPosition().X - orientacion.getX() * 5);
		posDisparo.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 5);
		CajaFalsa *est = new CajaFalsa(posDisparo);
		est->inicializarFisicas();
		soltarItem();
		items.push_back(est);
	}
	else if (getTipoObj() == 3)
	{
		setTurbo(true, true, 26000);
	}
	else if (getTipoObj() == 4)
	{
		posDisparo.setX(cuboNodo->getPosition().X - orientacion.getX() * 5);
		posDisparo.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 5);
		Aceite *est2 = new Aceite(posDisparo);
		est2->inicializarFisicas();
		//est2->getRigidBody()->setCollisionFlags(est2->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		soltarItem();
		items.push_back(est2);
	}
	else if (getTipoObj() == 5)
	{
		if (getProteccion() == false) setProteccion(true);
		soltarItem();
	}
	else if (getTipoObj() == 6)
	{
		proX3 = new Proyectil *[3];
		btVector3 orientacioncentral(orientacion.getX(), orientacion.getY(), orientacion.getZ());
		btVector3 centro(cuboNodo->getPosition().X + orientacion.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 5);
		btVector3 orientacionderecha = orientacioncentral.rotate(btVector3(0, 1, 0), 10 * PI / 180);
		btVector3 orientacionizquierda = orientacioncentral.rotate(btVector3(0, 1, 0), -10 * PI / 180);
		btVector3 iz(cuboNodo->getPosition().X + orientacionizquierda.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionizquierda.getZ() * 5);
		btVector3 d(cuboNodo->getPosition().X + orientacionderecha.getX() * 5, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionderecha.getZ() * 5);
		for (int i = 0; i < 3; i++) {


			if (i == 0) {
				proX3[i] = new Proyectil(iz);
				proX3[i]->inicializarFisicas();
				proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacionizquierda.getX() * 100, 5.0f, orientacionizquierda.getZ() * 100));
			}
			if (i == 1) {

				proX3[i] = new Proyectil(centro);
				proX3[i]->inicializarFisicas();
				proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacioncentral.getX() * 100, 5.0f, orientacioncentral.getZ() * 100));
			}
			if (i == 2) {

				proX3[i] = new Proyectil(d);
				proX3[i]->inicializarFisicas();
				proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacionderecha.getX() * 100, 5.0f, orientacionderecha.getZ() * 100));
			}


			items.push_back(proX3[i]);




		}
		soltarItem();
	}
	else if (getTipoObj() == 7)
	{
		if (getLimite() >= 10) {//puedo lanzar la habilidad
			h->getNodo()->setVisible(true);
			h->setOrientacion(orientacion);
			h->setPadre(this->getNodo());
			h->setPosicion(posDisparo);
			h->lanzarHabilidad();
			items.push_back(h);
			soltarItem();
		}
		else {
			cout << "que mierda estas haciendo , no puedes usar la habilidad si tu limite no es 10 o mas" << endl;
		}

	}
	else if (tipoObj == 8){
		pt = new ItemTeledirigido(posDisparo);
		pt->lanzarItemTeledirigido(posicionCarrera);
		items.push_back(pt);
		soltarItem();
	}
	pista->setItems(items);

}
void Corredor::aplicarAceite() {
	//CuerpoColisionChasis->setAngularFactor(btScalar(120*PI/180));
	//CuerpoColisionChasis->applyCentralForce(btVector3(100, 5.0f,100));
	frenodemano(true);
	CuerpoColisionChasis->setAngularVelocity(btVector3(0, 40, 0));
	for (int i = 0; i < 100; i++) {
		girarIzquierda();
	}
	Fuerza = 500;
	frenodemano(false);
}
void Corredor::incCargador() { cargador++; };
void Corredor::decCargador() { cargador--; };
//----------------------------------------//
//-------CALCULO DE DISTANCIA-------------//
//----------------------------------------//
void Corredor::calculoDistanciaPunto() {
	

	
	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(siguiente->getPosicion().getX(), siguiente->getPosicion().getY(), siguiente->getPosicion().getZ());

	distanciaWaypoint = posCoche.distance2(posWaypoint);

	//calulamos la distancia hasta el waypoint 
	//cout << "WAYPOINT ACTUAL:" <<actual->getWaypoint()->getID() << endl;
	//cout << "WAYPOINT SIGUIENTE:" << siguiente->getWaypoint()->getID() << endl;
	
	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("DISTANCIA: ");
	//cout <<"distanciaWaypoint: "<<distanciaWaypoint<<endl; 
	texto->agregar(to_string(distanciaWaypoint)+"\n");

	texto->agregar("WAYPOINT ACTUAL: ");
	texto->agregar(to_string(actual->getWaypoint()->getID() - 6) + "\n");
	texto->agregar("WAYPOINT SIGUIENTE: ");
	texto->agregar(to_string( siguiente->getWaypoint()->getID()-6)+"\n");
	texto->agregar("WAYPOINT SIGUIENTE_AUX: ");
	texto->agregar(to_string( siguiente_aux->getWaypoint()->getID()-6)+"\n");
	texto->agregar("VUELTA: ");
	texto->agregar(to_string(vueltas)+"\n");
	texto->agregar("Limite:" + to_string(getLimite()) + "\n");

	


}
void Corredor::calculoDistanciaPuntoActual() {

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(actual->getPosicion().getX(), actual->getPosicion().getY(), actual->getPosicion().getZ());

	distanciaWaypointActual = posCoche.distance2(posWaypoint);

	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("DISTANCIA ACTUAL: ");
	texto->agregar(to_string(distanciaWaypointActual) + "\n");

}
void Corredor::calculoDistanciaPuntoAnterior() {

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(anterior->getPosicion().getX(), anterior->getPosicion().getY(), anterior->getPosicion().getZ());

	distanciaWaypointAnterior = posCoche.distance2(posWaypoint);

	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("ANTERIOR: ");
	//cout <<"DISTANCIA ANTERIOR: "<<distanciaWaypointAnterior<<" ID: "<<cuboNodo->getID()<<" WAYPOINT ANT: "<<anterior->getWaypoint()->getID()-6<<endl; 
	texto->agregar(to_string(anterior->getWaypoint()->getID()-6)+"\n");
}
//----------------------------------------//
//------------MOVIMIENTO------------------//
//----------------------------------------//
void Corredor::acelerar()
{
	if (vehiculo->getCurrentSpeedKmHour() > 300 && !turboActivado) {
		if (vehiculo->getCurrentSpeedKmHour() > 350) {
			Fuerza = btScalar(0);
		}
		else {
			Fuerza = btScalar(200);		//limitador de velocidad
		}

	}
	if (vehiculo->getCurrentSpeedKmHour() < 370) {
		vehiculo->applyEngineForce(Fuerza, 0);
		vehiculo->applyEngineForce(Fuerza, 1);
		vehiculo->applyEngineForce(Fuerza, 2);
		vehiculo->applyEngineForce(Fuerza, 3);
		vehiculo->setSteeringValue(btScalar(0), 0);
		vehiculo->setSteeringValue(btScalar(0), 1);
	}
	else {
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
	if (vehiculo->getCurrentSpeedKmHour() < 0) {
		estado->setEstadoMovimiento(MARCHA_ATRAS);
	}
	else {
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
	estado->setEstadoMovimiento(DESACELERA);
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
//---------------------------------------//
//--------DEBUG TEXTO PANTALLA-----------//
//---------------------------------------//
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
	
	text += "\n Vector direccion(Orientacion) X[ " + to_string(orientacion.getX()) +
		" ] Y[ " + to_string(orientacion.getZ()) + "]";
	text += "\n Velocidad (km/h): " + to_string(vehiculo->getCurrentSpeedKmHour());
	text += "\n Fuerza Motor: " + to_string(vehiculo->getWheelInfo(0).m_engineForce);


	text += "\nTurbo: ";
	if (turboActivado) text += "Activado";
	else text += "Desactivado";

	return text;
}
//---------------------------------------//
//-----------METODOS UPDATE--------------//
//---------------------------------------//
void Corredor::update()
{
	Timer *time = Timer::getInstancia();
	if (turboActivado) {
		Motor3d *mundo = Motor3d::getInstancia();

		if (time->getTimer() - timerTurbo >= 1) {
			//cout << "Se acaba el turbo\n";
			desacelerar();
			setTurbo(false, false, 0);
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

	updateTeledirigido();
	updateEstado();
	if (h->getHabilidadActiva())updateHabilidad();
	movimiento();
	posicion.setX(cuboNodo->getPosition().X);
	posicion.setY(cuboNodo->getPosition().Y);
	posicion.setZ(cuboNodo->getPosition().Z);
	actualizarRuedas();
	updateVectorDireccion();
	calculoDistanciaPunto();
	calculoDistanciaPuntoActual();
	calculoDistanciaPuntoAnterior();
	//cout<<"Posicion carrera: "<<posicionCarrera<<" ID: "<<cuboNodo->getID()<<endl;
	//ActualizarRaytest();
	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("POSICION CARRERA: ");
	texto->agregar(to_string(posicionCarrera)+"\n");
	
}

void Corredor::updateTeledirigido(){
	if (pt!=NULL){
		Timer *t = Timer::getInstancia();
		if (objetivoFijado){
			if (t->getTimer()-timerTeledirigido>2){
				
				lanzarItemTeledirigido();
			}
			if (t->getTimer()-timerTeledirigido>3){
				objetivoFijado=false;

			}
		}
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
void Corredor::updateHabilidad() {
	Timer *tiempo = Timer::getInstancia();
	int inicioHabilidad = h->getInicioHabilidad();


	if (tiempo->getTimer() - inicioHabilidad >= 3) {
	//cout << "Se acaba el tiro\n";
	h->setHabilidadActiva(false);
	h->getNodo()->setVisible(false);
	h->eliminarFisicas();

	}
}
void Corredor::updateEstado(){
	if (vehiculo->getCurrentSpeedKmHour() < 0.5 && vehiculo->getCurrentSpeedKmHour() > -0.5){
		estado->setEstadoMovimiento(QUIETO);
	}
	estado->setEstadoCoche(POR_DEFECTO);
	if (proteccion == true) {
		estado->setEstadoCoche(CON_ESCUDO);
	}
	if (turboActivado == true) {
		estado->setEstadoCoche(CON_TURBO);
	}


	//Objetos:
	switch (tipoObj) {
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
		case 6:
		estado->setEstadoObjeto(FLECHA_TRIPLE);
		break;
		case 7:
		estado->setEstadoObjeto(HABILIDAD);
		break;
		case 8:
		estado->setEstadoObjeto(ITEM_TELEDIRIGIDO);
		break;
	}
	estado->update();
}
void Corredor::updateVectorDireccion()
{
	/* Actualiza el vector direccion del corredor. */
	btQuaternion quaternion = CuerpoColisionChasis->getOrientation();
	float anguloZ = quaternion.getAngle();
	float anguloX = cuboNodo->getRotation().Y * PI / 180;

	//cout<< "Rotacion en Y=="<< anguloZ  * 180/PI << endl;
	orientacion = btVector3(sin(anguloX), 0, cos(anguloZ));

	orientacion.normalize();
	//cout<< "ORIENTACION XNORMAL=="<< orientacion.X << " ORIENTACION ZNORMAL=="<< orientacion.Z  << endl;
}
//---------------------------------------//
//--------------DESTRUCTOR---------------//
//---------------------------------------//
Corredor::~Corredor() {
	cout << "\nENTRO DESTRUCTOR CORREDOR: \n";
	
	// Los rigid body se borran desde el motor de fisicas
	// Los collision shape se borran desde el motor de fisicas
	// delete CuerpoColisionChasis;
	delete vehiculo;
	cuboNodo->remove();

	cout << "SALGO DESTRUCTOR CORREDOR: ";
}