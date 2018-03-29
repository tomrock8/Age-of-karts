#include "TGestorRecursos.hpp"
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

TGestorRecursos::TGestorRecursos() {}
TGestorRecursos::~TGestorRecursos() {}


std::vector<mesh*> TGestorRecursos::loadMesh(const char *path) {
	scene = NULL;
	//primero limpiamos los vectores de mesh
	int  i = objMeshes.size();
	if (objMeshes.size() > 0) {
		objMeshes.erase(objMeshes.begin(), objMeshes.begin() + i);
	}
	Assimp::Importer import;
	scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		////cout << "(TRecursoTextura::loadModel) Error cargando assimp" << import.GetErrorString() << endl;
		return objMeshes;
	}
	objMeshes.reserve(scene->mNumMeshes);	//lectura de assimp
	fichName = new string(std::string(path).substr(std::string(path).find_first_of('/') + 1, std::string(path).size()));
	dir = new string(std::string(path).substr(0, std::string(path).find_last_of('/') + 1));
	////cout << " directorio " << dir->c_str() << endl;
	TextureName = NULL;
	processNode(scene->mRootNode, scene);

	return objMeshes;
}

void TGestorRecursos::processNode(aiNode *node, const aiScene *scene) {
	//procesar todas las mallas que se encuentren en el fichero

	for (GLuint i = 0; i < scene->mNumMeshes; i++) {
		TRecursoMalla *meshAux = NULL;
		TRecursoMaterial *MatAux = NULL;
		TRecursoTextura *TextAux = NULL;
		mesh *meshAcumulated = new mesh();
		aiMesh *mesh = scene->mMeshes[i];		//se recoje la malla de la escena

		meshName = scene->mMeshes[i]->mName.C_Str();
		//--------------------------------------------procesar la malla---------------------------------------------------------
		////cout << "________________________________________________________________________________________" << endl;
		//cout << "-------------------------PROCESANDO LA MALLA : " << meshName << "-----------------------" << endl;
		////cout << "________________________________________________________________________________________" << endl;

		if (meshName != NULL) {

			meshAux = getRecursoMalla(meshName, mesh, scene);


			//procesar el material

			MatAux = processMaterial(mesh, scene);

			
			//procesar las texturas
			////cout << "________________________________________________________________________________________" << endl;
			////cout << "----------------------PROCESANDO LAS TEXTURAS : " << TextureName->c_str() << "--------------------------"<<endl;
			////cout << "________________________________________________________________________________________" << endl;
			
			TextAux = processTextures(mesh, scene);//---------------------mal, tiene que comprobar con los nombres de las texturas

			//--guardar en objMesh todo lo de la malla actual
			meshAcumulated->setMesh(meshAux);
			meshAcumulated->setMat(MatAux);
			meshAcumulated->setTex(TextAux);
			objMeshes.push_back(meshAcumulated);

		}
		//lo mismo para los hijos que pueda tener
		//for (GLuint i = 0; i < node->mNumChildren; i++) {
		//	processNode(node->mChildren[i], scene);
		//}


	}
}

//-----------------------------------------------------------------------------------------------//
//------------------------------------TRECURSOMALLA----------------------------------------------//
//-----------------------------------------------------------------------------------------------//

TRecursoMalla *TGestorRecursos::getRecursoMalla(const char * nombre, aiMesh *mesh, const aiScene *scene) {
	bool encontrado = false;
	TRecursoMalla *recMalla = NULL;

			
	std::string nameExt = nombre;
	for (GLuint i = 0; i < recursoMallas.size(); i++) {
		recMalla = (TRecursoMalla *)recursoMallas.at(i);
		std::string nameRecMalla = recMalla->getNombre();
		////cout << "compruebo Malla : " << nameExt << "   VS   " << nameRecMalla << endl;
		//if (strcmp(fichName->c_str(), recursoMallas.at(i)->getNombre()) == 0) {
		if (nameExt.compare(nameRecMalla) == 0) {
			recMalla = (TRecursoMalla*)recursoMallas.at(i);
			////cout << "He encontrado el recurso Malla: " << recMalla->getNombre() << " y lo asigno a la mallaAux" << endl;
			encontrado = true;
			return recMalla;
		}

	}
	if (!encontrado) {
		////cout << "No se ha encontrado el material , se crea" << endl;
		
	
		recMalla = processMesh(mesh, scene);
		recursoMallas.push_back(recMalla);
	}


	return recMalla;
}
TRecursoMalla *TGestorRecursos::processMesh(aiMesh *mesh, const aiScene *scene) {
	TRecursoMalla *recMalla = new TRecursoMalla(mesh);
	//recMalla->inicializar();
	return recMalla;
}

