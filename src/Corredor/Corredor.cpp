#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include <string>
#include <time.h>
#include "Pista.hpp"
#include "Client.hpp"


//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*

Corredor::Corredor(stringw rutaObj, btVector3 pos,tipo_jugador tipo)
{
	cargador = 0;
	tipoObj = 0;
	turboActivado = false;
	objetivoFijado = false;
	aceiteActivado = false;
	timerTurbo = 0;
	timerAceite = 0;
	limite = 0;
	direccionContraria=0;
	estado = new EstadosJugador();
	vueltas = 1;
	maxvueltas=4;
	tipojugador=tipo;
	
	tipojugador=tipo;
	estado->setEstadoCarrera(PARRILLA);

	coche = Motor3d::instancia().getScene()->getMesh(rutaObj);
	cuboNodo = Motor3d::instancia().getScene()->addMeshSceneNode(coche, 0);
	//cuboNodo = smgr->addCubeSceneNode(1.5f);
	if (cuboNodo) {
		cuboNodo->setName("Jugador");
		
		cuboNodo->setScale(vector3df(1, 1, 1.5));
		// Desactivar la iluminacion del cubo
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(vector3df(pos.getX(), pos.getY(), pos.getZ()));
		cuboNodo->setRotation(vector3df(0.0f, 90.0f, 0.0f));
	}

	posicion.setX(pos.getX());
	posicion.setY(pos.getY());
	posicion.setZ(pos.getZ());

	//escudo
	//escudo = new Escudo(pos, getNodo());
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
	suspension = btScalar(1.9); // cuanto mas valor el chasis mas alto respecto a las ruedas
	anchoRueda = btScalar(0.4);			  //0.4
	radioRueda = btScalar(0.5);			  //No menor de 0.4 sino ni se mueve (ruedas pequenyas)
	alturaConexionChasis = btScalar(1.2); //influye mucho en la acceleracion de salida
	FuerzaFrenado = btScalar(-5000);
	FuerzaFrenoMano = btScalar(700);
	FuerzaFrenadoReposo = btScalar(60);
	
	//VALORES POR DEFECTO
	FuerzaGiro = btScalar(0.1); //manejo a la hora de girar
	Masa = btScalar(400);
	FuerzaMaxima = btScalar(4000); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
	Fuerza = FuerzaMaxima;
	indiceGiroAlto=btScalar(0.4);
	indiceGiroBajo=btScalar(0.1);
	velocidadMaxima=380;
	velocidadMaximaTurbo=420;
	velocidadLimiteGiro=150;
	velocidadMaximaAtras=-100;
	
	if (cuboNodo) InicializarFisicas();

	setParametros();

}
void Corredor::setParametros(){

	//cambiar parametros en funcion del tipo
	int num=0;
	switch (tipojugador){
		case GLADIADOR:
			//Motor3d::instancia().getScene()->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
			cuboNodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/red.png"));
			//----ACELERACION-----
			FuerzaMaxima = btScalar(3800);
			Fuerza = FuerzaMaxima;
			//----VELOCIDAD-------
			velocidadMaxima=360;
			velocidadMaximaTurbo=410;
			//----GIRO/MANEJO-----
			indiceGiroAlto=btScalar(0.4);
			indiceGiroBajo=btScalar(0.085);
			velocidadLimiteGiro=150;
			//------PESO------
			Masa = btScalar(400);
			//-----HABILIDAD-----
			num=1;
			break;
		case PIRATA:
			cuboNodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/green.png"));
			//----ACELERACION-----
			FuerzaMaxima = btScalar(4200); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
			Fuerza = FuerzaMaxima;
			//----VELOCIDAD-------
			velocidadMaxima=370;
			velocidadMaximaTurbo=415;
			//----GIRO/MANEJO-----
			indiceGiroAlto=btScalar(0.4);
			indiceGiroBajo=btScalar(0.085);
			velocidadLimiteGiro=130;
			//------PESO------
			Masa = btScalar(400);
			//-----HABILIDAD-----
			num=2;
			break;
		case VIKINGO:
			cuboNodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/blue.png"));
			//----ACELERACION-----
			FuerzaMaxima = btScalar(3600); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
			Fuerza = FuerzaMaxima;
			//----VELOCIDAD-------
			velocidadMaxima=360;
			velocidadMaximaTurbo=410;
			//----GIRO/MANEJO-----
			indiceGiroAlto=btScalar(0.4);
			indiceGiroBajo=btScalar(0.1);
			velocidadLimiteGiro=180;
			//------PESO------
			Masa = btScalar(400);
			//-----HABILIDAD-----
			num=3;
			break;
		case CHINO:
			cuboNodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/yellow.png"));
			//----ACELERACION-----
			FuerzaMaxima = btScalar(4000); // valor a cambiar para la aceleracion del pj , a mas valor antes llega a vmax
			Fuerza = FuerzaMaxima;
			//----VELOCIDAD-------
			velocidadMaxima=385;
			velocidadMaximaTurbo=425;
			//----GIRO/MANEJO-----
			indiceGiroAlto=btScalar(0.4);
			indiceGiroBajo=btScalar(0.07);
			velocidadLimiteGiro=110;
			//------PESO------
			Masa = btScalar(1200);
			//-----HABILIDAD-----
			num=4;
			break;
	}
	
}

