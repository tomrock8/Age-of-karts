#include "IrrlichtLib.hpp"
#include "Corredor.hpp"
#include "AxesSceneNode.cpp"
#include <string>


//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*
Corredor::Corredor(stringw rutaObj,vector3df pos)
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
	cuboNodo->setScale(vector3df(1,1,1.5));
	// Desactivar la iluminacion del cubo
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(pos);
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

}



void Corredor::InicializarFisicas(){
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();

	//posicion inicial
		btTransform transCoche;
		transCoche.setIdentity();
		btVector3 posTransCoche =  btVector3(cuboNodo->getPosition().X,cuboNodo->getPosition().Y,cuboNodo->getPosition().Z);
		transCoche.setOrigin(posTransCoche);

		//Motionstate
		motionStateCoche = new btDefaultMotionState(transCoche);//motionState = interpolacion
		

		//establecemos su centro de gravedad
		btTransform localTransform;
		localTransform.setIdentity();
		localTransform.setOrigin(btVector3(0,1,0));
		CentroGravedad = new btCompoundShape();



		//Forma Colision
		btVector3 TamanyoFormaColision(cuboNodo->getScale().X,cuboNodo->getScale().Y,cuboNodo->getScale().Z);
		//btVector3 TamanyoFormaColision(1,btScalar(0.5),2);
		FormaColision = new btBoxShape(TamanyoFormaColision);
		//masa coche
		Masa = 2000;
		btVector3 Inercia(0,0,0);
		FormaColision->calculateLocalInertia(Masa, Inercia);
		

		CentroGravedad->addChildShape(localTransform,FormaColision);

		
		//rigidbody del coche
		CuerpoColisionChasis = new btRigidBody(Masa, motionStateCoche, CentroGravedad, Inercia);

		CuerpoColisionChasis->setUserPointer((void *)(cuboNodo));
		
		
		
		//RaycastDel Coche
		btVehicleRaycaster* RayCastVehiculo = new btDefaultVehicleRaycaster(mundo);
		btRaycastVehicle::btVehicleTuning tuning;

		vehiculo = new btRaycastVehicle(tuning,CuerpoColisionChasis,RayCastVehiculo);
		
		CuerpoColisionChasis->setActivationState(DISABLE_DEACTIVATION);
		mundo->addVehicle(vehiculo);
		//vehiculo->setActivationState(DISABLE_DEACTIVATION);
		//almacenar el puntero al nodo irrlicht  para poder actualizar sus valores
		
		objetos.push_back(CuerpoColisionChasis);
		
		
	
		CrearRuedas(vehiculo,tuning);
		mundo->addRigidBody(CuerpoColisionChasis);
		bullet->setObjetos(objetos);
		//rigidBody->applyGravity();


	// luego declaramos sus ruedas
	
	// inicializamos la posicion de las ruedas
	


}


void Corredor::BorrarFisicas(){



}


void Corredor::CrearRuedas(btRaycastVehicle* vehiculo,btRaycastVehicle::btVehicleTuning tuning){

btVector3 direccionRuedas(0,-1,0);

btVector3 rotacionRuedas(-1,0,0);

btScalar suspension(0.9);

btScalar anchoRueda(0.4);

btScalar radioRueda(0.5);

btScalar alturaConexionChasis(1.2);

//btVector3 TamanyoFormaColision(1,btScalar(0.5),2);
btVector3 puntoConexionChasis(cuboNodo->getScale().X-radioRueda,alturaConexionChasis,cuboNodo->getScale().Z-anchoRueda);

vehiculo->setCoordinateSystem(0, 1, 2); // 0, 1, 2

// Agrega las ruedas delanteras
	vehiculo-> addWheel ((puntoConexionChasis * btVector3 (4 , 1 , 2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true);

	vehiculo-> addWheel ((puntoConexionChasis * btVector3 (-4 , 1 , 2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, true );

	// Agrega las ruedas traseras
	vehiculo-> addWheel ((puntoConexionChasis * btVector3 ( 4 , 1 , -2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false );

	vehiculo-> addWheel ((puntoConexionChasis * btVector3 (-4 , 1 , -2 )), direccionRuedas, rotacionRuedas, suspension, radioRueda, tuning, false );



for (int i = 0; i < vehiculo->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehiculo->getWheelInfo(i);
		wheel.m_suspensionStiffness = 20;
		wheel.m_wheelsDampingCompression =2.3f;
		wheel.m_wheelsDampingRelaxation = 4.4f;
		wheel.m_frictionSlip = 100;
		wheel.m_rollInfluence = 0.1f;
	
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


	int Corredor::getCargador(){return cargador;};
	void  Corredor::incCargador(){cargador++;};
	void  Corredor::decCargador(){cargador--;};



void Corredor::lanzarItem(Proyectil *item){
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	
	btRigidBody *rigidItem = item->inicializarFisicas();
	
	//item->lanzarItem(this);
	float rotDisparo = cuboNodo->getRotation().Y * PI / 180;
    item->getRigidBody()->setLinearVelocity(btVector3(sin(rotDisparo) * 100, 5.0f, cos(rotDisparo) * 100));
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