//-----------------------------------------------------------------------------------------------//
//-------------------------------------TRECURSOMATERIAL------------------------------------------//
//-----------------------------------------------------------------------------------------------//

TRecursoMaterial *TGestorRecursos::getRecursoMaterial(const char * nombre, const aiMaterial *mat) {
	bool encontrado = false;
	TRecursoMaterial *recMaterial = NULL;
	std::string nameExt = nombre;
	for (GLuint i = 0; i < recursoMaterials.size(); i++) {
		std::string nameRecMaterial = recursoMaterials.at(i)->getNombre();
		////cout << "compruebo Material : " << nameExt << "   VS   " << nameRecMaterial << endl;
		if (nameExt.compare(nameRecMaterial) == 0) {
			////cout << " he comprobado material y he visto que hay uno igual" << endl;
			recMaterial = (TRecursoMaterial*)recursoMaterials.at(i);
			encontrado = false;
			return recMaterial;
		}
	}
	if (!encontrado) {
		////cout << " El material no existe y se procede a crearlo " << endl;
		recMaterial = new TRecursoMaterial(mat);
		recursoMaterials.push_back(recMaterial);
	}
	return recMaterial;
}
TRecursoMaterial *TGestorRecursos::processMaterial(aiMesh *mesh, const aiScene *scene) {
	TRecursoMaterial *recMat = NULL;
	int numMaterials = scene->mNumMaterials - 1;
	//cargar todas las texturas	que hay en la malla con nNumMaterials, que contiene el numero de materiales
	//for (GLuint i = 0; i < numMaterials; i++) {
		//cargar los materiales
	if (mesh->mMaterialIndex >= 0) {// solo cuando no haya nada para comprobar
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];//recogemos el material
		if (recursoMaterials.size() == 0) {
			////cout << "________________________________________________________________________________________" << endl;
			////cout << "--------------------------PROCESANDO EL MATERIAL  cuando es 0 el vector de materiales---" << endl;
			////cout << "________________________________________________________________________________________" << endl;
			recursoMaterials.push_back(new TRecursoMaterial(mat));
			return (TRecursoMaterial*)recursoMaterials.at(0);
		}
		else {
			//auxliliar solo para guardar el nombre y comprobarlo
			recMatAux = new TRecursoMaterial(mat);
			////cout << " ________________________________________________________________________________________" << endl;
			////cout << "|-----------------------PROCESANDO EL MATERIAL : "<< (string)recMatAux->getNombre() <<"----------------------|" << endl;
			////cout << " ________________________________________________________________________________________" << endl;
			recMat = getRecursoMaterial(recMatAux->getNombre(), mat);
		}

	}

	//}
	return recMat;
}


