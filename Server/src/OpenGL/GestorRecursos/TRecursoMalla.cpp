#include "TRecursoMalla.hpp"

TRecursoMalla::TRecursoMalla(aiMesh *mesh, const char* nombre) {
	//recorrer cada uno de los vertices de la malla
	reserveVectorArrays(mesh->mNumVertices);
	setNombre(nombre);
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		
		//obtener la posicion de cada vertice
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

	//Por ultimo, calculamos el tamanyo y centro de la malla (necesarios para los bounding boxes)
	calculateSizeAndCenter();

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
	//Eliminamos los buffers creados mediante OpenGL
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(6, buffer);
	vertex.clear();
	normal.clear();
	uv.clear();
	tangents.clear(); //tangentes     | 
	bitangents.clear(); //bitangentes |necesarios para los mapas de normales 
	indices.clear();
	faces.clear();

}

void TRecursoMalla::activeVAO() {glBindVertexArray(VAO); }
void TRecursoMalla::disableVAO() { glBindVertexArray(0); }

// METODOS GET
const char *TRecursoMalla::getNombre() { return nombre->c_str(); }
glm::vec3 TRecursoMalla::getSize() { return sizeMalla; }
glm::vec3 TRecursoMalla::getCenter() { return centerMalla; }
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
	//Buffers de texturas
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

// ------------------------------------------
//	Calculo del tamanyo y centro de la malla
// ------------------------------------------

void TRecursoMalla::calculateSizeAndCenter(){

	float min_x, max_x; //Minima y maxima coordenada de la malla en el eje x
	float min_y, max_y; //Minima y maxima coordenada de la malla en el eje y
	float min_z, max_z; //Minima y maxima coordenada de la malla en el eje z

	//Igualamos cada eje al valor del primer vertice guardado de la malla
	min_x = max_x = vertex.at(0)[0];
	min_y = max_y = vertex.at(0)[1];
	min_z = max_z = vertex.at(0)[2];

	//Recorremos todos los vertices de la malla
	for (int i = 0; i < vertex.size(); i++){
		//Si la coordenada x es menor que la que tenemos guardada, la sustituimos
		if (min_x > vertex.at(i)[0]) min_x = vertex.at(i)[0];
		//Si la coordenada x es mayor que la que tenemos guardada, la sustituimos
		if (max_x < vertex.at(i)[0]) max_x = vertex.at(i)[0];
		//Si la coordenada y es menor que la que tenemos guardada, la sustituimos
		if (min_y > vertex.at(i)[1]) min_y = vertex.at(i)[1];
		//Si la coordenada y es mayor que la que tenemos guardada, la sustituimos
		if (max_y < vertex.at(i)[1]) max_y = vertex.at(i)[1];
		//Si la coordenada z es menor que la que tenemos guardada, la sustituimos
		if (min_z > vertex.at(i)[2]) min_z = vertex.at(i)[2];
		//Si la coordenada z es mayor que la que tenemos guardada, la sustituimos
		if (max_z < vertex.at(i)[2]) max_z = vertex.at(i)[2];
	}

	//Una vez obtenidas la maxima y minima coordenada de la malla en cada eje, podemos calcular su tamanyo y centro
	// TAMANYO = maxima coordenada en un eje - minima coordenada en ese mismo eje
	// CENTRO = maxima coordenada en un eje + minima coordenada en ese mismo eje, y posteriormente dividido entre 2
	//Guardamos los datos en los arrays que hemos definido para ello
	sizeMalla = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
	centerMalla = glm::vec3((max_x + min_x)/2, (max_y + min_y)/2, (max_z + min_z)/2);
}
