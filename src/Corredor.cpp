#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "AxesSceneNode.cpp"
#include <string>


//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*
Corredor::Corredor(stringw rutaObj,vector3df pos)
{

	Motor3d *m = Motor3d::getInstancia();
	ISceneManager *smgr = m->getScene();
	coche = smgr->getMesh(rutaObj);
	cuboNodo = smgr->addMeshSceneNode(coche, 0);
	//cuboNodo = smgr->addCubeSceneNode(1.5f);
	cuboNodo->setName("Jugador");
	//cambiar a color rojo del coche
	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
	cuboNodo->setScale(vector3df(1,1,1));
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

	direccionRuedas = btVector3(0,-1,0);
	rotacionRuedas= btVector3(-1,0,0);
	suspension=btScalar(1.0);	// Este valor tiene que ser ese... sino peta
	Fuerza=btScalar(6000);
	anchoRueda=btScalar(0.4);	//0.4
	radioRueda=btScalar(0.5);	//No menor de 0.4 sino ni se mueve (ruedas pequenyas)
	alturaConexionChasis=btScalar(1.2); //influye mucho en la acceleracion de salida
	Masa =btScalar(1100);
	FuerzaFrenado=btScalar(-10000);
	FuerzaGiro=btScalar(0.3);		//manejo a la hora de girar
	FuerzaFrenoMano=btScalar(800);
	FuerzaFrenadoReposo=btScalar(60);
}



void Corredor::InicializarFisicas(list<btRigidBody*> &objetos, btDiscreteDynamicsWorld *mundo){

	//posicion inicial
		btTransform PosInicial;
		PosInicial.setIdentity();
		PosInicial.setOrigin(btVector3(cuboNodo->getPosition().X,cuboNodo->getPosition().Y,cuboNodo->getPosition().Z));

		//Motionstate
		motionStateCoche = new btDefaultMotionState(PosInicial);//motionState = interpolacion
		

		//establecemos su centro de gravedad
		btTransform localTransform;
		localTransform.setIdentity();
		localTransform.setOrigin(btVector3(0,1,1));
		CentroGravedad = new btCompoundShape();


		//Forma Colision
		btVector3 TamanyoFormaColision(cuboNodo->getScale().X,cuboNodo->getScale().Y,cuboNodo->getScale().Z);
		FormaColision = new btBoxShape(TamanyoFormaColision);
		//masa coche
		
		btVector3 Inercia(0,0,0);
		FormaColision->calculateLocalInertia(Masa, Inercia);

		CentroGravedad->addChildShape(localTransform,FormaColision);

		
		//rigidbody del coche
		CuerpoColisionChasis = new btRigidBody(Masa, motionStateCoche, CentroGravedad, Inercia);

		//Asignamos el nodo irrlich al rigidbody
		CuerpoColisionChasis->setUserPointer((void *)(cuboNodo));
		//Rigidbody en el mundo
		mundo->addRigidBody(CuerpoColisionChasis);

		//RaycastDel Coche
		btVehicleRaycaster* RayCastVehiculo = new btDefaultVehicleRaycaster(mundo);
		btRaycastVehicle::btVehicleTuning tuning;

		//se crea el coche y se le pasa tunin y raycast
		vehiculo = new btRaycastVehicle(tuning,CuerpoColisionChasis,RayCastVehiculo);
		//se establece el sistemas de coordenadas
		vehiculo->setCoordinateSystem(0, 1, 2); // 0, 1, 2
		//actualizamos este cuerpo siempre para la fisicas
		CuerpoColisionChasis->setActivationState(DISABLE_DEACTIVATION);
		//se pone el coche en el mundo
		mundo->addVehicle(vehiculo);

		//metemos el rigidbody en el array para actualizar el nodo		
		objetos.push_back(CuerpoColisionChasis);

		CrearRuedas(vehiculo,tuning);
	
}


void Corredor::BorrarFisicas(){



}


