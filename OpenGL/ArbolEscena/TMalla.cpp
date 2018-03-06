#include "TMalla.hpp"

TMalla::TMalla(TRecursoMalla * m, TRecursoTextura *t) {
	//los vertices e indices son punteros a Trecurso malla 7
	malla = m;
	texture = t;
	//textures = t;
}


void TMalla::beginDraw(Shader *shader) {// Depende del tipo de entidad
	std::cout << "(TMalla::beginDraw()) Cargo Malla" << std::endl;
	draw(shader);
	/*llamar al	metodo de dibujado del recurso que se encarga
		del dibujado en OpenGL de los poligonos a partir
		de los vertices, las normales y las coordenadas de
		textura(Gestor)*/

}
void TMalla::endDraw() {}

void TMalla::draw(Shader *shader) {
	//primero el material y texura, despues la malla
	shader->use();
	shader->setMat4("model", modelMatrix);
	texture->activeTexture(shader->getProgram());
	malla->draw(shader, modelMatrix); // Llama al dibujado de la malla
	texture->disableTexture();
}

TRecursoMalla * TMalla::getMalla() {
	return malla;
}
/*
glm::mat4 TMalla::getmodelMatrix() {
	return modelMatrix;
}
*/