
#include "Pista.hpp"


//------------------------------\*
//---CONSTRUCTOR pista----------\*
//------------------------------\*

Pista *Pista::instancia = NULL;

Pista::Pista() {
}

Pista::~Pista() {
	cout << "Destructor de PISTA. Entro.\n";
	//destroy waypoint
	for (int i = 0; i < tamWaypoints; i++) {
		delete arrayWaypoints.at(i);
	}
	arrayWaypoints.clear();
	//delete arrayWaypoints;
	//destroy cajas
	for (int i = 0; i < tamCajas; i++) {
		delete arrayCajas.at(i);
	}
	arrayCajas.clear();
	//delete arrayCajas;
	//destroy turbo
	for (int i = 0; i < tamTurbos; i++) {
		delete arrayTurbos.at(i);
	}
	arrayTurbos.clear();
	//delete arrayTurbos;

	instancia = nullptr;
	cout << "Destructor de PISTA. Salgo.\n";
}

Pista *Pista::getInstancia()
{
	if (instancia == NULL)
		instancia = new Pista();

	return instancia;
}


void Pista::setMapa(stringw mapa, const char *fisicas, const char *waypoints) {
	Mapa = Motor3d::instancia().getScene()->addMeshSceneNode(Motor3d::instancia().getScene()->getMesh(mapa));
	if (Mapa) {
		Mapa->setName("Mapa");
		Mapa->setMaterialFlag(EMF_LIGHTING, false);
	}

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();

	//---------------------------FISICAS-------------------------------//

	fisicasMapa = new btBulletWorldImporter(0);
	fisicasMapa->loadFile(fisicas);
	{
		int num = fisicasMapa->getNumRigidBodies();

		for (int i = 0; i < num; i++) {
			CuerpoColisionMapa = (btRigidBody*)fisicasMapa->getRigidBodyByIndex(i);//recoger el rigidbody
			//almacenar en puntero al nodo irrlich para poder actualizar( en caso de ser  necesario)
			CuerpoColisionMapa->setUserPointer((void *)(Mapa));
			mundo->addRigidBody(CuerpoColisionMapa);//almacenar rigidbody en el mundo
		}
	}


	// ------------------------------------------------------

	//lectura de fichero
	std::string line;
	std::string tipo, pX, pY, pZ, orientacionWp, v1X, v1Y, v1Z, v2X, v2Y, v2Z;
	std::string tamanyoArrayWaypoints;
	std::string tamanyoArrayCajas;
	std::string tamanyoArrayTurbo;
	int tipoObj;
	tamWaypoints = 0;
	tamTurbos = 0;
	tamCajas = 0;
	float orientacion = 0.0f;


	ifstream myfile(waypoints);

	if (myfile.is_open()) {
		//crear el array de waypoints para almacenar el path
		getline(myfile, tamanyoArrayWaypoints, ' ');
		getline(myfile, tamanyoArrayCajas, ' ');
		getline(myfile, tamanyoArrayTurbo, ' ');


		
		
		//arrayCajas.resize(stoi(tamanyoArrayCajas));

		while (!myfile.eof()) {

			getline(myfile, tipo, ' ');//caja turbo o waypoint
			getline(myfile, pX, ' ');//posiciones
			getline(myfile, pY, ' ');//posiciones
			getline(myfile, pZ, ' ');//posiciones


			tipoObj = stoi(tipo);
			Waypoint *w =new Waypoint();
			if (tipoObj == 0) {//WAYPOINT
				arrayWaypoints.push_back(w);
				//=========================================//
				//orientacion con respecto a la carretera  //
				//=========================================//
				getline(myfile, orientacionWp, ' ');
				orientacion = stoi(orientacionWp);
				//cout <<"orientacion: "<<orientacion<<endl;
				//=========================================//
				//Vectores que complementan los waypoint   // 
				//=========================================//
				//la distribucion sera vector1, wp, vector 2
				//vector1
				getline(myfile, v1X, ' ');
				getline(myfile, v1Y, ' ');
				getline(myfile, v1Z, ' ');
				arrayWaypoints.at(arrayWaypoints.size()-1)->setVector1(btVector3(stof(v1X), stof(v1Y), stof(v1Z)));
				//vector2
				getline(myfile, v2X, ' ');
				getline(myfile, v2Y, ' ');
				getline(myfile, v2Z, ' ');
				arrayWaypoints.at(arrayWaypoints.size()-1)->setVector2(btVector3(stof(v2X), stof(v2Y), stof(v2Z)));
				//=========================================//
				//			rellenar los waypoints		   //
				//=========================================//
				//comento lo de la id de los waypoints porq da conflico con las cajas
				arrayWaypoints.at(arrayWaypoints.size()-1)->getWaypoint()->setID(arrayWaypoints.size()-1);

				if (arrayWaypoints.size()-1 > 0 && arrayWaypoints.size()-1 <= (stoi(tamanyoArrayWaypoints) - 2))
				{
					arrayWaypoints.at(arrayWaypoints.size()-1 - 1)->setSiguiente(arrayWaypoints.at(arrayWaypoints.size()-1));

				}
				else if (arrayWaypoints.size()-1 == (stoi(tamanyoArrayWaypoints) - 1)) {
					arrayWaypoints.at(arrayWaypoints.size()-1 - 1)->setSiguiente(arrayWaypoints.at(arrayWaypoints.size()-1));

					arrayWaypoints.at(arrayWaypoints.size()-1)->setSiguiente(arrayWaypoints[0]);

				}
				//cambiar a float y almacenar array de waypoints
				arrayWaypoints.at(arrayWaypoints.size()-1)->setPosicion(stof(pX), stof(pY), stof(pZ));
				arrayWaypoints.at(arrayWaypoints.size()-1)->setOrientacion(orientacion);//orientacion del waypoint
				arrayWaypoints.at(arrayWaypoints.size()-1)->inicializarFisicas();
				IMeshSceneNode *im = arrayWaypoints.at(arrayWaypoints.size()-1)->getWaypoint();
				GestorIDs::instancia().setIdentifier(im, im->getName());

				tamWaypoints++;
			}
			if (tipoObj == 1) {//CAJA
				Caja *cj= new Caja(btVector3(stof(pX), stof(pY), stof(pZ)));
				arrayCajas.push_back(cj);
				tamCajas++;

			}
			if (tipoObj == 2) {//TURBO
				Turbo *tb= new Turbo(btVector3(stof(pX), stof(pY), stof(pZ)), false);
				arrayTurbos.push_back(tb);
				tamTurbos++;
			}
			//cout << line << endl;
		}

		myfile.close();

	}
	else {
		cout << "Error abriendo archivo";
	}
}


void Pista::BorrarFisicas() {
	// a implementar
}

irr::core::list<Item *> Pista::getItems() {
	return Items;
}

void Pista::setItems(irr::core::list<Item *> itemMetodo) {
	Items = itemMetodo;
}
vector<Waypoint*> Pista::getArrayWaypoints() {
	return arrayWaypoints;
}
vector<Caja*> Pista::getArrayCaja() {
	return arrayCajas;
}
int Pista::getTamCajas() {
	return tamCajas;
}

void Pista::setArrayCaja(vector<Caja*> cajas) {
	arrayCajas = cajas;
}

vector<Turbo*>Pista::getArrayTurbo() {
	return arrayTurbos;
}

Turbo *Pista::getTurbo() {
	for (int i = 0; i < tamTurbos; i++) {
		if (arrayTurbos.at(i)->getTurboActivo() == true) {
			return arrayTurbos.at(i);
		}
	}

	return NULL;
}

Turbo *Pista::getTurbo(int id) {
	for (int i = 0; i < tamTurbos; i++) {
		if (arrayTurbos.at(i)->getId() == id) {
			return arrayTurbos.at(i);
		}
	}

	return NULL;
}

int Pista::getTamArrayWaypoints() {
	return tamWaypoints;
}