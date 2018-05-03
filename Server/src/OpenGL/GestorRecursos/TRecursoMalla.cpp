#include "TRecursoMalla.hpp"

TRecursoMalla::TRecursoMalla(aiMesh *mesh, const char* nombre) {
	//recorrer cada uno de los vertices de la malla
	reserveVectorArrays(mesh->mNumVertices);
	setNombre(nombre);
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		
		//obtener la posicion de cada vertices
		aiVector3D pos = mesh->mVertices[i];
		saveVectorPosition(glm::vec3(pos.x, pos.y, pos.z));
		//Obtener la posicion de las texturas
		if (mesh->HasTextureCoords(0)) {
			aiVector3D textCoord = mesh->mTextureCoords[0][i];
			saveVectorUV(glm::vec2(textCoord.x, textCoord.y));
		}
		else saveVectorUV(glm::vec2(0, 0));

		//Obtener las normales de los vectores
		aiVector3D normales = mesh->mNormals[i];
		saveVectorNormal(glm::vec3(normales.x, normales.y, normales.z));

		//Obtener las tangentes de cada vertice
		if (mesh->HasTangentsAndBitangents()) {
			aiVector3D tangentes = mesh->mTangents[i];
			saveVectorTangentes(glm::vec3(tangentes.x, tangentes.y, tangentes.z));

			//Obtener las bitangentes de cada vertice
			aiVector3D bitangentes = mesh->mBitangents[i];
			saveVectorBitangentes(glm::vec3(bitangentes.x, bitangentes.y, bitangentes.z));
		}
		else {
			saveVectorTangentes(glm::vec3(0.06f, 0.47f, 0.8f));
			saveVectorBitangentes(glm::vec3(-0.99f, 0.001f, 0.006f));
		}

	}

	//Obtener los indices
	int j = 0;
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {// las caras estan compuestas por 3 vertices, forman un triangulo
		saveVectorIndices(mesh->mFaces[i].mIndices[0]);
		saveVectorIndices(mesh->mFaces[i].mIndices[1]);
		saveVectorIndices(mesh->mFaces[i].mIndices[2]);
	}

	//cout << "TRecursoMalla (47) : Tengo " << faces.size() << " caras" << endl;
	//for (GLuint i = 0; i < mesh->mNumFaces; i++) {// las caras estan compuestas por 3 vertices, forman un triangulo
	//	faces.push_back(glm::ivec3(mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]));
	//}
	//cout << "TRecursoMalla (52) : Tengo " << faces.size() << " caras" << endl;


}
void TRecursoMalla::setFaces(std::vector<unsigned short> indices) {
	////std::vector<glm::ivec3> faces;
	//int j = 0;
	//for (GLuint i = 0; j < indices.size(); i++) {
	//	
	//	
	//}
}
TRecursoMalla::~TRecursoMalla() {
	//delete &VAO;
	//delete buffer;
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(6, buffer);
}

void TRecursoMalla::activeVAO() { glBindVertexArray(VAO); }
void TRecursoMalla::disableVAO() { glBindVertexArray(0); }

// METODOS GET
const char *TRecursoMalla::getNombre() { return nombre->c_str(); }

// METODOS SET
void TRecursoMalla::setNombre(const char *s) { 
	nombre = new std::string(s);

}

void TRecursoMalla::draw() {
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

}

void TRecursoMalla::inicializar() {	//buffers de datos adaptados a los que maneja openGL
	//Generar y activar el vertex array object, VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Generar los ids para los bufers de openGL
	glGenBuffers(6, buffer);
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
	//Buffers de tangentes
	glBindBuffer(GL_ARRAY_BUFFER, buffer[3]);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);
	//Buffers de bitangentes
	glBindBuffer(GL_ARRAY_BUFFER, buffer[4]);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(4);
	//Buffers de indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	//por ultimo desactivar el VAO
	glBindVertexArray(0);
}

// ------------------------------------------
//	Guardado de valores en los vectores
// ------------------------------------------

void TRecursoMalla::reserveVectorArrays(GLuint mNumVertices) {
	//reserva la memoria de los vectores 
	vertex.reserve(mNumVertices);
	uv.reserve(mNumVertices);
	normal.reserve(mNumVertices);
	indices.reserve(3 * mNumVertices);//son caras en base a triangulos , 3 vertices = 1 triangulo , 3*nvertices
	tangents.reserve(mNumVertices);
	bitangents.reserve(mNumVertices);

}
void TRecursoMalla::saveVectorTangentes(glm::vec3 m) { tangents.push_back(m); }
void TRecursoMalla::saveVectorBitangentes(glm::vec3 m) { bitangents.push_back(m); }
void TRecursoMalla::saveVectorPosition(glm::vec3 m) { vertex.push_back(m); }
void TRecursoMalla::saveVectorUV(glm::vec2 m) { uv.push_back(m); }
void TRecursoMalla::saveVectorNormal(glm::vec3 m) { normal.push_back(m); }
void TRecursoMalla::saveVectorIndices(GLuint m) { indices.push_back(m); }
std::vector<glm::ivec3> TRecursoMalla::getFaces() {
	return faces;
}
std::vector<unsigned short> TRecursoMalla::getIndices()
{
	return indices;
}
std::vector<glm::vec3> TRecursoMalla::getVertex() {
	return vertex;
}
