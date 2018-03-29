#include "TMalla.hpp"

TMalla::TMalla(TRecursoMalla * m, TRecursoMaterial *mat, TRecursoTextura *t) {
	//los vertices e indices son punteros a Trecurso malla 7
	malla = m;
	material = mat;
	malla->inicializar();
	textures = t;
}

void TMalla::beginDraw(Shader *shader) {// Depende del tipo de entidad
	/* Llamar al metodo de dibujado del recurso que se encarga del dibujado 
	en OpenGL de los poligonos a partir de los vertices, las normales y 
	las coordenadas de textura(Gestor) */
	draw(shader);
}

void TMalla::draw(Shader *shader) {
	//Pasamos la modelMatrix de la malla al shader para calcular la posicion final
	shader->setMat4("model", modelMatrix);
	//Activamos el buffer VAO donde hemos guardado los datos de la malla anteriormente
	malla->activeVAO();
	//Activamos la textura de la malla
	textures->activeTexture(shader);
	//Activamos el material
	material->activeMaterial(shader);
	// Se llama al dibujado de la malla
	malla->draw();
	//Desactivamos las texturas usadas
	textures->disableTexture();
	//Desacivamos el buffer VAO antes del dibujado de la siguiente malla
	malla->disableVAO();
}

void TMalla::endDraw() {}

// METODOS GET
TRecursoMalla * TMalla::getMalla() { return malla; }
TRecursoMaterial * TMalla::getMaterial() { return material; }
TRecursoTextura * TMalla::getTextures() { return textures; }