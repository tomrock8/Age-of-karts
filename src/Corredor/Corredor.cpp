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
	velocidadMaxima=380;
	velocidadMaximaTurbo=420;
	direccionContraria=0;
	velocidadMaximaAtras=-100;
	velocidadLimiteGiro=120;
	turboActivado = false;
	objetivoFijado = false;
	aceiteActivado = false;
	timerTurbo = 0;
	timerAceite = 0;
	limite = 0;
	coche = Motor3d::instancia().getScene()->getMesh(rutaObj);
	cuboNodo = Motor3d::instancia().getScene()->addMeshSceneNode(coche, 0);
	//cuboNodo = smgr->addCubeSceneNode(1.5f);
	if (cuboNodo) {
		cuboNodo->setName("Jugador");
		//cambiar a color rojo del coche
		Motor3d::instancia().getScene()->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
		cuboNodo->setScale(vector3df(1, 1, 1.5));
		// Desactivar la iluminacion del cubo
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(vector3df(pos.getX(), pos.getY(), pos.getZ()));
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
	rueda1 = Motor3d::instancia().getScene()->addMeshSceneNode(Motor3d::instancia().getScene()->getMesh("assets/Objetos/rueda.obj"));
    rueda2 = Motor3d::instancia().getScene()->addMeshSceneNode(Motor3d::instancia().getScene()->getMesh("assets/Objetos/rueda.obj"));
    rueda3 = Motor3d::instancia().getScene()->addMeshSceneNode(Motor3d::instancia().getScene()->getMesh("assets/Objetos/rueda.obj"));
    rueda4 = Motor3d::instancia().getScene()->addMeshSceneNode(Motor3d::instancia().getScene()->getMesh("assets/Objetos/rueda.obj"));

	rueda1->setScale(vector3df(2,1,1));//alante derecha
	rueda2->setScale(vector3df(2,1,1));//delante izquierda
	rueda3->setScale(vector3df(2,1,1));//atras derecha
	rueda4->setScale(vector3df(2,1,1));//atras izquierda


	//establecemos el primer waypoint del mapa
	Pista *mapa = Pista::getInstancia();
	anterior = mapa->getArrayWaypoints()[mapa->getTamArrayWaypoints() - 1];
	actual = mapa->getArrayWaypoints()[0];
	siguiente = actual->getNextWaypoint();
	siguiente_aux = siguiente;

	//smgr->getMeshManipulator()->setVertexColors(rueda1->getMesh(),SColor(255, 255, 0, 0));
	rueda1->setMaterialFlag(EMF_LIGHTING, false);
	rueda2->setMaterialFlag(EMF_LIGHTING, false);
	rueda3->setMaterialFlag(EMF_LIGHTING, false);
	rueda4->setMaterialFlag(EMF_LIGHTING, false);


	direccionRuedas = btVector3(0, -1, 0);
	rotacionRuedas = btVector3(-1, 0, 0);
	suspension = btScalar(2.3); // cuanto mas valor el chasis mas alto respecto a las ruedas
	FuerzaMaxima = btScalar(4000); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
	Fuerza = FuerzaMaxima;
	anchoRueda = btScalar(0.4);			  //0.4
	radioRueda = btScalar(0.5);			  //No menor de 0.4 sino ni se mueve (ruedas pequenyas)
	alturaConexionChasis = btScalar(1.2); //influye mucho en la acceleracion de salida
	Masa = btScalar(500);
	FuerzaFrenado = btScalar(-8000);
	FuerzaGiro = btScalar(0.1); //manejo a la hora de girar
	FuerzaFrenoMano = btScalar(700);
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
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, -3)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);

	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, -3)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);

	for (int i = 0; i < vehiculo->getNumWheels(); i++)
	{
		btWheelInfo &wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = 10;    // a mas valor mas altura del chasis respecto a las ruedas va en funcion de compresion y relajacion
		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f; 
		wheel.m_wheelsDampingRelaxation =  btScalar(0.5)* 2 *btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f; 
		wheel.m_frictionSlip = btScalar(10000);  //100;  //conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover 
		wheel.m_rollInfluence = 0;       //0.1f;  //Empieza a rodar muy loco, si el valor es alto 
											//wheel.m_maxSuspensionForce = 40000.f;  //A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa) 
		//wheel.m_maxSuspensionTravelCm = 10; //a menos valor la suspension es mas dura por lo tanto el chasis no baja tanto 

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
	localTransform.setOrigin(btVector3(0, 1, 0));
	CentroGravedad = new btCompoundShape();

	//Forma Colision
	btVector3 TamanyoFormaColision(cuboNodo->getScale().X, cuboNodo->getScale().Y, cuboNodo->getScale().Z * 2);
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
	if(totalJugadores == 2)
	{
		if(posicionCarrera == 1)
		{
			SimPosCarrera = 3;
		}
		else if(posicionCarrera == 2){
			SimPosCarrera = 4;
		}
	}
	else if(totalJugadores == 3)
	{
		if(posicionCarrera == 1)
		{
			SimPosCarrera = 2;
		}
		else if(posicionCarrera == 2)
		{
			SimPosCarrera = 3;
		}
		else if(posicionCarrera == 3)
		{
			SimPosCarrera = 4;
		}
	}
	else if(totalJugadores == 4)
	{
		if(posicionCarrera == 1)
		{
			SimPosCarrera == 2;
		}
		else if(posicionCarrera == 2)
		{
			SimPosCarrera == 3;
		}
		else if(posicionCarrera == 3)
		{
			SimPosCarrera == 4;
		}
		else if(posicionCarrera == 4)
		{
			SimPosCarrera == 5;
		}
	}
	else 
	{
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
	if(SimPosCarrera == 1){
		if(random < 16)
			tipoObj = 1;
		else if(random < 43)
			tipoObj = 2;
		else if(random < 53)
			tipoObj = 3;
		else if(random < 79)
			tipoObj = 4;
		else if(random < 96)
			tipoObj = 5;
		else if(random < 101)
			tipoObj = 6;
		else if(random < 101)
			tipoObj = 7;
		else if(random < 101)
			tipoObj = 8;
	}else if(SimPosCarrera == 2 || SimPosCarrera == 3){
		if(random < 16)
			tipoObj = 1;
		else if(random < 31)
			tipoObj = 2;
		else if(random < 46)
			tipoObj = 3;
		else if(random < 61)
			tipoObj = 4;
		else if(random < 76)
			tipoObj = 5;
		else if(random < 86)
			tipoObj = 6;
		else if(random < 91)
			tipoObj = 7;
		else if(random < 101)
			tipoObj = 8;
	}else if(SimPosCarrera == 4 || SimPosCarrera == 5){
		if(random < 16)
			tipoObj = 1;
		else if(random < 26)
			tipoObj = 2;
		else if(random < 41)
			tipoObj = 3;
		else if(random < 51)
			tipoObj = 4;
		else if(random < 61)
			tipoObj = 5;
		else if(random < 76)
			tipoObj = 6;
		else if(random < 86)
			tipoObj = 7;
		else if(random < 101)
			tipoObj = 8;
	}else if(SimPosCarrera == 6){
		if(random < 11)
			tipoObj = 1;
		else if(random < 14)
			tipoObj = 2;
		else if(random < 29)
			tipoObj = 3;
		else if(random < 31)
			tipoObj = 4;
		else if(random < 46)
			tipoObj = 5;
		else if(random < 61)
			tipoObj = 6;
		else if(random < 76)
			tipoObj = 7;
		else if(random < 101)
			tipoObj = 8;
	}

	cout << "Posicion: " << posicionCarrera << " - NumRandom: " << random << " - Objeto: " << tipoObj << endl;

	if(tipoObj == 8)
		cargador = 3;
	else if(tipoObj == 3)
		cargador = 1;
	
	Client *c = Client::getInstancia();
	if (c->getConnected())
		c->PlayerSetObject(tipoObj);
	//cout << "Random ------>" << tipoObj << endl;
}
void Corredor::setTipoObj(int i)
{
	tipoObj = i;
	if(tipoObj == 8)
		cargador = 3;
	else if(tipoObj == 3)
		cargador = 1;
	cout << "el objeto --- " << i << " ---" << endl;
	//cout << "Random ------>" << tipoObj << endl;
}
void Corredor::setWaypointActual(ISceneNode *nodo)
{
	//de momento lo pongo así, no da la segunda vuelta pero habria que mirar cuales se han visitado y cuales no
	//mas adelante se ve, Las IDS no funcionan bien tengo que preguntarle a santi a si que de momento lo comento para que
	//se puedan coger las cajas.

	bool b=false;	//booleano para comprobar si esta siguiendo bien los waypoints
	if (nodo->getID() > actual->getWaypoint()->getID() && nodo->getID() == siguiente_aux->getWaypoint()->getID()) {

		actual = actual->getNextWaypoint();
		siguiente = actual->getNextWaypoint();
		if (actual->getWaypoint()->getID() == siguiente_aux->getWaypoint()->getID()){
			siguiente_aux = siguiente;
			b=true;
			direccionContraria=0;
		}
	}

	if (nodo->getID() - 6 == 0) { //caso de vuelta completa

		if (siguiente_aux->getWaypoint()->getID() - 6 == 0) {
			//cout<<"HAS DADO UNA VUELTA"<<endl;
			vueltas++;
			actual = siguiente_aux;
			siguiente = actual->getNextWaypoint();
			siguiente_aux = actual->getNextWaypoint();
			b=true;
			direccionContraria=0;
		}

	}

	if (b==false && nodo->getID() - 6 != 0 && nodo->getID()>=siguiente_aux->getWaypoint()->getID()-2 && direccionContraria!=2) { //comprobador de recuperacion de direccion correcta en caso de no estar en la meta
		direccionContraria=0;
	}
	Pista *mapa = Pista::getInstancia();
	//comprobadores direccion opuesta
	if ((b==false && nodo->getID()<siguiente_aux->getWaypoint()->getID()-4 && nodo->getID() - 6 != 0) || (mapa->getTamArrayWaypoints()-1 == nodo->getID() - 6 && (actual->getWaypoint()->getID()-6!=mapa->getTamArrayWaypoints()-1))) {
		direccionContraria=1;
		if ((mapa->getTamArrayWaypoints()-1 == nodo->getID() - 6 && (actual->getWaypoint()->getID()-6!=mapa->getTamArrayWaypoints()-1))){
			direccionContraria=2;	//va marcha atras en la meta id=0 la comprobacion es distinta (por ser mayor el id siempre, tanto para alante como para atras)
		}
	}

	
	//cout<< "NODO ACTUAL:"<< actual->getWaypoint()->getID() <<endl;
	//cout<< "NODO SIGUIENTE:"<< siguiente->getWaypoint()->getID() <<endl;
}
void Corredor::setTurbo(bool activo, bool objeto, int valor) {
	turboActivado = activo;
	if (activo) {
		SetFuerzaVelocidad(valor);
		acelerar();
		Timer *time = Timer::getInstancia();
		timerTurbo = time->getTimer();
		if (objeto) {
			if(cargador==1) 
				tipoObj = 0;
			cargador--;
			cout << "Turbos restantes: " << cargador << endl;
		}
	}
	else {
		SetFuerzaVelocidad(FuerzaMaxima);
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
	rigidItem->setCollisionFlags(rigidItem->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	if (direccionItem == 1)
		item->getRigidBody()->setLinearVelocity(btVector3(orientacion.getX() * 400, 5.0f, orientacion.getZ() * 400));
	else if (direccionItem == -1)
		item->getRigidBody()->setLinearVelocity(btVector3(-orientacion.getX() * 400, 20.0f, -orientacion.getZ() * 400));
	tipoObj = 0;
	decCargador();
}
void Corredor::lanzarItemTeledirigido()
{
	resetFuerzas();
	aplicarAceite();
	//tipoObj = 0;
}
void Corredor::setObjetivoTelederigido() {
	objetivoFijado = true;
	Timer *time = Timer::getInstancia();
	timerTeledirigido = time->getTimer();
}
void Corredor::setAceite(){
	aceiteActivado = true;
	Timer *time = Timer::getInstancia();
	timerAceite = time->getTimer();
}

void Corredor::resetFuerzas(){
	CuerpoColisionChasis->clearForces();
	btVector3 zeroVector(0,0,0);
	CuerpoColisionChasis->setLinearVelocity(zeroVector);
	CuerpoColisionChasis->setAngularVelocity(zeroVector);
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
	core::list<Item *> items = pista->getItems();
	if (getTipoObj() == 1)		// PROYECTIL
	{
		pro = new Proyectil(posDisparo);
		lanzarItem(pro, 1);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		items.push_back(pro);
		soltarItem();
	}
	else if (getTipoObj() == 2)	//CAJA FALSA
	{
		posDisparo.setX(cuboNodo->getPosition().X - orientacion.getX() * 10);
		posDisparo.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 10);
		CajaFalsa *est = new CajaFalsa(posDisparo);
		est->inicializarFisicas();
		soltarItem();
		items.push_back(est);
	}
	else if (getTipoObj() == 3)	//TURBO
	{
		setTurbo(true, true, 26000);
	}
	else if (getTipoObj() == 4)	//ACEITE
	{
		posDisparo.setX(cuboNodo->getPosition().X - orientacion.getX() * 10);
		posDisparo.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 10);
		Aceite *est2 = new Aceite(posDisparo);
		est2->inicializarFisicas();
		//est2->getRigidBody()->setCollisionFlags(est2->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		soltarItem();
		items.push_back(est2);
	}
	else if (getTipoObj() == 5)	//ESCUDO
	{
		if (getProteccion() == false) setProteccion(true);
		soltarItem();
	}
	else if (getTipoObj() == 6)	//FLECHA TRIPLE
	{
		proX3 = new Proyectil *[3];
		btVector3 orientacioncentral(orientacion.getX(), orientacion.getY(), orientacion.getZ());
		btVector3 centro(cuboNodo->getPosition().X + orientacion.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 10);
		btVector3 orientacionderecha = orientacioncentral.rotate(btVector3(0, 1, 0), 10 * PI / 180);
		btVector3 orientacionizquierda = orientacioncentral.rotate(btVector3(0, 1, 0), -10 * PI / 180);
		btVector3 iz(cuboNodo->getPosition().X + orientacionizquierda.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionizquierda.getZ() * 10);
		btVector3 d(cuboNodo->getPosition().X + orientacionderecha.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionderecha.getZ() * 10);
		for (int i = 0; i < 3; i++) {


			if (i == 0) {
				proX3[i] = new Proyectil(iz);
				proX3[i]->inicializarFisicas();
				proX3[i]->getRigidBody()->setCollisionFlags(proX3[i]->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
				proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacionizquierda.getX() * 400, 5.0f, orientacionizquierda.getZ() * 400));
			}
			if (i == 1) {

				proX3[i] = new Proyectil(centro);
				proX3[i]->inicializarFisicas();
				proX3[i]->getRigidBody()->setCollisionFlags(proX3[i]->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
				proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacioncentral.getX() * 400, 5.0f, orientacioncentral.getZ() * 400));
			}
			if (i == 2) {

				proX3[i] = new Proyectil(d);
				proX3[i]->inicializarFisicas();
				proX3[i]->getRigidBody()->setCollisionFlags(proX3[i]->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
				proX3[i]->getRigidBody()->setLinearVelocity(btVector3(orientacionderecha.getX() * 400, 5.0f, orientacionderecha.getZ() * 400));
			}


			items.push_back(proX3[i]);




		}
		soltarItem();
	}
	else if (tipoObj == 7)	//FLECHA TELEDIRIGIDA
	{
		pt = new ItemTeledirigido(posDisparo);
		pt->lanzarItemTeledirigido(posicionCarrera);
		items.push_back(pt);
		soltarItem();
	}
	else if(tipoObj == 8)	//TURBO TRIPLE
	{
		setTurbo(true, true, 26000);
	}
	else if (getTipoObj() == 9) //HABILIDAD
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
			cout << "No puedes usar la habilidad si tu limite no es 10 o mas" << endl;
		}

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
	/*
	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("DISTANCIA: ");
	//cout <<"distanciaWaypoint: "<<distanciaWaypoint<<endl; 
	texto->agregar(to_string(distanciaWaypoint) + "\n");

	texto->agregar("WAYPOINT ACTUAL: ");
	texto->agregar(to_string(actual->getWaypoint()->getID() - 6) + "\n");
	texto->agregar("WAYPOINT SIGUIENTE: ");
	texto->agregar(to_string(siguiente->getWaypoint()->getID() - 6) + "\n");
	texto->agregar("WAYPOINT SIGUIENTE_AUX: ");
	texto->agregar(to_string(siguiente_aux->getWaypoint()->getID() - 6) + "\n");
	texto->agregar("VUELTA: ");
	texto->agregar(to_string(vueltas) + "\n");
	texto->agregar("Limite:" + to_string(getLimite()) + "\n");
*/
	



}
void Corredor::calculoDistanciaPuntoActual() {

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(actual->getPosicion().getX(), actual->getPosicion().getY(), actual->getPosicion().getZ());

	distanciaWaypointActual = posCoche.distance2(posWaypoint);
/*
	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("DISTANCIA ACTUAL: ");
	texto->agregar(to_string(distanciaWaypointActual) + "\n");
*/
}
void Corredor::calculoDistanciaPuntoAnterior() {

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(anterior->getPosicion().getX(), anterior->getPosicion().getY(), anterior->getPosicion().getZ());

	distanciaWaypointAnterior = posCoche.distance2(posWaypoint);
/*
	TextoPantalla * texto = TextoPantalla::getInstancia();
	texto->agregar("ANTERIOR: ");
	//cout <<"DISTANCIA ANTERIOR: "<<distanciaWaypointAnterior<<" ID: "<<cuboNodo->getID()<<" WAYPOINT ANT: "<<anterior->getWaypoint()->getID()-6<<endl; 
	texto->agregar(to_string(anterior->getWaypoint()->getID()-6)+"\n");
*/
}
//----------------------------------------//
//------------MOVIMIENTO------------------//
//----------------------------------------//
void Corredor::acelerar()
{
	if((vehiculo->getCurrentSpeedKmHour()>velocidadMaxima && !turboActivado) || (turboActivado && vehiculo->getCurrentSpeedKmHour()>velocidadMaximaTurbo)){

		vehiculo->applyEngineForce(0, 0);
		vehiculo->applyEngineForce(0, 1);
		vehiculo->applyEngineForce(0, 2);
		vehiculo->applyEngineForce(0, 3);

	}else{
		vehiculo->applyEngineForce(Fuerza, 0);
		vehiculo->applyEngineForce(Fuerza, 1);
		vehiculo->applyEngineForce(Fuerza, 2);
		vehiculo->applyEngineForce(Fuerza, 3);
		}
		vehiculo->setSteeringValue(btScalar(0), 0);
		vehiculo->setSteeringValue(btScalar(0), 1);
	
	estado->setEstadoMovimiento(AVANZA);
	estado->setDireccionMovimiento(RECTO);
}
void Corredor::frenar()
{
	

	if(vehiculo->getCurrentSpeedKmHour() < velocidadMaximaAtras){

	vehiculo->applyEngineForce(0, 0);
	vehiculo->applyEngineForce(0, 1);
	vehiculo->applyEngineForce(0, 2);
	vehiculo->applyEngineForce(0, 3);
	
	}else{
	vehiculo->applyEngineForce(FuerzaFrenado, 0);
	vehiculo->applyEngineForce(FuerzaFrenado, 1);
	vehiculo->applyEngineForce(FuerzaFrenado, 2);
	vehiculo->applyEngineForce(FuerzaFrenado, 3);
	
	}

	vehiculo->setSteeringValue(btScalar(0), 0);
	vehiculo->setSteeringValue(btScalar(0), 1);

	if(vehiculo->getCurrentSpeedKmHour() < 0){
		estado->setEstadoMovimiento(MARCHA_ATRAS);
	}else{
		estado->setEstadoMovimiento(FRENA);
	}
	
	
}
void Corredor::girarDerecha()
{
	estado->setDireccionMovimiento(DERECHA);
	if(vehiculo->getCurrentSpeedKmHour()<velocidadLimiteGiro){
	FuerzaGiro = 0.4;
	}else{
	FuerzaGiro = 0.1;
	}

	vehiculo->setSteeringValue(FuerzaGiro, 0);
	vehiculo->setSteeringValue(FuerzaGiro, 1);
}
void Corredor::girarIzquierda()
{
	estado->setDireccionMovimiento(IZQUIERDA);
	if(vehiculo->getCurrentSpeedKmHour()<velocidadLimiteGiro){
	FuerzaGiro = 0.4;
	}else{
	FuerzaGiro = 0.1;
	}
	vehiculo->setSteeringValue(-FuerzaGiro, 0);
	vehiculo->setSteeringValue(-FuerzaGiro, 1);
}
void Corredor::frenodemano(bool activo)
{
	int friccion = 1.f;
	if (activo) {
		estado->setEstadoMovimiento(DERRAPA);
		FuerzaGiro = btScalar(0.25);

		//vehiculo->applyEngineForce(Fuerza, 0);
		//vehiculo->applyEngineForce(Fuerza, 1);
		//vehiculo->applyEngineForce(Fuerza, 2);
		//vehiculo->applyEngineForce(Fuerza, 3);

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

void Corredor::comprobarSueloRuedas()
{
/*
	for(int i = 0; i < vehiculo->getNumWheels(); i++){
	if(!vehiculo->getWheelInfo(i).m_raycastInfo.m_isInContact){
		
		vehiculo->applyEngineForce(0, 0);
		vehiculo->applyEngineForce(0, 1);
		vehiculo->applyEngineForce(0, 2);
		vehiculo->applyEngineForce(0, 3);
		vehiculo->setSteeringValue(0, 0);
		vehiculo->setSteeringValue(0, 1);
		//cout<<"LA RUEDA :: "<< i << "NO ESTA EN CONTACTO"<<endl;
	}	
	
	}	

*/
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


	TextoPantalla *texto =TextoPantalla::getInstancia();
	texto->agregar("---------------------- \n");
    texto->agregar("\n Rueda alante izquierda: ");
	texto->agregar(to_string(vehiculo->getWheelInfo(0).m_engineForce));
	texto->agregar("\n Rueda alante derecha: ");
	texto->agregar(to_string(vehiculo->getWheelInfo(1).m_engineForce));
	texto->agregar("\n Rueda atras izquierda:");
	texto->agregar(to_string(vehiculo->getWheelInfo(2).m_engineForce));
	texto->agregar("\n Rueda atras derecha: ");
	texto->agregar(to_string(vehiculo->getWheelInfo(3).m_engineForce));
	texto->agregar("\n VELOCIDAD: ");
	texto->agregar(to_string(vehiculo->getCurrentSpeedKmHour()));
	if (direccionContraria!=0){
		texto->agregar("\nVAS EN DIRECCION CONTRARIA, JUGADOR: ");
		texto->agregar(to_string(cuboNodo->getID())+"\n");
	}

	Timer *time = Timer::getInstancia();

	if (turboActivado) {
		if (time->getTimer() - timerTurbo >= 1) {
			//cout << "Se acaba el turbo\n";
			desacelerar();
			setTurbo(false, false, 0);
		}
	}


	updateTimerObstaculos();
	updateEstado();
	if (h->getHabilidadActiva())updateHabilidad();
	movimiento();
	comprobarSueloRuedas();
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
	
	texto->agregar("\nPOSICION CARRERA: ");
	texto->agregar(to_string(posicionCarrera) + "\n");

}

void Corredor::updateTimerObstaculos() {
	if (pt != NULL) {	
		if (objetivoFijado){
			Timer *t = Timer::getInstancia();
			if (t->getTimer()-timerTeledirigido>=2){
				
				lanzarItemTeledirigido();
			}
			if (t->getTimer()-timerTeledirigido>=3){
				objetivoFijado=false;

			}
		}
	}
	if (aceiteActivado){
		Timer *t2 = Timer::getInstancia();
			if (t2->getTimer()-timerAceite>=0){
				lanzarItemTeledirigido();
			}
			if (t2->getTimer()-timerAceite>=1){
				aceiteActivado=false;
			}
		}

}

void Corredor::actualizarRuedas()
{
	btTransform ruedas = vehiculo->getWheelTransformWS(0);
	vector3df Euler;
	btQuaternion TQuat = ruedas.getRotation();
	quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;
	
	float distanciAancho=1.5;
	float ditanciaLargo=1;

	

	//rueda1
	rueda1->setPosition(vector3df(distanciAancho*orientacion.getZ() +ruedas.getOrigin().getX() + ditanciaLargo*orientacion.getX(), ruedas.getOrigin().getY()+0.5,
	orientacion.getX()*-distanciAancho + ruedas.getOrigin().getZ()+ ditanciaLargo*orientacion.getZ()));
	rueda1->setRotation(vector3df(Euler.X, Euler.Y , Euler.Z));
	
	//rueda2
	ruedas = vehiculo->getWheelTransformWS(1);
	TQuat = ruedas.getRotation();
	q = quaternion(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;
	rueda2->setPosition(vector3df(-distanciAancho*orientacion.getZ() +ruedas.getOrigin().getX() + ditanciaLargo*orientacion.getX(), ruedas.getOrigin().getY()+0.5,
	orientacion.getX()*distanciAancho + ruedas.getOrigin().getZ()+ ditanciaLargo*orientacion.getZ()));
	rueda2->setRotation(vector3df(Euler.X, Euler.Y +180 , Euler.Z));
	
	//rueda3
	ruedas = vehiculo->getWheelTransformWS(2);
	TQuat = ruedas.getRotation();
	q = quaternion(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;
	rueda3->setPosition(vector3df(distanciAancho*orientacion.getZ() +ruedas.getOrigin().getX() + ditanciaLargo*orientacion.getX(), ruedas.getOrigin().getY()+0.5,
	orientacion.getX()*-distanciAancho + ruedas.getOrigin().getZ()+ ditanciaLargo*orientacion.getZ()));
	rueda3->setRotation(vector3df(Euler.X, Euler.Y , Euler.Z));
	
	
	//rueda4
	ruedas = vehiculo->getWheelTransformWS(3);
	TQuat = ruedas.getRotation();
	q = quaternion(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= RADTODEG;
	rueda4->setPosition(vector3df(-distanciAancho*orientacion.getZ() +ruedas.getOrigin().getX() + ditanciaLargo*orientacion.getX(), ruedas.getOrigin().getY()+0.5,
	orientacion.getX()*distanciAancho + ruedas.getOrigin().getZ()+ ditanciaLargo*orientacion.getZ()));
	rueda4->setRotation(vector3df(Euler.X, Euler.Y +180, Euler.Z));


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
void Corredor::updateEstado() {
	if (vehiculo->getCurrentSpeedKmHour() < 0.5 && vehiculo->getCurrentSpeedKmHour() > -0.5) {
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
		estado->setEstadoObjeto(ITEM_TELEDIRIGIDO);
		break;
		case 8:
		estado->setEstadoObjeto(HABILIDAD);
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
	cout << "\nENTRO DESTRUCTOR CORREDOR. ";
	delete vehiculo;
	cout << "SALGO DESTRUCTOR CORREDOR\n";
}