void Corredor::CrearRuedas(btRaycastVehicle* vehiculo,btRaycastVehicle::btVehicleTuning tuning){




btVector3 puntoConexionChasis(cuboNodo->getScale().X-radioRueda,alturaConexionChasis,cuboNodo->getScale().Z-anchoRueda);



// Agrega las ruedas delanteras
	vehiculo-> addWheel ((puntoConexionChasis * btVector3 (4 , 1 , 2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	vehiculo-> addWheel ((puntoConexionChasis * btVector3 (-4 , 1 , 2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true );

	// Agrega las ruedas traseras
	vehiculo-> addWheel ((puntoConexionChasis * btVector3 ( 4 , 1 , -2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false );

	vehiculo-> addWheel ((puntoConexionChasis * btVector3 (-4 , 1 , -2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false );



for (int i = 0; i < vehiculo->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = 60;	//tambaleo de las ruedas (se mueve como si fuera por terreno con baches). A mayor valor mayor tambaleo
		wheel.m_wheelsDampingCompression = 20; //Derrape a mayor giro //btScalar(0.3)*2*btSqrt(wheel.m_suspensionStiffness);  //btScalar(0.8) //valor anterior=2.3f;
		wheel.m_wheelsDampingRelaxation = 20; //btScalar(0.5)*2*btSqrt(wheel.m_suspensionStiffness);  //1 //valor anterior=4.4f;
		wheel.m_frictionSlip = btScalar(1.2); //100;	//conviene que el valor no sea muy bajo. En ese caso desliza y cuesta de mover
		wheel.m_rollInfluence = 0.1; //0.1f;	//Empieza a rodar muy loco, si el valor es alto
		//wheel.m_maxSuspensionForce = 40000.f;	//A mayor valor, mayor estabilidad, (agarre de las ruedas al suelo), pero el manejo empeora (derrapa)
		wheel.m_maxSuspensionTravelCm = 8000.f;	//Nose muy bien que funcion tiene, pero si el valor es muy bajo el coche no avanza
		
/*
	PARAMETROS EN RUEDAS DISPONIBLES
	wheel.m_chassisConnectionCS = connectionPointCS;
	wheel.m_wheelDirectionCS = wheelDirectionCS0;
	wheel.m_wheelAxleCS = wheelAxleCS;
	wheel.m_suspensionRestLength = suspensionRestLength;
	wheel.m_wheelRadius = wheelRadius;
	wheel.m_suspensionStiffness = tuning.m_suspensionStiffness;
	wheel.m_wheelsDampingCompression = tuning.m_suspensionCompression;
	wheel.m_wheelsDampingRelaxation = tuning.m_suspensionDamping;
	wheel.m_frictionSlip = tuning.m_frictionSlip;
	wheel.m_bIsFrontWheel = isFrontWheel;
	wheel.m_maxSuspensionTravelCm = tuning.m_maxSuspensionTravelCm;
	wheel.m_maxSuspensionForce = tuning.m_maxSuspensionForce;*/
	}

}


btRaycastVehicle* Corredor::getVehiculo(){

	return vehiculo;

}

btRigidBody * Corredor::getRigidBody(){

	return CuerpoColisionChasis;
}


IMeshSceneNode *Corredor::getNodo()
{

	return cuboNodo;
}


void Corredor::Pedazodemierda(){

	orientacion = vector3df(sin( ( cuboNodo->getRotation().Y ) * PI/180.0f ), 0, cos( ( cuboNodo->getRotation().Y  ) * PI/180.0f ) );


	//cout<< "Rotacion en Y=="<< cuboNodo->getRotation().Y << endl;  
	//cout<< "ORIENTACION X=="<< orientacion.X << "ORIENTACION Z==" << orientacion.Z << endl;
	orientacion.normalize();
	//cout<< "ORIENTACION XNORMAL=="<< orientacion.X << "ORIENTACION ZNORMAL==" << orientacion.Z << endl;


}

void Corredor::actualizarRuedas(){

		
			btTransform ruedas= vehiculo->getWheelTransformWS(0);

			rueda1->setPosition(vector3df(ruedas.getOrigin().getX(),ruedas.getOrigin().getY(),ruedas.getOrigin().getZ()));
			rueda1->setRotation(vector3df(ruedas.getRotation().getX(),ruedas.getRotation().getY(),ruedas.getRotation().getZ()));
			ruedas= vehiculo->getWheelTransformWS(1);
			
			
			rueda2->setPosition(vector3df(ruedas.getOrigin().getX(),ruedas.getOrigin().getY(),ruedas.getOrigin().getZ()));
			rueda2->setRotation(vector3df(ruedas.getRotation().getX(),ruedas.getRotation().getY(),ruedas.getRotation().getZ()));
			ruedas= vehiculo->getWheelTransformWS(2);

			rueda3->setPosition(vector3df(ruedas.getOrigin().getX(),ruedas.getOrigin().getY(),ruedas.getOrigin().getZ()));
			rueda3->setRotation(vector3df(ruedas.getRotation().getX(),ruedas.getRotation().getY(),ruedas.getRotation().getZ()));
			ruedas= vehiculo->getWheelTransformWS(3);

			rueda4->setPosition(vector3df(ruedas.getOrigin().getX(),ruedas.getOrigin().getY(),ruedas.getOrigin().getZ()));
			rueda4->setRotation(vector3df(ruedas.getRotation().getX(),ruedas.getRotation().getY(),ruedas.getRotation().getZ()));
		
				
}



void Corredor::acelerar(){

			vehiculo-> applyEngineForce ( Fuerza , 2 );
			vehiculo-> applyEngineForce ( Fuerza , 3 );
			vehiculo-> setSteeringValue ( btScalar (0), 0 );
			vehiculo-> setSteeringValue ( btScalar (0), 1 );
}


void Corredor::frenar(){

			vehiculo-> applyEngineForce ( FuerzaFrenado , 2 );
			vehiculo-> applyEngineForce ( FuerzaFrenado , 3 );
			vehiculo-> setSteeringValue ( btScalar (0), 0 );
			vehiculo-> setSteeringValue ( btScalar (0), 1 );
}


void Corredor::girarDerecha(){

			vehiculo-> setSteeringValue ( FuerzaGiro, 0 );
			vehiculo-> setSteeringValue ( FuerzaGiro, 1 );


}


void Corredor::girarIzquierda(){

			vehiculo-> setSteeringValue ( -FuerzaGiro, 0 );
			vehiculo-> setSteeringValue ( -FuerzaGiro, 1 );

}


void Corredor::desacelerar(){

			vehiculo-> applyEngineForce ( 0 , 2 );
			vehiculo-> applyEngineForce ( 0 , 3 );
			
			vehiculo-> setSteeringValue ( 0, 0 );
			vehiculo-> setSteeringValue ( 0, 1 );	

			vehiculo->setBrake(FuerzaFrenadoReposo, 2);
			vehiculo->setBrake(FuerzaFrenadoReposo, 3);

}


void Corredor::frenodemano(){

				vehiculo->setBrake(FuerzaFrenoMano, 2);
				vehiculo->setBrake(FuerzaFrenoMano, 3);

}

/*
	Crea un item proyectil, lo añade al mundo,  a la lista de objetos y le da velocidad linear
*/
void Corredor::lanzarItem(Proyectil *&item,btDiscreteDynamicsWorld *mundo,core::list<btRigidBody *> &objetos){
	
	btRigidBody *rigidItem = item->inicializarFisicas();
	mundo->addRigidBody(rigidItem);
	objetos.push_back(rigidItem);
	
	//item->lanzarItem(this);
	float rotDisparo = cuboNodo->getRotation().Y * PI / 180;
    rigidItem->setLinearVelocity(btVector3(sin(rotDisparo) * 100, 5.0f, cos(rotDisparo) * 100));
    std::cout << "Disparo " << std::endl;
    decCargador();
}

/*

std::string Corredor::getDireccion() {
	if (norte) {
		if (este) {
			return "noreste";
		}
		else {
			if (oeste) {
				return "noroeste";
			}
			else {
				return "norte";
			}
		}
	}


	if (sur) {
		if (este) {
			return "sureste";
		}
		else {
			if (oeste) {
				return "suroeste";
			}
			else {
				return "sur";
			}
		}
	}


	if (este) {
		if (norte) {
			return "noreste";
		}
		else {
			if (sur) {
				return "sureste";
			}
			else {
				return "este";
			}
		}
	}


	if (oeste) {
		if (norte) {
			return "noroeste";
		}
		else {
			if (sur) {
				return "suroeste";
			}
			else {
				return "oeste";
			}
		}
	}

	return "No Direccion";

}

int Corredor::getDireccionGrados() {
	return direccionGrados;
}

std::string Corredor::toString() {
	std::string text = "";
	//Mostrar la Posicion y Velocidad actuales.
	text += "\n Velocidad: ";
	text += to_string(getVelocidad());
	text += "\n Posicion [";
	text += to_string(getPosicion().X);
	text += ", ";
	text += to_string(getPosicion().Y);
	text += ", ";
	text += to_string(getPosicion().Z);
	text += "]\n";
	text += " Direccion: ";
	text += getDireccion();
	text += " [ ";
	text += to_string(getDireccionGrados());
	text += " ]\n";

	return text;
}





	Identifica la rotacion del coche en grados

void Corredor::updateDireccionGrados() {
	int grados = rotCocheY; //cambiar rotacion
	if (grados < 0) {
		grados = 180 + (180 + grados);
	}

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

void Corredor::updateDireccion() {
	updateDireccionGrados();
	// NORTE
	if (direccionGrados <= 20 || direccionGrados >= 341) {
		norte = true;
		sur = false;
		este = false;
		oeste = false;
	}
	else {
		// NORESTE
		if (direccionGrados >= 21 && direccionGrados <= 70) {
			norte = true;
			sur = false;
			este = true;
			oeste = false;
		}
		else {
			// ESTE
			if (direccionGrados >= 71 && direccionGrados <= 110) {
				norte = false;
				sur = false;
				oeste = false;
				este = true;
			}
			else {
				// SURESTE
				if (direccionGrados >= 111 && direccionGrados <= 160) {
					norte = false;
					sur = true;
					este = true;
					oeste = false;
				}
				else {
					// SUR
					if (direccionGrados >= 161 && direccionGrados <= 200) {
						norte = false;
						sur = true;
						este = false;
						oeste = false;
					}
					else {
						// SUROESTE
						if (direccionGrados >= 201 && direccionGrados <= 250) {
							norte = false;
							sur = true;
							este = false;
							oeste = true;
						}
						else {
							// OESTE
							if (direccionGrados >= 251 && direccionGrados <= 290) {
								norte = false;
								sur = false;
								este = false;
								oeste = true;
							}
							else {
								// NOROESTE
								if (direccionGrados >= 291 && direccionGrados <= 340) {
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





void Corredor::setAxis()
{
	Motor3d *m = Motor3d::getInstancia();
	AxesSceneNode *axis = new AxesSceneNode(cuboNodo, m->getScene(), -1);
	axis->setAxesScale(20); //  for the length of the axes
	axis->drop();
}

//	----------------------------------------------
//		METODOS GET
//	----------------------------------------------

*/