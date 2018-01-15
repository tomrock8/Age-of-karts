
#include "Pista.hpp"
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

//------------------------------\*
//---CONSTRUCTOR pista----------\*
//------------------------------\*

Pista *Pista::instancia = NULL;

Pista::Pista() {
}

Pista::~Pista()
{
}

Pista *Pista::getInstancia()
{
	if (instancia == NULL)
		instancia = new Pista();

	return instancia;
}

void Pista::InicializarFisicas()
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
}

void Pista::setMapa(stringw mapa, const char *fisicas, const char *waypoints)
{
	idObjCajas=100;//variable inicializada al principio para poner los ids de los objetos que aparecen random en las cajas
	Motor3d *m = Motor3d::getInstancia();
	Mapa = m->getScene()->addMeshSceneNode(m->getScene()->getMesh(mapa));
	if (Mapa)
	{
		Mapa->setName("MAPA1");
		Mapa->setMaterialFlag(EMF_LIGHTING, false);
	}
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	//bullet->setFisicas(fisicas);---------------------------------------------------------------Comentado por error, segmentation fault
	//lectura de waypoints y creacion de los menesteres pertinentes
	//lectura de fichero
	std::string line;
	int j;
	std::string tipo, pX, pY, pZ;
	std::string tamanyoArrayWaypoints;
	std::string tamanyoArrayCajas;
	std::string tamanyoArrayTurbo;
	int tipoObj;


	tamCajas = tamTurbos = tamWaypoints = 0;
	ifstream myfile(waypoints);

	if (myfile.is_open())
	{


		//crear el array de waypoints para almacenar el path
		getline(myfile, tamanyoArrayWaypoints, ' ');
		getline(myfile, tamanyoArrayTurbo, ' ');
		getline(myfile, tamanyoArrayCajas, ' ');

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
				arrayWaypoints[tamWaypoints] = new Waypoint();
				arrayWaypoints[tamWaypoints]->setNombre(std::to_string(tamWaypoints));
				if (tamWaypoints == 0)
				{ //si es el primero apuntara al ultimo
					arrayWaypoints[tamWaypoints]->setSiguiente(arrayWaypoints[stoi(tamanyoArrayWaypoints) + 1]);
				}
				else if (tamWaypoints == stoi(tamanyoArrayWaypoints) - 2)
				{ //si es el ultimo apuntara al primero
					arrayWaypoints[tamWaypoints]->setSiguiente(arrayWaypoints[0]);
				}
				else arrayWaypoints[tamWaypoints]->setSiguiente(arrayWaypoints[tamWaypoints + 1]);

				//cambiar a float y almacenar array de waypoints
				arrayWaypoints[tamWaypoints]->setPosicion(stof(pX), stof(pY), stof(pZ));
				tamWaypoints++;
			}
			if (tipoObj == 1) {//CAJA
				arrayCajas[tamCajas] = new Caja(vector3df(stof(pX), stof(pY), stof(pZ)), tamCajas);
				tamCajas++;

			}
			if (tipoObj == 2) {//TURBO
				arrayTurbos[tamTurbos] = new Turbo(tamTurbos, btVector3(stof(pX), stof(pY), stof(pZ)), false);
				tamTurbos++;
			}
			cout << line << endl;
		}


		myfile.close();
	}

	else
		cout << "Error abriendo archivo";


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
		if (arrayTurbos[i]->getId() == id) {			return arrayTurbos[i];
		}
	}

	return NULL;
}


//RECOGER Y ASIGNAR IDS A LOS OBJETOS RANDOM DE LAS CAJAS
void Pista::setIdsObjetosCaja(int i){
	idObjCajas = i;
}
int Pista::getIdsObjetosCaja(){
	return idObjCajas;
}