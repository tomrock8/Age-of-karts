
#include "Pista.hpp"

Pista::Pista(vector3df Posicion, vector3df Scala)
{

	Motor3d *m = Motor3d::getInstancia();
	ISceneManager *smgr = m->getScene();
	//IMesh *mapa = smgr->getMesh("assets/mapa01.obj");
	IMesh *mapa = smgr->getMesh("assets/mapa.obj");

	//countMesh = mapa->getMeshBufferCount();

	// -----------------------------
	//  GEOMETRIA MAPA
	// -----------------------------
	//!!!Declaracion de escala y llamada al metodo getBulletTriangleMesh, obteniendo todos los triangulos descompuestos del mapa para realizar colisiones
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
}

Pista::~Pista()
{
}

void Pista::InicializarFisicas(list<btRigidBody *> &objetos, btDiscreteDynamicsWorld *mundo)
{

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
	CuerpoColisionMapa->setUserPointer((void *)(MapaNodo));

	//add al mundo
	//mundo->addRigidBody(cuerpoMapa);
	//objetos.push_back(cuerpoMapa);

	MapaNodo->setName("MAPA1");

	mundo->addRigidBody(CuerpoColisionMapa);
	objetos.push_back(CuerpoColisionMapa);
}

void Pista::BorrarFisicas()
{

	// a implementar
}

btTriangleMesh *Pista::getBulletTriangleMesh(IMesh *const mesh, vector3df escala)
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