
#include "Pista.hpp"

//------------------------------\*
//---CONSTRUCTOR pista----------\*
//------------------------------\*

Pista *Pista::instancia = NULL;


Pista::Pista(){

    m = Motor3d::getInstancia();
    smgr = m->getScene();
	//countMesh = mapa->getMeshBufferCount();


	// -----------------------------
	//  GEOMETRIA MAPA
	// -----------------------------
	/*//!!!Declaracion de escala y llamada al metodo getBulletTriangleMesh, obteniendo todos los triangulos descompuestos del mapa para realizar colisiones
	btTriangleMesh *btm = getBulletTriangleMesh(mapa, Scala);
	//En vez de btBoxShape usamos btBvhTriangleMeshShape (solo para el terreno)
	FormaColision = new btBvhTriangleMeshShape(btm, true);

	// Cargar modelo mapa
	MapaNodo = smgr->addOctreeSceneNode(mapa);
	ITriangleSelector *selector = 0;
	//smgr->getMeshManipulator()->setVertexColors(mapaNodo->getMesh(), SColor(255, 232, 128, 0));
	//selector=smgr->createTriangleSelector(mapa,0);
	//mapaNodo->setTriangleSelector(selector);
	//countTriangle = selector->getTriangleCount();


	MapaNodo->setScale(Scala);
	MapaNodo->setMaterialFlag(EMF_LIGHTING, false); // iluminacion
	MapaNodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);

	
	//}
	*/
}


Pista::~Pista(){


}


Pista *Pista::getInstancia()
{
	if (instancia == NULL)
		instancia = new Pista();

	return instancia;
}

void Pista::InicializarFisicas(){
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

void Pista::setMapa(stringw mapa, const char* fisicas, const char* waypoints){
	Mapa = smgr->addMeshSceneNode(smgr->getMesh(mapa));
	Mapa->setName("MAPA1");
	if(Mapa) {
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
	Waypoint **arrayWaypoints;

	  ifstream myfile (waypoints);
	  if (myfile.is_open())
	  {
		 getline(myfile, line); 

				//crear el array de waypoints para almacenar el path
				tamanyoArrayWaypoints = stoi(line);
				arrayWaypoints = new Waypoint *[tamanyoArrayWaypoints];	
				//se crea un array con las posiciones de los waypoints que se recogeran del fichero

				  for (int j = 0 ;j<tamanyoArrayWaypoints; j++){
				  		cout<<"cuanto tengo: "<<tamanyoArrayWaypoints<<"\n";
						//seteamos los Waypoins
						arrayWaypoints[j] = new Waypoint();
						arrayWaypoints[j]->setNombre(std::to_string(j));
						if(j ==0){//si es el primero apuntara al ultimo
							arrayWaypoints[j]->setSiguiente(arrayWaypoints[tamanyoArrayWaypoints-1]);
						}
						else if(j==tamanyoArrayWaypoints-2){//si es el ultimo apuntara al primero
								arrayWaypoints[j]->setSiguiente(arrayWaypoints[0]);
						} else arrayWaypoints[j]->setSiguiente(arrayWaypoints[j+1]);
						getline(myfile, wX, ' ');
						getline(myfile, wY, ' ');
						getline(myfile, wZ);
						//cambiar a float y almacenar array de waypoints
						arrayWaypoints[j]->setPosicion(std::stof(wX),std::stof(wY),std::stof(wZ));
						//incrementar la j para los waypoints

						cout << "x: " << std::stof(wX) <<"y: "<<std::stof(wY)<<"z: "<< std::stof(wZ) << '\n';
			}

		
	    myfile.close();
	  }

	  else cout << "Error abriendo archivo";	

	}

void Pista::BorrarFisicas(){

// a implementar

}

irr::core::list<Item *> Pista::getItems(){
	return Items;
}

void Pista::setItems(irr::core::list<Item *> itemMetodo){
		 Items = itemMetodo;
}

/*btTriangleMesh* Pista::getBulletTriangleMesh(IMesh *const mesh, vector3df escala)
{
	btVector3 vertices[3];
	u32 i, j, k;
	s32 index, numVertices;
	u16 *mb_indices;

	btTriangleMesh *pTriMesh = new btTriangleMesh();

	for (i = 0; i < mesh->getMeshBufferCount(); i++)
	{
		irr::scene::IMeshBuffer *mb = mesh->getMeshBuffer(i);

		//////////////////////////////////////////////////////////////////////////
		// Extract vertex data                                                  //
		// Because the vertices are stored as structs with no common base class,//
		// We need to handle each type separately                               //
		//////////////////////////////////////////////////////////////////////////
		if (mb->getVertexType() == irr::video::EVT_STANDARD)
		{
			irr::video::S3DVertex *mb_vertices = (irr::video::S3DVertex *)mb->getVertices();
			mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount();
			for (j = 0; j < mb->getIndexCount(); j += 3)
			{ //get index into vertex list
				for (k = 0; k < 3; k++)
				{
					//three verts per triangle
					index = mb_indices[j + k];
					if (index > numVertices)
						continue;
					//convert to btVector3
					vertices[k] = btVector3(mb_vertices[index].Pos.X * escala.X, mb_vertices[index].Pos.Y * escala.Y, mb_vertices[index].Pos.Z * escala.Z); // 1100
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}
		else if (mb->getVertexType() == irr::video::EVT_2TCOORDS)
		{
			// Same but for S3DVertex2TCoords data
			irr::video::S3DVertex2TCoords *mb_vertices = (irr::video::S3DVertex2TCoords *)mb->getVertices();
			u16 *mb_indices = mb->getIndices();
			s32 numVertices = mb->getVertexCount();
			for (j = 0; j < mb->getIndexCount(); j += 3)
			{ //index into irrlicht data
				for (k = 0; k < 3; k++)
				{
					s32 index = mb_indices[j + k];
					if (index > numVertices)
						continue;
					vertices[k] = btVector3(mb_vertices[index].Pos.X * escala.X, mb_vertices[index].Pos.Y * escala.Y, mb_vertices[index].Pos.Z * escala.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}

		// Does not handle the EVT_TANGENTS type
	}

	if (pTriMesh)
	{
		return pTriMesh;
	}

	return NULL;
}
*/