//-----------------------------//
//----------FISICAS------------//
//-----------------------------//
void Corredor::InicializarFisicas()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	vector<btRigidBody *> objetos = bullet->getObjetos();

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
	localTransform.setOrigin(btVector3(0, 1.5, 0));
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

void Corredor::CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning)
{

	//btVector3 TamanyoFormaColision(1,btScalar(0.5),2);
	btVector3 puntoConexionChasis(cuboNodo->getScale().X - radioRueda, alturaConexionChasis, cuboNodo->getScale().Z - anchoRueda);

	vehiculo->setCoordinateSystem(0, 1, 2); // 0, 1, 2

											// Agrega las ruedas delanteras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, 3)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, 3)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	// Agrega las ruedas traseras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, -3)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);

	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, -3)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);

	for (int i = 0; i < vehiculo->getNumWheels(); i++)
	{
		btWheelInfo &wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = btScalar(20);    // a mas valor mas altura del chasis respecto a las ruedas va en funcion de compresion y relajacion
		wheel.m_wheelsDampingCompression = btScalar(2.3f);//btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f; 
		wheel.m_wheelsDampingRelaxation =  btScalar(4.4f);//btScalar(0.5)* 2 *btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f; 
		wheel.m_frictionSlip = btScalar(10000);  //100;  //conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover 
		wheel.m_rollInfluence = btScalar(0);       //0.1f;  //Empieza a rodar muy loco, si el valor es alto 
		wheel.m_maxSuspensionForce = 30000.f;  //A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa) 
		//wheel.m_maxSuspensionTravelCm = 10; //a menos valor la suspension es mas dura por lo tanto el chasis no baja tanto 										   
	}
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
const char* Corredor::getNombre(){
	return nombre;
}
int Corredor::getID(){
	return id;
}
Corredor::tipo_jugador Corredor::getTipoJugador(){
	return tipojugador;
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
void Corredor::setPosicionCarrera(int i,int j) { 
  if (j==0){ 
    if (vueltas<=maxvueltas) 
    posicionCarrera = i; 
  }else{ 
    posicionCarrera = i; 
  } 
}
void Corredor::setMaxVueltas(int i){
	maxvueltas=i;
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

	//cout << "Posicion: " << posicionCarrera << " - NumRandom: " << random << " - Objeto: " << tipoObj << endl;

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
	if (nodo->getID() > actual->getID() && nodo->getID() == siguiente_aux->getID()) {

		actual = actual->getNextWaypoint();
		siguiente = actual->getNextWaypoint();
		if (actual->getID() == siguiente_aux->getID()){
			siguiente_aux = siguiente;
			b=true;
			direccionContraria=0;
		}
	}

	if (nodo->getID() - 6 == 0) { //caso de vuelta completa

		if (siguiente_aux->getID() - 6 == 0) {
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
	if ((b==false && nodo->getID()<siguiente_aux->getID()-4 && nodo->getID() - 6 != 0) || (mapa->getTamArrayWaypoints()-1 == nodo->getID() - 6 && (actual->getID()-6!=mapa->getTamArrayWaypoints()-1))) {
		direccionContraria=1;
		if ((mapa->getTamArrayWaypoints()-1 == nodo->getID() - 6 && (actual->getID()-6!=mapa->getTamArrayWaypoints()-1))){
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
	Fuerza = btScalar(turbo);
}
void Corredor::setFriccion(btScalar valor) {
	for (int i = 0; i < vehiculo->getNumWheels(); i++) {
		vehiculo->getWheelInfo(i).m_frictionSlip = btScalar(valor);  //100;	//conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover	
	}
}
void Corredor::setProteccion(bool s) {
	//escudo->getNodo()->setVisible(s);
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
void Corredor::setTipoJugador(int tj){
	switch (tj){

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
//-------------------------------------//
//-------TRATAMIENTOS OBJETOS----------//
//-------------------------------------//

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
void Corredor::setID(int i){
	id=i;
	cuboNodo->setID(id);
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
	vector<Item *> items = pista->getItems();
		
		btVector3 posicion(cuboNodo->getPosition().X + orientacion.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 10);
		btVector3 escala(2,2,2);
		btScalar masa=50;
		float tiempoDestruccion=15;
		btVector3 tamanyoNodo(3,3,3);
		btScalar radio=8;
		float alt=1;

	if (getTipoObj() == 1)		// PROYECTIL
	{
	
		Proyectil *pro = new Proyectil(posicion,escala,masa,tiempoDestruccion,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
		pro->lanzarItem(1,orientacion);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		pro->setLanzado(true);
		items.push_back(pro);

		
		soltarItem();
		
	}
	else if (getTipoObj() == 2)	//CAJA FALSA
	{
		alt=-1;
		masa=0;
		posicion.setX(cuboNodo->getPosition().X - orientacion.getX() * 10);
		posicion.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 10);
		CajaFalsa *est = new CajaFalsa(posicion,escala,masa,tiempoDestruccion,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
		items.push_back(est);
		
		soltarItem();
	}
	else if (getTipoObj() == 3)	//TURBO
	{
		setTurbo(true, true, 26000);
	}
	else if (getTipoObj() == 4)	//ACEITE
	{	
		alt=-1;
		masa=0;
		posicion.setX(cuboNodo->getPosition().X - orientacion.getX() * 10);
		posicion.setZ(cuboNodo->getPosition().Z - orientacion.getZ() * 10);
		Aceite *est2 = new Aceite(posicion,escala,masa,tiempoDestruccion,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
		items.push_back(est2);
	
		soltarItem();
	}
	else if (getTipoObj() == 5)	//ESCUDO
	{
		//if (getProteccion() == false) setProteccion(true);
		//decCargador();
		alt=2;
		masa=50;
		radio=8;
		escala = btVector3(10,10,10);
		tamanyoNodo= btVector3(9,9,9);
		tiempoDestruccion=50;
		posicion= btVector3(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
		posicion.setY(posicion.getY()+alt);
		Escudo *escudo = new Escudo(cuboNodo,posicion,escala,masa,tiempoDestruccion,ESFERA,tamanyoNodo,radio,alt,cuboNodo->getID());
		escudo->setLanzado(true);
		items.push_back(escudo);
		soltarItem();
	}
	else if (getTipoObj() == 6)	//FLECHA TRIPLE
	{
		
		btVector3 orientacioncentral(orientacion.getX(), orientacion.getY(), orientacion.getZ());
		btVector3 centro(cuboNodo->getPosition().X + orientacion.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 10);
		btVector3 orientacionderecha = orientacioncentral.rotate(btVector3(0, 1, 0), 10 * PI / 180);
		btVector3 orientacionizquierda = orientacioncentral.rotate(btVector3(0, 1, 0), -10 * PI / 180);
		btVector3 iz(cuboNodo->getPosition().X + orientacionizquierda.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionizquierda.getZ() * 10);
		btVector3 d(cuboNodo->getPosition().X + orientacionderecha.getX() * 10, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacionderecha.getZ() * 10);
		
				posicion=iz;
				Proyectil *pro1= new Proyectil(posicion,escala,masa,tiempoDestruccion,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
				//pro1->inicializarFisicas();
				pro1->lanzarItem(1,orientacionizquierda);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
				pro1->setLanzado(true);
				items.push_back(pro1);

				posicion=centro;
				Proyectil *pro2= new Proyectil(posicion,escala,masa,tiempoDestruccion,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
				//pro2->inicializarFisicas();
				pro2->lanzarItem(1,orientacion);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
				pro2->setLanzado(true);
				items.push_back(pro2);
			
				posicion=d;
				Proyectil *pro3= new Proyectil(posicion,escala,masa,tiempoDestruccion,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
				//pro3->inicializarFisicas();
				pro3->lanzarItem(1,orientacionderecha);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
				pro3->setLanzado(true);
				items.push_back(pro3);
			

				//decCargador();
				soltarItem();
	}
	else if (tipoObj == 7)	//FLECHA TELEDIRIGIDA
	{
		//cout << "Teledirigido de Jugador " << this->getID() << " - posicion: " << posicionCarrera << endl;
		alt=1;
		posicion.setY(posicion.getY()+alt);
		ItemTeledirigido *pt = new ItemTeledirigido(posicion,escala,masa,50,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
		pt->setWaypoint(actual);
		pt->lanzarItem(1,orientacion);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		pt->setLanzado(true);
		items.push_back(pt);
		soltarItem();
		
	}
	else if(tipoObj == 8)	//TURBO TRIPLE
	{
		setTurbo(true, true, 26000);
	}
	std::cout << "Tipo obj: " << getTipoObj() << " / " << items.size() << std::endl;
	
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

	btVector3 posCoche(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z);
	btVector3 posWaypoint(vector.getX(), vector.getY(), vector.getZ());

	return posCoche.distance2(posWaypoint);

}

//----------------------------------------//
//------------MOVIMIENTO------------------//
//----------------------------------------//
void Corredor::acelerar()
{
	if((vehiculo->getCurrentSpeedKmHour()>velocidadMaxima && !turboActivado) || (turboActivado && vehiculo->getCurrentSpeedKmHour()>velocidadMaximaTurbo)){
		limitadorVelocidad();

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
		FuerzaGiro =indiceGiroAlto;
	}else{
		FuerzaGiro =indiceGiroBajo;
	}

	vehiculo->setSteeringValue(FuerzaGiro, 0);
	vehiculo->setSteeringValue(FuerzaGiro, 1);

}
void Corredor::girarIzquierda()
{
	estado->setDireccionMovimiento(IZQUIERDA);
	if(vehiculo->getCurrentSpeedKmHour()<velocidadLimiteGiro){
		FuerzaGiro = indiceGiroAlto;
	}else{
		FuerzaGiro = indiceGiroBajo;
	}
	vehiculo->setSteeringValue(-FuerzaGiro, 0);
	vehiculo->setSteeringValue(-FuerzaGiro, 1);
	
}
void Corredor::frenodemano(bool activo, bool objeto)
{
	int friccion = 1.f;
	if (activo) {
		if(estado->getEstadoMovimiento() != 0 && estado->getEstadoMovimiento() != 4 && (estado->getDireccionMovimiento()==1 || estado->getDireccionMovimiento()==2) && !objeto){
			limite+=10;
		}

		estado->setEstadoMovimiento(DERRAPA);
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
void Corredor::desacelerar()
{
	estado->setEstadoMovimiento(DESACELERA);
	limitadorVelocidad();

	vehiculo->setSteeringValue(0, 0);
	vehiculo->setSteeringValue(0, 1);

	vehiculo->setBrake(60, 0);
	vehiculo->setBrake(60, 1);
	vehiculo->setBrake(60, 2);
	vehiculo->setBrake(60, 3);
}
void Corredor::limitadorVelocidad(){
	vehiculo->applyEngineForce(0, 0);
	vehiculo->applyEngineForce(0, 1);
	vehiculo->applyEngineForce(0, 2);
	vehiculo->applyEngineForce(0, 3);
}
void Corredor::comprobarSueloRuedas()
{
/*
	int cont=0;
	for(int i = 0; i < vehiculo->getNumWheels(); i++){
	if(!vehiculo->getWheelInfo(i).m_raycastInfo.m_isInContact){
		
		cont++;
		//cout<<"LA RUEDA :: "<< i << "NO ESTA EN CONTACTO"<<endl;
	}	
	
	}	

	if(cont==4){

	}
*/
}

void Corredor::recolocarWaypoint(){
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
	if((vehiculo->getCurrentSpeedKmHour()>velocidadMaxima && !turboActivado) || (turboActivado && vehiculo->getCurrentSpeedKmHour()>velocidadMaximaTurbo)){
		limitadorVelocidad();
	}
	for (unsigned int i = 0; i< sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
			std::cout << "Joystick " << i << " is connected!" << std::endl;
	}
	Timer *time = Timer::getInstancia();
	if (turboActivado) {
		if (time->getTimer() - timerTurbo >= 1) {
			//cout << "Se acaba el turbo\n";
			desacelerar();
			setTurbo(false, false, 0);
		}
	}
	if (estado->getEstadoCarrera()!=PARRILLA){
		movimiento();
		updateEstado();
	}
	updateTimerObstaculos();
	updateEstado();
	//if (habilidadJugador->getHabilidadActiva())updateHabilidad();
	
	comprobarSueloRuedas();
	posicion.setX(cuboNodo->getPosition().X);
	posicion.setY(cuboNodo->getPosition().Y);
	posicion.setZ(cuboNodo->getPosition().Z);
	actualizarRuedas();
	updateVectorDireccion();
	distanciaWaypoint = getDistanciaPunto(siguiente->getPosicion());
	distanciaWaypointActual = getDistanciaPunto(actual->getPosicion());
	updateText();
	updateHijos();
	
}

void Corredor::updateText(){
	TextoPantalla *texto =TextoPantalla::getInstancia();
	texto->agregar("---------------------- \n");
	texto->agregar("Jugador ");
	texto->agregar(to_string(this->getID()));
	//texto->agregar("VELOCIDAD: ");
	//texto->agregar(to_string(vehiculo->getCurrentSpeedKmHour()));
	if (direccionContraria!=0){
		texto->agregar("\nVAS EN DIRECCION CONTRARIA, JUGADOR: ");
		texto->agregar(to_string(this->getID())+"\n");
	}
        texto->agregar(" SIGUIENTE WAY: ");
        texto->agregar(to_string(siguiente->getID()-6)+"\n");
	texto->agregar(" - POSICION: ");
	texto->agregar(to_string(posicionCarrera));
	texto->agregar(" - VUELTA: ");
	if (vueltas>maxvueltas){
		texto->agregar("HA LLEGADO ");
		texto->agregar(to_string(posicionCarrera)+"!\n");
	}else{
		texto->agregar(to_string(vueltas));
	}
	texto->agregar("\nOBJETO: ");
	switch (tipoObj) {
	case 0: 
		texto->agregar("NADA\n");
		break;
	case 1:
		texto->agregar("FLECHA\n");
		break;
	case 2:
		texto->agregar("CAJA FALSA\n");
		break;
	case 3:
		texto->agregar("TURBO\n");
		break;
	case 4:
		texto->agregar("ACEITE\n");
		break;
	case 5:
		texto->agregar("ESCUDO\n");
		break;
	case 6:
		texto->agregar("FLECHA TRIPLE\n");
		break;
	case 7:
		texto->agregar("FLECHA TELEDIRIGIDA\n");
		break;
	case 8:
		texto->agregar("TURBO TRIPLE\n");
		break;
	}
	texto->agregar("Habilidad: "+ to_string(limite) + "/100\n");
}

void Corredor::updateTimerObstaculos() {

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

void Corredor::lanzarHabilidad(){
	if (getLimite() >= 100) {//puedo lanzar la habilidad
		Pista *pista = Pista::getInstancia();
		vector<Item *> items = pista->getItems();
		
		btVector3 posicion(cuboNodo->getPosition().X + orientacion.getX() * 20, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z + orientacion.getZ() * 20);
		btVector3 escala(5,5,5);
		btScalar masa=100;
		float tiempoDestruccion=40;
		btVector3 tamanyoNodo(3,3,3);
		btScalar radio=12;
		float alt=10;

		Habilidad * habilidadJugador;

		switch(tipojugador){

		case PIRATA:	
		
		masa=100;
		radio=12;
		alt=10;

		posicion.setY(posicion.getY()+alt);
		habilidadJugador = new Habilidad(1,cuboNodo,posicion,escala,masa,tiempoDestruccion,ESFERA,tamanyoNodo,radio,alt,cuboNodo->getID());
		habilidadJugador->lanzarItem(1,orientacion);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		habilidadJugador->setLanzado(true);
		
		break;

		case VIKINGO:
		escala = btVector3(3,3,3);	
		masa=0;
		radio=8;
		alt=2;	

		posicion.setY(posicion.getY()+alt);
		habilidadJugador = new Habilidad(2,cuboNodo,posicion,escala,masa,tiempoDestruccion,ESFERA,tamanyoNodo,radio,alt,cuboNodo->getID());
		habilidadJugador->lanzarItem(1,orientacion);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		habilidadJugador->setLanzado(true);	
		break;

		case GLADIADOR:
		habilidadJugador = new Habilidad(3,cuboNodo,posicion,escala,masa,tiempoDestruccion,ESFERA,tamanyoNodo,radio,alt,cuboNodo->getID());
		habilidadJugador->lanzarItem(1,orientacion);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		habilidadJugador->setLanzado(true);
		break;

		case CHINO:
		posicion = btVector3(cuboNodo->getPosition().X , cuboNodo->getPosition().Y, cuboNodo->getPosition().Z );
		tamanyoNodo = btVector3(2,2,2);
		escala = btVector3(5,7,10);
		masa=100;	
		alt=2;

		posicion.setY(posicion.getY()+alt);
		habilidadJugador = new Habilidad(4,cuboNodo,posicion,escala,masa,tiempoDestruccion,CUBO,tamanyoNodo,radio,alt,cuboNodo->getID());
		habilidadJugador->lanzarItem(1,orientacion);// por defecto sera siempre 1, (cambiar esto para eliminarlo del constructor) PENDIENTE
		habilidadJugador->setLanzado(true);	
		break;

			


		}
		items.push_back(habilidadJugador);

		pista->setItems(items);
		setLimite(0);

	}
	else {
		cout << "No puedes usar la habilidad si tu limite no es 100 o mas" << endl;
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
	//cout<< "ORIENTACION XNORMAL COCHE=="<< orientacion.getX() << " ORIENTACION ZNORMAL COCHE=="<< orientacion.getZ()  << endl;
}
//---------------------------------------//
//--------------DESTRUCTOR---------------//
//---------------------------------------//
Corredor::~Corredor() {
	cout << "\nENTRO DESTRUCTOR CORREDOR. ";
	delete vehiculo;
	cout << "SALGO DESTRUCTOR CORREDOR\n";
}
		