#include "TMalla.hpp"

TMalla::TMalla(std::vector <mesh *> m) {
	//los vertices e indices son punteros a Trecurso malla 7
	malla = m;
}

void TMalla::beginDraw(Shader *shader) {// Depende del tipo de entidad
	/* Llamar al metodo de dibujado del recurso que se encarga del dibujado 
	en OpenGL de los poligonos a partir de los vertices, las normales y 
	las coordenadas de textura(Gestor) */
	draw(shader);
}

void TMalla::draw(Shader *shader) {
	for (GLuint i = 0; i < malla.size(); i++) {
		//Pasamos la modelMatrix de la malla al shader para calcular la posicion final
		shader->setMat4("model", modelMatrix);
		//Activamos el buffer VAO donde hemos guardado los datos de la malla anteriormente
		malla.at(i)->getMesh()->activeVAO();
		if (malla.at(i)->getText()->getNombre() != NULL) {
			//Activamos la textura de la malla
			malla.at(i)->getText()->activeTexture(shader);
		}
			
		//Activamos el material
		malla.at(i)->getMat()->activeMaterial(shader);
		// Se llama al dibujado de la malla
		malla.at(i)->getMesh()->draw();

		if (malla.at(i)->getText()->getNombre() != NULL) {
			//Desactivamos las texturas usadas
			malla.at(i)->getText()->disableTexture();
		}
		
		//Desacivamos el buffer VAO antes del dibujado de la siguiente malla
		malla.at(i)->getMesh()->disableVAO();
	}
	
	
	
}

void TMalla::endDraw() {}

// METODOS GET
//TRecursoMalla * TMalla::getMalla() { return malla; }
//TRecursoMaterial * TMalla::getMaterial() { return material; }
//TRecursoTextura * TMalla::getTextures() { return textures; }