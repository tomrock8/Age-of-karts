#include "Pista.hpp"

Pista *Pista::instancia = NULL;

Pista::Pista() {
}

Pista::~Pista() {
	cout << "Destructor de PISTA. Entro.\n";
	//destroy waypoint
	if(arrayWaypoints.size()>0){
	for (int i = 0; i < tamWaypoints; i++) {
		delete arrayWaypoints.at(i);
	}
	arrayWaypoints.clear();
	}
	if(arrayCajas.size()>0){
	//destroy cajas
	for (int i = 0; i < tamCajas; i++) {
		delete arrayCajas.at(i);
	}
	arrayCajas.clear();
	}
	//delete arrayCajas;
	if(arrayTurbos.size()>0){
	//destroy turbo
	for (int i = 0; i < tamTurbos; i++) {
		delete arrayTurbos.at(i);
	}
	arrayTurbos.clear();
	}
	//delete arrayTurbos;

	if(Items.size()>0){
	//destroy items
	for (int i = 0; i < Items.size(); i++) {
		delete Items.at(i);
	}
	Items.clear();
	}
	
	instancia = nullptr;
	cout << "Destructor de PISTA. Salgo.\n";
}



	
Pista *Pista::getInstancia() {
	if (instancia == NULL)
		instancia = new Pista();

	return instancia;
}


void Pista::setMapa() {
	//void Pista::setMapa(const char* mapa, const char *fisicas, const char *waypoints) {
	if (strcmp(nameMap, "pirata") == 0) {
		path = "assets/MapaPirata/mapaPirata.obj";
		wayPoints = "assets/MapaPirata/wp.obj";
		loadPirateMapElements();
	}
	else if (strcmp(nameMap, "gladiador") == 0) {
		//Cargar mapa gladiador
	}
	if (nameMap != NULL) {
		createMap(path);
		loadWaypoints(wayPoints);

	}

}

void Pista::createMap(const char *path) {
	Mapa = TMotor::instancia().newMeshNode("mapa", path, "escena_raiz", true);
	GestorIDs::instancia().setIdentifier(Mapa, "mapa");
}
void Pista::loadWaypoints(const char* waypoints) {
	//lectura de fichero
	std::string line;
	std::string tipo, pX, pY, pZ, orientacionWp, v1X, v1Y, v1Z, v2X, v2Y, v2Z, v3X, v3Y, v3Z, v4X, v4Y, v4Z;
	std::string tamanyoArrayWaypoints;
	std::string tamanyoArrayCajas;
	std::string tamanyoArrayTurbo;
	std::string pole;
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
		getline(myfile, pole, ' ');

		//arrayCajas.resize(stoi(tamanyoArrayCajas));

		while (!myfile.eof()) {

			getline(myfile, tipo, ' ');//caja turbo o waypoint
			tipoObj = stoi(tipo);
			if (tipoObj != 0) {
				getline(myfile, pX, ' ');//posiciones para cajas turbo y posiciones
				getline(myfile, pY, ' ');//posiciones para cajas turbo y posiciones
				getline(myfile, pZ, ' ');//posiciones para cajas turbo y posiciones
			}
			if (tipoObj == 0) {//WAYPOINT
				Waypoint *w = new Waypoint();
				arrayWaypoints.push_back(w);
				//=========================================//
				//orientacion con respecto a la carretera  //
				//=========================================//
				getline(myfile, orientacionWp, ' ');
				orientacion = stof(orientacionWp);
				//cout <<"orientacion: "<<orientacion<<endl;
				//=========================================//
				//Vectores que complementan los waypoint   //
				//=========================================//
				//la distribucion sera vector1, wp, vector 2
				//vector1
				getline(myfile, v1X, ' ');
				getline(myfile, v1Y, ' ');
				getline(myfile, v1Z, ' ');
				arrayWaypoints.at(arrayWaypoints.size() - 1)->setVector1(btVector3(stof(v1X), stof(v1Y), stof(v1Z)));
				//vector2
				getline(myfile, v2X, ' ');
				getline(myfile, v2Y, ' ');
				getline(myfile, v2Z, ' ');
				arrayWaypoints.at(arrayWaypoints.size() - 1)->setVector2(btVector3(stof(v2X), stof(v2Y), stof(v2Z)));
				//vector3
				getline(myfile, v3X, ' ');
				getline(myfile, v3Y, ' ');
				getline(myfile, v3Z, ' ');
				arrayWaypoints.at(arrayWaypoints.size() - 1)->setVector3(btVector3(stof(v3X), stof(v3Y), stof(v3Z)));
				//vector4
				getline(myfile, v4X, ' ');
				getline(myfile, v4Y, ' ');
				getline(myfile, v4Z, ' ');
				arrayWaypoints.at(arrayWaypoints.size() - 1)->setVector4(btVector3(stof(v4X), stof(v4Y), stof(v4Z)));

				//WP central
				getline(myfile, pX, ' ');//posiciones central wp
				getline(myfile, pY, ' ');//posiciones central wp
				getline(myfile, pZ, ' ');//posiciones central wp
			 //=========================================//
			 //			rellenar los waypoints		   //
			 //=========================================//
			 //comento lo de la id de los waypoints porq da conflico con las cajas
				arrayWaypoints.at(arrayWaypoints.size() - 1)->getWaypoint()->setID(arrayWaypoints.size() - 1);

				if (arrayWaypoints.size() - 1 > 0 && arrayWaypoints.size() - 1 <= (stoi(tamanyoArrayWaypoints) - 2))
				{
					arrayWaypoints.at(arrayWaypoints.size() - 1 - 1)->setSiguiente(arrayWaypoints.at(arrayWaypoints.size() - 1));

				}
				else if (arrayWaypoints.size() - 1 == (stoi(tamanyoArrayWaypoints) - 1)) {
					arrayWaypoints.at(arrayWaypoints.size() - 1 - 1)->setSiguiente(arrayWaypoints.at(arrayWaypoints.size() - 1));
					arrayWaypoints.at(arrayWaypoints.size() - 1)->setSiguiente(arrayWaypoints[0]);

				}
				//cambiar a float y almacenar array de waypoints
				arrayWaypoints.at(arrayWaypoints.size() - 1)->setPosicion(stof(pX), stof(pY), stof(pZ));
				arrayWaypoints.at(arrayWaypoints.size() - 1)->setOrientacion(orientacion);//orientacion del waypoint
				arrayWaypoints.at(arrayWaypoints.size() - 1)->inicializarFisicas();
				obj3D *im = arrayWaypoints.at(arrayWaypoints.size() - 1)->getWaypoint();
				GestorIDs::instancia().setIdentifier(im, im->getName());
				arrayWaypoints.at(arrayWaypoints.size() - 1)->setID(GestorIDs::instancia().getIDLibre() - 1);
				tamWaypoints++;
			}
			if (tipoObj == 1) {//CAJA
				Caja *cj = new Caja(btVector3(stof(pX), stof(pY), stof(pZ)));
				arrayCajas.push_back(cj);
				tamCajas++;

			}
			if (tipoObj == 2) {//TURBO
				Turbo *tb = new Turbo(btVector3(stof(pX), stof(pY), stof(pZ)), false);
				arrayTurbos.push_back(tb);
				tamTurbos++;
			}
			if (tipoObj == 3) {//POSICION PARRILLA
				parrilla.push_back(glm::vec3(stof(pX), stof(pY), stof(pZ)));

			}
			//cout << line << endl;
		}

		myfile.close();
	}
	else {
		cout << "Error abriendo archivo";
	}
}


