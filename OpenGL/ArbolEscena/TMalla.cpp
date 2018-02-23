#include "TMalla.hpp"

TMalla::TMalla(TRecursoMalla * m) {
	std::cout << "LLego hasta aqui y me doy con un canto en los dientes" << std::endl;
	//los vertices e indices son punteros a Trecurso malla 7
	vertices = m->getVertices();

	indices = m->getIndices();
	//textures = t;
	inicializar();//setear los bufers de vectores
}
void TMalla::inicializar() {					//buffers de datos adaptados a los que maneja openGL
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);						//genera un buffer en VBO para los 
	glGenBuffers(1, &EBO);						//genera un buffer en EBO

	glBindVertexArray(VAO);						//array activo es vao
	glBindBuffer(GL_ARRAY_BUFFER, VBO);			//cargar los datos en el vertex buffer VBO

	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	//cargar los datos en el vertex buffer EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Posiciones vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	//Posiciones Normales
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
	//Posiciones Texturas
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, textCoord));

	glBindVertexArray(0);//se suelta el array de vertices que se usaba
}

void TMalla::beginDraw() {// Depende del tipo de entidad
	std::cout << "Cargo Malla" << std::endl;
	
	/*llamar al	metodo de dibujado del recurso que se encarga
		del dibujado en OpenGL de los poligonos a partir
		de los vertices, las normales y las coordenadas de
		textura(Gestor)*/
}
void TMalla::endDraw(){}

void TMalla::draw(Shader shader) {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}