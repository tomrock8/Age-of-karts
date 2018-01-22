
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

/*void Pista::InicializarFisicas()
{
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	irr::core::list<btRigidBody *> objetos = bullet->getObjetos();

	Masa = 0;
	//posicion inicial del objeto
	btVector3 posicionMapa(0, 0, 0);
	btTransform mapaTransformacion;
	mapaTransformacion.setIdentity();
	mapaTransformacion.setOrigin(posicionMapa);

	//motionState por defecto
	MotionState = new btDefaultMotionState(mapaTransformacion);

	//crear la forma del mapa, el escalado ya se ha aplicado previamente en la funcion de triangleshape
	//btVector3 mapaExtension(escala.X, escala.Y, escala.Z);

	// Add mass
	btVector3 localInertia;
	FormaColision->calculateLocalInertia(Masa, localInertia);

	//creacion del objeto
	CuerpoColisionMapa = new btRigidBody(Masa, MotionState, FormaColision, localInertia);
	//almacenar en puntero al nodo irrlich para poder actualizar( en caso de ser  necesario)
	CuerpoColisionMapa->setUserPointer((void *)(Mapa));

	//add al mundo
	//mundo->addRigidBody(cuerpoMapa);
	//objetos.push_back(cuerpoMapa);

	mundo->addRigidBody(CuerpoColisionMapa);
	objetos.push_back(CuerpoColisionMapa);
	bullet->setObjetos(objetos);
}*/

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
	std::string tipo, pX, pY, pZ, orientacionWp;
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
				//seteamos los Waypoins
				getline(myfile, orientacionWp, ' ');//orientacion con respecto a la carretera	
				orientacion = stoi(orientacionWp);
				//cout <<"orientacion: "<<orientacion<<endl;
				arrayWaypoints[tamWaypoints] = new Waypoint();

				//comento lo de la id de los waypoints porq da conflico con las cajas
				//arrayWaypoints[tamWaypoints]->getWaypoint()->setID(tamWaypoints);
				
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


