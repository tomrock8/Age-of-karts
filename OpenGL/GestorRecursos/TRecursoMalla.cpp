#include "TRecursoMalla.hpp"

TRecursoMalla::TRecursoMalla(string path) {
	loadModel(path);
	inicializar();
}
TRecursoMalla::~TRecursoMalla() {
	//delete &VAO;
	//delete buffer;
}

void TRecursoMalla::inicializar() {	//buffers de datos adaptados a los que maneja openGL
	//Generar y activar el vertex array object, VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Generar los ids para los bufers de openGL
	glGenBuffers(4, buffer);
	//Buffers de vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	//Buffers de Texturas
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	//Buffers de normales
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	//Buffers de indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);


	//por ultimo desactivar el VAO
	glBindVertexArray(0);

}

//--------------------------------//
//------------ASSIMP--------------//
//--------------------------------//
void TRecursoMalla::loadModel(string path) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "(TRecursoMalla::loadModel) Error cargando assimp" << import.GetErrorString() << endl;
		return;
	}
	setNombre(path.c_str());
	cout << "(TRecursoMalla::loadModel) El recurso de malla se va a llamar :" << getNombre() << endl;
	processNode(scene->mRootNode, scene);
}
void TRecursoMalla::processNode(aiNode *node, const aiScene *scene) {
	//procesar todas las mallas que se encuentren en el fichero
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];		//se recoje la malla de la escena
		processMesh(mesh, scene);								// se guarda en el vector de malla
	}
	//lo mismo para los hijos que pueda tener
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}
void TRecursoMalla::processMesh(aiMesh *mesh, const aiScene *scene) {
	//reserva la memoria de los vectores 
	vertex.reserve(mesh->mNumVertices);
	uv.reserve(mesh->mNumVertices);
	normal.reserve(mesh->mNumVertices);
	indices.reserve(3 * mesh->mNumVertices);//son caras en base a triangulos , 3 vertices = 1 triangulo , 3*nvertices

	//recorrer cada uno de los vertices de la malla
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		//obtener la posicion de cada vertices
		aiVector3D pos = mesh->mVertices[i];
		vertex.push_back(glm::vec3(pos.x, pos.y, pos.z));
		//Obtener la posicion de las texturas
		if (mesh->HasTextureCoords(0)) {
			aiVector3D textCoord = mesh->mTextureCoords[0][i];
			uv.push_back(glm::vec2(textCoord.x, textCoord.y));
		}
		else uv.push_back(glm::vec2(0, 0));
		//Obtener las normales de los vectores
		aiVector3D normales = mesh->mNormals[i];
		normal.push_back(glm::vec3(normales.x, normales.y, normales.z));
	}
	//Obtener los indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {// las caras estan compuestas por 3 vertices, forman un triangulo
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);

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

void TRecursoMalla::draw(Shader *shader,glm::mat4 modelMatrix) {//se le pasa un shader
	/*for (GLuint i = 0; i < meshes.size; i++) {
		meshes[i].draw(shader);// dibujado de la malla
	}*/
	//shader->setMat4("model", modelMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

/*vector<vertex> *TRecursoMalla::getVertices() {
	return &vertices;
}
vector<GLuint> *TRecursoMalla::getIndices() {
	return &indices;
}*/