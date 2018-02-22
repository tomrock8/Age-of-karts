#include "TRecursoMalla.hpp"

TRecursoMalla::TRecursoMalla(string path){
	loadModel(path);
}
TRecursoMalla::~TRecursoMalla() {

}
//--------------------------------//
//------------ASSIMP--------------//
//--------------------------------//
void TRecursoMalla::loadModel(string path) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "Error cargando assimp" << import.GetErrorString() << endl;
		return;
	}
	//nombre = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}
void TRecursoMalla::processNode(aiNode *node, const aiScene *scene) {
	//procesar todas las mallas que se encuentren en el fichero
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];		//se recoje la malla de la escena
		processMesh(mesh, scene);								// se guarda en el vector de malla
	}
	//lo mismo para los hijos que pueda tener ( no debe tener hijos los modelos que carguemos)
	/*for (GLuint i = 0; i < node->mNumMeshes; i++) {
		processNode(node->mChildren[i], scene);
	}*/
}
void TRecursoMalla::processMesh(aiMesh *mesh, const aiScene *scene) {
	//vertex es una estructura que contiene  posicion, normal, y coordenadas de textura

	//recorrer cada uno de los vertices de la malla
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		vertex v;
		glm::vec3 vector;//asimp usa su propia clase vectorial , creamos un vector para pasarle los datos de assimp
		//posiciones
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		v.position = vector;
		//normales
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		v.normal = vector;
		//texturas , va aqui o en otra clase?
		//indices 
		//se recorre cada cara de la malla y devuelve los vectores indices
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// devuelve los indices y los guarda en el vector de indices
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		
	}

}
//--------------------------------//
//------HERENCIA DE TRECURSO------//
//--------------------------------//
const char *TRecursoMalla::getNombre() {
	return nombre; 
}
void TRecursoMalla::setNombre(const char *s) {
	nombre = s; 
}

void TRecursoMalla::draw(Shader shader) {//se le pasa un shader
	/*for (GLuint i = 0; i < meshes.size; i++) {
		meshes[i].draw(shader);// dibujado de la malla
	}*/
}

vector<vertex> *TRecursoMalla::getVertices() {
	return &vertices;
}
vector<GLuint> *TRecursoMalla::getIndices() {
	return &indices;
}