void Pista::loadPirateMapElements() {
	TNodo *aux = NULL;
	std::string pX, pY, pZ, num;
	float posX, posY, posZ;
	TMotor::instancia().newMeshNode("elementos", "assets/MapaPirata/elementos.obj", "escena_raiz", false);
	TMotor::instancia().newMeshNode("elementos", "assets/MapaPirata/Turbos.obj", "escena_raiz", false);
	TMotor::instancia().newMeshNode("elementos", "assets/MapaPirata/parrilla.obj", "escena_raiz", false);
	//lectura de las palmeras y su creacion 
	/*
	ifstream myfile("assets/MapaPirata/palmeras1.txt");
	std::string palm = "palmeras";
	int i = 0;
	std::vector<obj3D*>palmeras;
		while (!myfile.eof()) {

			//crear el array de waypoints para almacenar el path
			getline(myfile, pX, ' ');
			getline(myfile, pY, ' ');
			getline(myfile, pZ, ' ');
			std::string palm2 = palm + to_string(i);

			palmeras.push_back( TMotor::instancia().newMeshNode(palm2.c_str(), "assets/MapaPirata/palmeras1.obj", "escena_raiz", false));
			palmeras.at(i)->setPosition(stof(pX), stof(pY), stof(pZ));
			i++;
		}

	myfile.close();
	aux = NULL;
*/
}

std::vector<Item *> Pista::getItems() { return Items; }
void Pista::setNumVueltas(int n) { vueltas = n; }
void Pista::setNombreMapa(const char *name) { nameMap = name; }
void Pista::setItems(std::vector<Item *> itemMetodo) { Items = itemMetodo; }
std::vector<Waypoint*> Pista::getArrayWaypoints() { return arrayWaypoints; }
std::vector<Caja*> Pista::getArrayCaja() { return arrayCajas; }
std::vector<glm::vec3> Pista::getParrilla() { return parrilla; }
int Pista::getTamCajas() { return tamCajas; }
void Pista::setArrayCaja(std::vector<Caja*> cajas) { arrayCajas = cajas; }
std::vector<Turbo*>Pista::getArrayTurbo() { return arrayTurbos; }
int Pista::getTamArrayWaypoints() { return tamWaypoints; }
int Pista::getNumVueltas() { return vueltas; }

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
		if (arrayTurbos.at(i)->getID() == id) {
			return arrayTurbos.at(i);
		}
	}

	return NULL;
}