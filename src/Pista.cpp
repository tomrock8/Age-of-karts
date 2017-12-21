
#include "Pista.hpp"

//------------------------------\*
//---CONSTRUCTOR pista----------\*
//------------------------------\*

Pista *Pista::instancia = NULL;

Pista::Pista()
{

	m = Motor3d::getInstancia();
	smgr = m->getScene();
	
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
	Mapa = smgr->addMeshSceneNode(smgr->getMesh(mapa));
	Mapa->setName("MAPA1");
	if (Mapa)
	{
		Mapa->setMaterialFlag(EMF_LIGHTING, false);
	}
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	//bullet->setFisicas(fisicas);---------------------------------------------------------------Comentado por error, segmentation fault
	//lectura de waypoints y creacion de los menesteres pertinentes
	// ----------------------------//
	// ---------Waypoints----------//
	// ----------------------------//
	//lectura de fichero

	std::string line;

	int j;
	std::string wX, wY, wZ;
	int tamanyoArrayWaypoints = 0;
	

	ifstream myfile(waypoints);
	if (myfile.is_open())
	{
		getline(myfile, line);

		//crear el array de waypoints para almacenar el path
		tamanyoArrayWaypoints = stoi(line);
		arrayWaypoints = new Waypoint *[tamanyoArrayWaypoints];
		//se crea un array con las posiciones de los waypoints que se recogeran del fichero

		for (int j = 0; j < tamanyoArrayWaypoints; j++)
		{
			cout << "cuanto tengo: " << tamanyoArrayWaypoints << "\n";
			//seteamos los Waypoins
			arrayWaypoints[j] = new Waypoint();
			arrayWaypoints[j]->setNombre(std::to_string(j));
			if (j == 0)
			{ //si es el primero apuntara al ultimo
				arrayWaypoints[j]->setSiguiente(arrayWaypoints[tamanyoArrayWaypoints - 1]);
			}
			else if (j == tamanyoArrayWaypoints - 2)
			{ //si es el ultimo apuntara al primero
				arrayWaypoints[j]->setSiguiente(arrayWaypoints[0]);
			}
			else
				arrayWaypoints[j]->setSiguiente(arrayWaypoints[j + 1]);
			getline(myfile, wX, ' ');
			getline(myfile, wY, ' ');
			getline(myfile, wZ);
			//cambiar a float y almacenar array de waypoints
			arrayWaypoints[j]->setPosicion(std::stof(wX), std::stof(wY), std::stof(wZ));
			//incrementar la j para los waypoints

			cout << "x: " << std::stof(wX) << "y: " << std::stof(wY) << "z: " << std::stof(wZ) << '\n';
		}

		myfile.close();
	}

	else
		cout << "Error abriendo archivo";
		t = new Turbo(100, btVector3(25.0f,-4.5f,50.0f),false);

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
Waypoint **Pista::getArrayWaypoints(){
	return arrayWaypoints;
}

Turbo *Pista::getTurbo(){
	return t;
}