//-----------------------------------------------------------------------------------------------------//
//-----------------------------------TGESTORTEXTURAS---------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
TRecursoTextura *TGestorRecursos::getRecursoTextura(const char * nombre, aiMesh *mesh, const aiScene *scene) {
	bool encontrado = false;
	TRecursoTextura *recTextura = NULL;
	//recoger textura solo si existe
	if (nombre != NULL) {

	
	std::string nameExt = nombre;
	nameExt = nameExt.substr(0, nameExt.find_first_of('_'));
	for (GLuint i = 0; i < recursoTexturas.size(); i++) {
		std::string nameRecText = recursoTexturas.at(i)->getNombre();
		nameRecText = nameRecText.substr(0, nameRecText.find_first_of('_'));
		//cout << "compruebo Textura : " << nameExt << "   VS   " << nameRecText << endl;
		if (nameExt.compare(nameRecText) == 0) {
			//cout << " He encontrado una textura igual , la devuelvo" << endl;
			recTextura = (TRecursoTextura*)recursoTexturas.at(i);
			encontrado = true;
			return recTextura;
		}
	}
	}
	return recTextura;
}
TRecursoTextura *TGestorRecursos::processTextures(aiMesh *mesh, const aiScene *scene) {
	TRecursoTextura * recText = NULL;
	int numMaterials = scene->mNumMaterials - 1;
	//	for (GLuint i = 0; i < numMaterials; i++) {
		//la primera vez que entre se creara el recurso de texturas
	const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		if (recursoTexturas.size() == 0) {
			const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			recText = new TRecursoTextura(mat);
			//cargar los id de las texturas
			GLuint diffuseTexture = recText->getDiffuseTextureID();
			GLuint specularTexture = recText->getSpecularTextureID();
			GLuint normalTexture = recText->getNormalTextureID();
			loadTexture(scene, mesh, aiTextureType_DIFFUSE, diffuseTexture, recText);
			//carga de textura specular
			loadTexture(scene, mesh, aiTextureType_SPECULAR, specularTexture, recText);
			//carga de textura normal
			loadTexture(scene, mesh, aiTextureType_HEIGHT, normalTexture, recText);
			recursoTexturas.push_back(new TRecursoTextura(mat));

		
		}
		else {
			////auxliar solo para comprobar el nombre
			recTextAux = new TRecursoTextura(mat);
			const char *mal = recTextAux->getNombre();
			//cout << "El nombre sale mal: " << mal <<endl;

			//comprobar 
		
			recText = getRecursoTextura(recTextAux->getNombre(), mesh, scene);
			//no existe se crea y almacena
			if (recText == NULL) {
				//cout << " el recurso estaba vacio y me pongo a crearlo" << endl;
				recText = new TRecursoTextura(mat);
				//cargar los id de las texturas
				GLuint diffuseTexture = recText->getDiffuseTextureID();
				GLuint specularTexture = recText->getSpecularTextureID();
				GLuint normalTexture = recText->getNormalTextureID();
				loadTexture(scene, mesh, aiTextureType_DIFFUSE, diffuseTexture, recText);
				//carga de textura specular
				loadTexture(scene, mesh, aiTextureType_SPECULAR, specularTexture, recText);
				//carga de textura normal
				loadTexture(scene, mesh, aiTextureType_HEIGHT, normalTexture, recText);
				recursoTexturas.push_back(recText);
			}
		}
			

//	}

	return recText;
}
void TGestorRecursos::loadTexture(const aiScene *scene, const aiMesh *mesh, aiTextureType textureType, GLuint &texture, TRecursoTextura *recText) {
	if (mesh->mMaterialIndex >= 0) { //obtener el material correspondiente
		const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		////////cout << mat->GetTextureCount(textureType) << endl;
		for (GLuint i = 0; i < mat->GetTextureCount(textureType); i++) {
			aiString path;
			if (mat->GetTexture(textureType, i, &path) == AI_SUCCESS) {
				const string texturePath = path.C_Str();
				TextureName = new string(texturePath.substr(0, texturePath.find_first_of('_')));
				if (recText->getNombre() == NULL) {
					//cout << "he entrado aqui porque no existia una textura igual" << endl;
					recText->setNombre(TextureName->c_str());
				}
				
				//cout << "-----------------------------------------------------------" << endl;
				//con la textura recogida comprobar que no esta guardada antes de crearla
				string typeText; //tipo de textura
				//Segun el tipo de textura, se le asigna un nombre para recuperar posteriormente su id a la hora de dibujarla
				if (textureType == aiTextureType_DIFFUSE) {
					typeText = "texture_diffuse";
				}
				else if (textureType == aiTextureType_SPECULAR) {
					typeText = "texture_specular";
				}
				else {
					typeText = "texture_normal";
				}

				////cout << " Estoy cargando la textura : " << dir->c_str() + texturePath << std::endl;
				//asignar la textura
				texture = textureFromFile(dir->c_str() + texturePath);
				//////cout << texture << "*****" << endl;
				recText->setTexture(typeText, texture);
			}
		}
	}
}

//cargar textura y generar identificador
GLuint TGestorRecursos::textureFromFile(const std::string& file) {
	GLuint idTexture;
	glGenTextures(1, &idTexture);

	int width, height, comp;
	////cout << file << std::endl;
	unsigned char *imgTexture = stbi_load(file.c_str(), &width, &height, &comp, 3);

	///*if (imgTexture) {
	//	////cout << "he cargado la textura" << comp << endl;
	//}
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);//liberamos la textura

	stbi_image_free(imgTexture);

	return idTexture;
}