
#include "Pista.hpp"


//------------------------------\*
//---CONSTRUCTOR pista----------\*
//------------------------------\*

Pista *Pista::instancia = NULL;

Pista::Pista() {
}

Pista::~Pista()
{
	//destroy waypoint
	for (int i = 0; i < tamWaypoints; i++) {
		delete arrayWaypoints[i];
	}
	delete arrayWaypoints;
	//destroy cajas
	for (int i = 0; i < tamCajas; i++) {
		delete arrayCajas[i];
	}
	delete arrayCajas;
	//destroy turbo
	for (int i = 0; i < tamTurbos; i++) {
		delete arrayTurbos[i];
	}
	delete arrayTurbos;

	delete instancia;
}

Pista *Pista::getInstancia()
{
	if (instancia == NULL)
		instancia = new Pista();

	return instancia;
}


void Pista::setMapa(stringw mapa, const char *fisicas, const char *waypoints)
{
	Motor3d *m = Motor3d::getInstancia();
	GestorIDs *ids = GestorIDs::getInstancia();
	Mapa = m->getScene()->addMeshSceneNode(m->getScene()->getMesh(mapa));
	if (Mapa)
	{
		Mapa->setName("MAPA1");
		Mapa->setMaterialFlag(EMF_LIGHTING, false);
	}
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	//---------------------------FISICAS-------------------------------//
	//cout << "cuantos rigidBodies tengo: " << num << endl;
	//CuerpoColisionMapa = (btRigidBody*)bullet->getFisicas()->getRigidBodyByIndex(0);

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

	if (myfile.is_open())
	{


		//crear el array de waypoints para almacenar el path
		getline(myfile, tamanyoArrayWaypoints, ' ');
		getline(myfile, tamanyoArrayCajas, ' ');
		getline(myfile, tamanyoArrayTurbo, ' ');
		

		arrayWaypoints = new Waypoint *[stoi(tamanyoArrayWaypoints)];
		arrayTurbos = new Turbo *[stoi(tamanyoArrayTurbo)];
		arrayCajas = new Caja *[stoi(tamanyoArrayCajas)];

		//cout<<"waypoints:"<<wp<<"turbo: "<<turbos<<"caja: "<<cajas<<endl;



		while (!myfile.eof()) {

			getline(myfile, tipo, ' ');//caja turbo o waypoint
			getline(myfile, pX, ' ');//posiciones
			getline(myfile, pY, ' ');//posiciones
			getline(myfile, pZ, ' ');//posiciones


			tipoObj = stoi(tipo);
			if (tipoObj == 0) {//WAYPOINT
				arrayWaypoints[tamWaypoints] = new Waypoint();
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
				arrayWaypoints[tamWaypoints]->setVector1(btVector3(stof(v1X), stof(v1Y), stof(v1Z)));
				//vector2
				getline(myfile, v2X, ' ');
				getline(myfile, v2Y, ' ');
				getline(myfile, v2Z, ' ');
				arrayWaypoints[tamWaypoints]->setVector2(btVector3(stof(v2X), stof(v2Y), stof(v2Z)));
				//=========================================//
				//			rellenar los waypoints		   //
				//=========================================//
				//comento lo de la id de los waypoints porq da conflico con las cajas
				arrayWaypoints[tamWaypoints]->getWaypoint()->setID(tamWaypoints);
				
				if (tamWaypoints > 0 && tamWaypoints <= (stoi(tamanyoArrayWaypoints) - 2))
				{
					arrayWaypoints[tamWaypoints - 1]->setSiguiente(arrayWaypoints[tamWaypoints]);

				}
				else if (tamWaypoints == (stoi(tamanyoArrayWaypoints) - 1)) {
					arrayWaypoints[tamWaypoints - 1]->setSiguiente(arrayWaypoints[tamWaypoints]);

					arrayWaypoints[tamWaypoints]->setSiguiente(arrayWaypoints[0]);

				}
				//cambiar a float y almacenar array de waypoints
				arrayWaypoints[tamWaypoints]->setPosicion(stof(pX), stof(pY), stof(pZ));
				arrayWaypoints[tamWaypoints]->setOrientacion(orientacion);//orientacion del waypoint
				arrayWaypoints[tamWaypoints]->inicializarFisicas();
				IMeshSceneNode *im= arrayWaypoints[tamWaypoints]->getWaypoint();
				ids->setIdentifier(im,im->getName());

				tamWaypoints++;
			}
			if (tipoObj == 1) {//CAJA
				arrayCajas[tamCajas] = new Caja(vector3df(stof(pX), stof(pY), stof(pZ)));
				tamCajas++;

			}
			if (tipoObj == 2) {//TURBO
				arrayTurbos[tamTurbos] = new Turbo(btVector3(stof(pX), stof(pY), stof(pZ)), false);
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


void Pista::BorrarFisicas()
{

	// a implementar
}

irr::core::list<Item *> Pista::getItems()
{
	return Items;
}

void Pista::setItems(irr::core::list<Item *> itemMetodo)
{
	Items = itemMetodo;
}
Waypoint **Pista::getArrayWaypoints() {
	return arrayWaypoints;
}
Caja **Pista::getArrayCaja() {
	return arrayCajas;
}
int Pista::getTamCajas() {
	return tamCajas;
}

void Pista::setArrayCaja(Caja **cajas) {
	arrayCajas = cajas;
}

Turbo **Pista::getArrayTurbo() {
	return arrayTurbos;
}

Turbo *Pista::getTurbo() {
	for (int i = 0; i < tamTurbos; i++) {
		if (arrayTurbos[i]->getTurboActivo() == true) {
			return arrayTurbos[i];
		}
	}

	return NULL;
}

Turbo *Pista::getTurbo(int id) {
	for (int i = 0; i < tamTurbos; i++) {
		if (arrayTurbos[i]->getId() == id) {
			return arrayTurbos[i];
		}
	}

	return NULL;
}


