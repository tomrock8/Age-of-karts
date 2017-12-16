#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include <string>

//-------------------------
//	CONSTRUCTOR CORREDOR
//-------------------------
Corredor::Corredor(stringw rutaObj, vector3df pos)
{

	Motor3d *m = Motor3d::getInstancia();
	ISceneManager *smgr = m->getScene();
	coche = smgr->getMesh(rutaObj);
	cuboNodo = smgr->addMeshSceneNode(coche, 0);
	cuboNodo->setName("Jugador");

	//cambiar a color rojo del coche
	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
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
{
	return cuboNodo;
}

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

void Corredor::update()
{
	movimiento();
	Pedazodemierda();
	actualizarRuedas();
	updateDireccion();
}

//Identifica la rotacion del coche en grados
void Corredor::updateDireccionGrados()
{
	float grados = cuboNodo->getRotation().Y * 180 / PI; //cambiar rotacion

	cout << "ROTATION: " << cuboNodo->getRotation().Y << " == GRADOS: " << grados << endl;
	/*
	if (grados < 0) {
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
