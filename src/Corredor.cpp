#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
<<<<<<< HEAD
#include <string>

//-------------------------
//	CONSTRUCTOR CORREDOR
//-------------------------
Corredor::Corredor(stringw rutaObj, vector3df pos)
{
=======
#include "AxesSceneNode.cpp"
using namespace irr;

//enum
//{
//	// No colisionable, para evitar cogerlo por error
//	ID_NULO = 0,
//
//	// Objetos que pueden colisionar
//	ID_COLISION = 1 << 0,
//
//	// Objetos para iluminar
//	ID_ILUMINAR = 1 << 1
//};
//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*
Corredor::Corredor(ISceneManager *smgr, stringw rutaObj,  btVector3 posCoche)
{				  
	//aceleraciones
	aceleracion = 0.1;   //aceleracion eje Z
	aceleracionInversa = 0.01; //marcha atras
	aceleracionFrenado = 0.15; //aceleracion eje X
	tiempo = 0.5;

	// velocidades
	velocidadIni = 0;
	velocidadX = 0;
	velocidadZ = 0;
	velocidad = 0;
	velocidadMax = 1.5;

	//posiciones
	posXIni = 0;
	posZIni = 0;
	posYIni = 0;
	posX = 0;
	posZ = 0;
	posY = 0;
	//rotaciones
	Rotacionruedas = 3;
	Rotacioncoche = 2;
	rotRuedasY = 0;
	rotCocheY = 0;
	anguloMaxRuedas = 50;
	anguloMaxCoche = 180;

	// booleanos
	adelante = false;
	atras = false;
	// la escala siempre sera 1:1:1
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.

	coche = smgr->getMesh(rutaObj);
	cuboNodo = smgr->addMeshSceneNode(coche, 0);
	cuboNodo->setName("Jugador");

	//cambiar a color rojo del coche

	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
<<<<<<< HEAD
	cuboNodo->setScale(vector3df(1, 1, 1));

	// Desactivar la iluminacion del cubo
	cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	cuboNodo->setPosition(pos);

	//CREAMOS LAS RUEDAS
	rueda1 = smgr->addCubeSceneNode(1.f);
	rueda2 = smgr->addCubeSceneNode(1.f);
	rueda3 = smgr->addCubeSceneNode(1.f);
	rueda4 = smgr->addCubeSceneNode(1.f);

	rueda1->setMaterialFlag(EMF_LIGHTING, false);
	rueda2->setMaterialFlag(EMF_LIGHTING, false);
	rueda3->setMaterialFlag(EMF_LIGHTING, false);
	rueda4->setMaterialFlag(EMF_LIGHTING, false);

	direccionRuedas = btVector3(0, -1, 0);
	rotacionRuedas = btVector3(-1, 0, 0);
	suspension = btScalar(0.9);
	Fuerza = btScalar(10000);
	anchoRueda = btScalar(0.4);
	radioRueda = btScalar(0.5);
	alturaConexionChasis = btScalar(1.2);
	Masa = btScalar(2000);
	FuerzaFrenado = btScalar(-15000);
	FuerzaGiro = btScalar(0.3);
	FuerzaFrenoMano = btScalar(500);
	FuerzaFrenadoReposo = 20;
}

void Corredor::InicializarFisicas(list<btRigidBody *> &objetos, btDiscreteDynamicsWorld *mundo)
{
	//posicion inicial
	btTransform PosInicial;
	PosInicial.setIdentity();
	PosInicial.setOrigin(btVector3(cuboNodo->getPosition().X, cuboNodo->getPosition().Y, cuboNodo->getPosition().Z));

	//Motionstate
	motionStateCoche = new btDefaultMotionState(PosInicial); //motionState = interpolacion

	//establecemos su centro de gravedad
	btTransform localTransform;
	localTransform.setIdentity();
	localTransform.setOrigin(btVector3(0, 1, 1));
	CentroGravedad = new btCompoundShape();

	//Forma Colision
	btVector3 TamanyoFormaColision(cuboNodo->getScale().X, cuboNodo->getScale().Y, cuboNodo->getScale().Z);
	FormaColision = new btBoxShape(TamanyoFormaColision);
	//masa coche

	btVector3 Inercia(0, 0, 0);
	FormaColision->calculateLocalInertia(Masa, Inercia);

	CentroGravedad->addChildShape(localTransform, FormaColision);

	//rigidbody del coche
	CuerpoColisionChasis = new btRigidBody(Masa, motionStateCoche, CentroGravedad, Inercia);

	//Asignamos el nodo irrlich al rigidbody
	CuerpoColisionChasis->setUserPointer((void *)(cuboNodo));

	//Rigidbody en el mundo
	mundo->addRigidBody(CuerpoColisionChasis);

	//Raycast del coche
	btVehicleRaycaster *RayCastVehiculo = new btDefaultVehicleRaycaster(mundo);
	btRaycastVehicle::btVehicleTuning tuning;

	//se crea el coche y se le pasa tunin y raycast
	vehiculo = new btRaycastVehicle(tuning, CuerpoColisionChasis, RayCastVehiculo);

	//se establece el sistemas de coordenadas
	vehiculo->setCoordinateSystem(0, 1, 2); // 0, 1, 2

	//actualizamos este cuerpo siempre para la fisicas
	CuerpoColisionChasis->setActivationState(DISABLE_DEACTIVATION);

	//se pone el coche en el mundo
	mundo->addVehicle(vehiculo);

	//metemos el rigidbody en el array para actualizar el nodo
	objetos.push_back(CuerpoColisionChasis);

	CrearRuedas(vehiculo, tuning);
}

void Corredor::BorrarFisicas()
{
}

void Corredor::CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning)
{
	btVector3 puntoConexionChasis(cuboNodo->getScale().X - radioRueda, alturaConexionChasis, cuboNodo->getScale().Z - anchoRueda);

	// Agrega las ruedas delanteras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, 2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);
	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, 2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	// Agrega las ruedas traseras
	vehiculo->addWheel((puntoConexionChasis * btVector3(4, 1, -2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);
	vehiculo->addWheel((puntoConexionChasis * btVector3(-4, 1, -2)), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false);

	for (int i = 0; i < vehiculo->getNumWheels(); i++)
	{
		btWheelInfo &wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = 40;
		wheel.m_wheelsDampingCompression = 2.3f;
		wheel.m_wheelsDampingRelaxation = 4.4f;
		wheel.m_frictionSlip = 100;
		wheel.m_rollInfluence = 0.1f;
		//wheel.m_maxSuspensionForce = 2000.f;
		//wheel.m_maxSuspensionTravelCm = 10;
	}
}

btRaycastVehicle *Corredor::getVehiculo()
{
	return vehiculo;
}

btRigidBody *Corredor::getRigidBody()
{
	return CuerpoColisionChasis;
}

IMeshSceneNode *Corredor::getNodo()
=======
	// Desactivar la iluminacion del cubo
	if (cuboNodo)
	{
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(vector3df(230, 20, 0));
		//-------------bullet----------------
		//posicion inicial
		transCoche;
		transCoche.setIdentity();
		transCoche.setOrigin(posCoche);

		//dar motionstate
		motionStateCoche = new btDefaultMotionState(transCoche);//motionState = interpolacion
		//contorno
		btVector3 extensionCoche(0.5f,0.5f,0.5f);
		contorno = new btBoxShape(extensionCoche);
		//masa coche
		localInertia;
		masa = 150;
		contorno->calculateLocalInertia(masa, localInertia);

		//rigidbody del coche
		rigidBody = new btRigidBody(masa, motionStateCoche, contorno, localInertia);
		//almacenar el puntero al nodo irrlicht  para poder actualizar sus valores
		rigidBody->setUserPointer((void *)(cuboNodo));


	}

	// una vez hemos establecido la posicion del coche establecemos sus posiciones x,z,y
	posX = cuboNodo->getPosition().X;
	posZ = cuboNodo->getPosition().Z;
	posY = cuboNodo->getPosition().Y;

	// luego declaramos sus ruedas
	ruedasDelanteras = smgr->addCubeSceneNode(0.5f);
	ruedasTraseras = smgr->addCubeSceneNode(0.5f);
	ruedasDelanteras->setParent(cuboNodo);
	ruedasTraseras->setParent(cuboNodo);
	// inicializamos la posicion de las ruedas
	ruedasDelanteras->setPosition(vector3df(-1.2, -0.5, 1));
	ruedasTraseras->setPosition(vector3df(-1.2, -0.5, -1));
}
//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
float Corredor::movimiento(float pos, float vel, float accel, float delta)
{
	pos = pos + vel * delta + (0.5 * accel * (exp2(delta))); //donde el 0 es la velocidad inicial
	return pos;
}
//-----------------------\*
//-------POSICION--------\*
//-----------------------\*

void Corredor::updatePosicion()
{
	posY = cuboNodo->getPosition().Y;
	//posX+=cuboNodo->getPosition().X;
	//posZ+=cuboNodo->getPosition().Z;
	//posX+=this->getPosicion().X;
	//posY+=this->getPosicion().Y;
	//posZ+=this->getPosicion().Z;
	cuboNodo->setPosition(vector3df(posX, posY, posZ));
	//ruedasDelanteras->setPosition(vector3df(posX - 7, posY - 2 , posZ + 4));
	//ruedasTraseras->setPosition(vector3df(posX - 7, posY - 2 , posZ - 4));
}

void Corredor::updateVelocidadInicial()
{
	velocidadIni = velocidad;
}

void Corredor::updatePosicionInicial()
{
	posXIni = posX;
	posZIni = posZ;
}

void Corredor::update()
{
	updatePosicion();
	updateVelocidadInicial();
	updatePosicionInicial();
}

void Corredor::setAxis(ISceneManager *smgr)
{

	//AxesSceneNode *axis = new AxesSceneNode(cuboNodo, smgr, -1);
	//axis->setAxesScale(20); //  for the length of the axes
	//axis->drop();
	
}

float Corredor::getVelocidad()
{
	return velocidad;
}
vector3df Corredor::getPosicion()
{
	return cuboNodo->getPosition();
}

vector3df Corredor::getRotacion()
{
	return cuboNodo->getRotation();
}

IMeshSceneNode* Corredor::getNodo()
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.
{
	return cuboNodo;
}

<<<<<<< HEAD
void Corredor::Pedazodemierda()
{
	orientacion = vector3df(sin((cuboNodo->getRotation().Y) * PI / 180.0f), 0, cos((cuboNodo->getRotation().Y) * PI / 180.0f));
	//cout<< "Rotacion en Y=="<< cuboNodo->getRotation().Y << endl;
	//cout<< "ORIENTACION X=="<< orientacion.X << "ORIENTACION Z==" << orientacion.Z << endl;
	orientacion.normalize();
	//cout<< "ORIENTACION XNORMAL=="<< orientacion.X << "ORIENTACION ZNORMAL==" << orientacion.Z << endl;
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

void Corredor::acelerar()
{
	vehiculo->applyEngineForce(Fuerza, 2);
	vehiculo->applyEngineForce(Fuerza, 3);
	vehiculo->setSteeringValue(btScalar(0), 0);
	vehiculo->setSteeringValue(btScalar(0), 1);
}

void Corredor::frenar()
{
	vehiculo->applyEngineForce(FuerzaFrenado, 2);
	vehiculo->applyEngineForce(FuerzaFrenado, 3);
	vehiculo->setSteeringValue(btScalar(0), 0);
	vehiculo->setSteeringValue(btScalar(0), 1);
}

void Corredor::girarDerecha()
{
	vehiculo->setSteeringValue(FuerzaGiro, 0);
	vehiculo->setSteeringValue(FuerzaGiro, 1);
}

void Corredor::girarIzquierda()
{
	vehiculo->setSteeringValue(-FuerzaGiro, 0);
	vehiculo->setSteeringValue(-FuerzaGiro, 1);
}

void Corredor::desacelerar()
{
	vehiculo->applyEngineForce(0, 2);
	vehiculo->applyEngineForce(0, 3);

	vehiculo->setSteeringValue(0, 0);
	vehiculo->setSteeringValue(0, 1);

	vehiculo->setBrake(60, 2);
	vehiculo->setBrake(60, 3);
}

void Corredor::frenodemano()
{
	vehiculo->setBrake(300, 2);
	vehiculo->setBrake(300, 3);
}

//	----------------------------------------------
//		METODOS GET
//	----------------------------------------------

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
=======
//-----------------------\*
//---MOVIMIENTO JUGADOR--\*
//-----------------------\*
void Corredor::acelerar()
{

	//cout<<"velocidad marcha adelante: "<<v<<"  "<<endl;

	//esto da un ligero retroceso al arrancar de nuevo (sin ser excesivo con v=-2,5)
	if (velocidadIni < 0)
	{
		velocidadIni = 0;
	}

	//variamos la velocidad en funcion de la acceleracion (sin pasar de la velocidad max=5)
	if (velocidad < velocidadMax)
	{
		velocidad = velocidadIni + aceleracion * tiempo;
	}
	//calculamos el desplazamiento en z y x, en funcion del angulo (radianes)
	posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
	posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
	atras = false;
	adelante = true;
}

//-----------------------\*
//---FRENADO JUGADOR---\*
//-----------------------\*
void Corredor::frenar()
{
	if (adelante == true)
	{
		posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
		posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
		if (velocidad > -2.5f)
		{ //Controla que no pase de 2.5 de velocidad en marcha atras cuando se frena
			velocidad = velocidadIni - aceleracionFrenado * tiempo;
			//	v = v -0.3;
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.
		}
		//v = vIni -Afrenado*t;
	}
	else
	{

<<<<<<< HEAD
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
=======
		//cout<<"velocidad marcha atras: "<<v<<"  "<<endl;
		atras = true;
		adelante = false;
		if (velocidad > -2.5f && velocidad < 2.5f)
		{
			//v = vIni + Afrenado*t;
			velocidad = velocidadIni - aceleracionInversa * tiempo;
			//	v = v -0.3;
		}

		posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
		posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
	}
}
//-----------------------\*
//------GIRAR JUGADOR----\*
//-----------------------\*
void Corredor::girarDerecha()
{
	if (velocidad >= 0.5 || velocidad <= -0.5) {
		rotCocheY += Rotacioncoche;

		if (rotCocheY > 180) {
			rotCocheY = -179;
		}
	}
	/*
	if(rotCocheY > anguloMaxCoche)
	{
		rotCocheY=anguloMaxCoche;
	}
*/
	if (velocidad < 0) {
		rotRuedasY -= Rotacionruedas;
		if (rotRuedasY < -anguloMaxRuedas)
		{
			rotRuedasY = -anguloMaxRuedas;
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.
		}
	}
	else {
		rotRuedasY += Rotacionruedas;
		if (rotRuedasY > anguloMaxRuedas)
		{
			rotRuedasY = anguloMaxRuedas;
		}

<<<<<<< HEAD
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

int Corredor::getDireccionGrados()
{
	return direccionGrados;
}

std::string Corredor::toString()
{
	std::string text = "";

	//Mostrar la Posicion y Velocidad actuales.
	/*
	text += "\n Velocidad: ";
	text += to_string(getVelocidad());
	*/
	text += "\n Posicion [";
	text += to_string(cuboNodo->getPosition().X);
	text += ", ";
	text += to_string(cuboNodo->getPosition().Y);
	text += ", ";
	text += to_string(cuboNodo->getPosition().Z);
	text += "]\n";
	text += " Direccion: ";
	text += getDireccion();
	text += " [ ";
	text += to_string(getDireccionGrados());
	text += " ]\n";

	return text;
}

//	----------------------------------------------
//		METODOS UPDATE
//	----------------------------------------------
=======
	}
	ruedasDelanteras->setRotation(vector3df(0, rotRuedasY, 0));
	cuboNodo->setRotation(vector3df(0, rotCocheY, 0));
}

void Corredor::girarIzquierda()
{
	if (velocidad >= 0.5 || velocidad <= -0.5) {
		rotCocheY -= Rotacioncoche;
		if (rotCocheY < -180) {
			rotCocheY = 179;
		}
	}
	/*
	if(rotCocheY < -anguloMaxCoche)
	{
		rotCocheY=-anguloMaxCoche;
	}
*/
	if (velocidad < 0) {
		rotRuedasY += Rotacionruedas;
		if (rotRuedasY > anguloMaxRuedas)
		{
			rotRuedasY = anguloMaxRuedas;
		}
	}
	else {
		rotRuedasY -= Rotacionruedas;
		if (rotRuedasY < -anguloMaxRuedas)
		{
			rotRuedasY = -anguloMaxRuedas;
		}
	}
	ruedasDelanteras->setRotation(vector3df(0, rotRuedasY, 0));
	cuboNodo->setRotation(vector3df(0, rotCocheY, 0));
}

void Corredor::resetGiro()
{
	if (rotRuedasY > 0) {
		rotRuedasY -= Rotacionruedas + 0.5;
	}
	if (rotRuedasY < 0) {
		rotRuedasY += Rotacionruedas + 0.5;
	}
	ruedasDelanteras->setRotation(vector3df(0, rotRuedasY, 0));
}

//-----------------------\*
//---DESACELERAR JUGADOR--\*
//-----------------------\*
void Corredor::desacelerar()
{
	//desaceleracion
	//X = Xi + Vi . t - 1/2 . a . t²
	//V = Vi - a . t
	velocidad = velocidadIni - aceleracionInversa * tiempo;
	if (velocidad > 0)
	{
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.

void Corredor::update()
{
	movimiento();
	Pedazodemierda();
	actualizarRuedas();
	updateDireccion();
}

<<<<<<< HEAD
//Identifica la rotacion del coche en grados
void Corredor::updateDireccionGrados()
{
	//cout << "ORI Y; " << orientacion.getSphericalCoordinateAngles().Y << " --- " << orientacion.getAs3Values().Y << endl;
	float grados = orientacion.getSphericalCoordinateAngles().Y * 2; //ROTACION OBTENIDA
	
	// ----------------------------
	// 	PROBLEMON: 0 grados es a la derecha, no enfrente D:
	// ----------------------------

	//cout << "ROTATION: " << cuboNodo->getRotation().Y << " == GRADOS: " << grados << endl;
	/*
	if (grados < 0)
	{
		grados = 180 + (180 + grados);
	}
	*/
	direccionGrados = grados;
}

/*
Actualiza la posicion hacia la que mira el corredor
Tabla de grados -
	Norte    - 000 [341-20]
	Noreste  - 045 [21-70]
	Este	 - 090 [71-110]
	SurEste  - 135 [111-160]
	Sur		 - 180 [161-200]
	SurOeste - 225 [201-250]
	Oeste    - 270 [251-290]
	NorOeste - 315 [291-340]
*/
void Corredor::updateDireccion()
{
	updateDireccionGrados();
	// NORTE
	if (direccionGrados <= 20 || direccionGrados >= 341)
	{
		norte = true;
		sur = false;
		este = false;
		oeste = false;
	}
	else
	{
		// NORESTE
		if (direccionGrados >= 21 && direccionGrados <= 70)
		{
			norte = true;
			sur = false;
			este = true;
			oeste = false;
		}
		else
		{
			// ESTE
			if (direccionGrados >= 71 && direccionGrados <= 110)
			{
				norte = false;
				sur = false;
				oeste = false;
				este = true;
			}
			else
			{
				// SURESTE
				if (direccionGrados >= 111 && direccionGrados <= 160)
				{
					norte = false;
					sur = true;
					este = true;
					oeste = false;
				}
				else
				{
					// SUR
					if (direccionGrados >= 161 && direccionGrados <= 200)
					{
						norte = false;
						sur = true;
						este = false;
						oeste = false;
					}
					else
					{
						// SUROESTE
						if (direccionGrados >= 201 && direccionGrados <= 250)
						{
							norte = false;
							sur = true;
							este = false;
							oeste = true;
						}
						else
						{
							// OESTE
							if (direccionGrados >= 251 && direccionGrados <= 290)
							{
								norte = false;
								sur = false;
								este = false;
								oeste = true;
							}
							else
							{
								// NOROESTE
								if (direccionGrados >= 291 && direccionGrados <= 340)
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
=======
		if (adelante)
		{ //desacelerar una vez el coche iba hacia adelante
			posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
			posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
		}
	}
	else if (velocidad<0 && velocidad>-2.6) {
		velocidad = 0;
		if (atras)
		{ //esta parte no se si entra alguna vez XD

			posZ = posZIni - velocidad * cos((PI / 180) * (rotCocheY));
			posX = posXIni - velocidad * sin((PI / 180) * (rotCocheY));
		}
	}
}
void Corredor::escalar(float tam)
{
	core::vector3d<f32> factorEscalado(tam, tam, tam);
	cuboNodo->setScale(factorEscalado);
}
btRigidBody* Corredor::getRigidBody(){
	return rigidBody;
}
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.
