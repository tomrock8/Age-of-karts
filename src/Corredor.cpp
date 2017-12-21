#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include <string>

//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*
Corredor::Corredor(stringw rutaObj, vector3df pos)
{
	cargador = 10;
	Motor3d *m = Motor3d::getInstancia();
	ISceneManager *smgr = m->getScene();
	coche = smgr->getMesh(rutaObj);
	cuboNodo = smgr->addMeshSceneNode(coche, 0);
	//cuboNodo = smgr->addCubeSceneNode(1.5f);
	cuboNodo->setName("Jugador");
	//cambiar a color rojo del coche
	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
	cuboNodo->setScale(vector3df(1, 1, 1.5));
	// Desactivar la iluminacion del cubo
	cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	cuboNodo->setPosition(pos);
	posicion.setX(pos.X);
	posicion.setY(pos.Y);
	posicion.setZ(pos.Z);
	//-------------bullet----------------
	rueda1 = smgr->addCubeSceneNode(1.f);
	rueda2 = smgr->addCubeSceneNode(1.f);
	rueda3 = smgr->addCubeSceneNode(1.f);
	rueda4 = smgr->addCubeSceneNode(1.f);

	//smgr->getMeshManipulator()->setVertexColors(rueda1->getMesh(),SColor(255, 255, 0, 0));
	rueda1->setMaterialFlag(EMF_LIGHTING, false);
	rueda2->setMaterialFlag(EMF_LIGHTING, false);
	rueda3->setMaterialFlag(EMF_LIGHTING, false);
	rueda4->setMaterialFlag(EMF_LIGHTING, false);

	direccionRuedas = btVector3(0, -1, 0);
	rotacionRuedas = btVector3(-1, 0, 0);
	suspension = btScalar(1.0); // Este valor tiene que ser ese... sino peta
	Fuerza = btScalar(6000);
	anchoRueda = btScalar(0.4);			  //0.4
	radioRueda = btScalar(0.5);			  //No menor de 0.4 sino ni se mueve (ruedas pequenyas)
	alturaConexionChasis = btScalar(1.2); //influye mucho en la acceleracion de salida
	Masa = btScalar(1100);
	FuerzaFrenado = btScalar(-10000);
	FuerzaGiro = btScalar(0.3); //manejo a la hora de girar
	FuerzaFrenoMano = btScalar(800);
	FuerzaFrenadoReposo = btScalar(60);
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

	//Motionstate
	motionStateCoche = new btDefaultMotionState(transCoche); //motionState = interpolacion

	//establecemos su centro de gravedad
	btTransform localTransform;
	localTransform.setIdentity();
	localTransform.setOrigin(btVector3(0, 1, 0));
	CentroGravedad = new btCompoundShape();

	//Forma Colision
	btVector3 TamanyoFormaColision(cuboNodo->getScale().X, cuboNodo->getScale().Y, cuboNodo->getScale().Z);
	//btVector3 TamanyoFormaColision(1,btScalar(0.5),2);
	FormaColision = new btBoxShape(TamanyoFormaColision);
	//masa coche
	Masa = 2000;
	btVector3 Inercia(0, 0, 0);
	FormaColision->calculateLocalInertia(Masa, Inercia);

	CentroGravedad->addChildShape(localTransform, FormaColision);

	//rigidbody del coche
	CuerpoColisionChasis = new btRigidBody(Masa, motionStateCoche, CentroGravedad, Inercia);

	CuerpoColisionChasis->setUserPointer((void *)(cuboNodo));

	//RaycastDel Coche
	btVehicleRaycaster *RayCastVehiculo = new btDefaultVehicleRaycaster(mundo);
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

void Corredor::CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning)
{

	btVector3 direccionRuedas(0, -1, 0);

	btVector3 rotacionRuedas(-1, 0, 0);

	btScalar suspension(0.9);

	btScalar anchoRueda(0.4);

	btScalar radioRueda(0.5);

	btScalar alturaConexionChasis(1.2);

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
		wheel.m_suspensionStiffness = 60;	  //tambaleo de las ruedas (se mueve como si fuera por terreno con baches). A mayor valor mayor tambaleo
		wheel.m_wheelsDampingCompression = 20; //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f;
		wheel.m_wheelsDampingRelaxation = 20;  //btScalar(0.5)*2*btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f;
		wheel.m_frictionSlip = btScalar(1.2);  //100;	//conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover
		wheel.m_rollInfluence = 0.1;		   //0.1f;	//Empieza a rodar muy loco, si el valor es alto
		//wheel.m_maxSuspensionForce = 40000.f;	//A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa)
		wheel.m_maxSuspensionTravelCm = 8000.f; //Nose muy bien que funcion tiene, pero si el valor es muy bajo el coche no avanza

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

void Corredor::setTipoObj()
{
	srand(time(NULL));
	tipoObj = rand() % 2 + 1;
	cout << "Random ------>"<< tipoObj << endl;
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
	std::cout << "Disparo " << std::endl;
	tipoObj = 0;
	decCargador();
}

void Corredor::soltarItem(Estatico *item)
{
	tipoObj = 0;
}

void Corredor::SetFuerzaVelocidad(int turbo)
{
	Fuerza = turbo;
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
void Corredor::frenodemano()
{
	vehiculo->setBrake(FuerzaFrenoMano, 2);
	vehiculo->setBrake(FuerzaFrenoMano, 3);
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

	return text;
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
	movimiento();
	posicion.setX(cuboNodo->getPosition().X);
	posicion.setY(cuboNodo->getPosition().Y);
	posicion.setZ(cuboNodo->getPosition().Z);
	actualizarRuedas();
	updateDireccion();
}

/**
 * Identifica la rotacion del coche en grados
*/
void Corredor::updateDireccionGrados()
{
	btTransform centerOfMassWorldTrans;
	motionStateCoche->getWorldTransform(centerOfMassWorldTrans);

	float radianes = centerOfMassWorldTrans.getRotation().getY(); //ROTACION OBTENIDA
	float grados = radianes * 180;

	if (grados < 0)
	{
		grados = 180 + (180 + grados);
	}

	direccionGrados = grados;
	//cout<<"X: "<<direccionGrados<<endl;
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
	updateDireccionGrados();

	// NORTE
	if (direccionGrados <= 20 && direccionGrados >= 341)
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