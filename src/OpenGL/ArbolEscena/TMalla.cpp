#include "TMalla.hpp"
#include "TMotor.hpp"

TMalla::TMalla(std::vector <mesh *> m) {
	//Los vertices e indices son punteros a TRecursoMalla 
	malla = m;
	visible = true;
	//Creamos un bounding box por cada TRecursoMalla
	if (TMotor::instancia().getBoundingBoxes() == true) {
		for (int i = 0; i < malla.size(); i++) {
			boundingBox *b = new boundingBox(malla.at(i)->getMesh()->getCenter(), malla.at(i)->getMesh()->getSize());
			bBoxes.push_back(b);
		}
	}
}

TMalla::~TMalla() {
	std::cout << "Destructor de TMalla\n";
	visible = false; // Ocultar para no dibujar mas

	// Eliminar los punteros del vector
	if (malla.size() > 0) {
		for (int i = 0; i < malla.size(); i++) {
			delete malla.at(i);
		}
		malla.clear(); // Vaciar el vector
	}

	if (TMotor::instancia().getBoundingBoxes() && bBoxes.size() > 0) {
		for (int i = 0; i < bBoxes.size(); i++) {
			delete bBoxes.at(i);
		}
		bBoxes.clear(); 
	}
}

// METODOS GET
bool TMalla::isVisible() { return visible; }

// METODOS SET
void TMalla::setVisible(bool visible) { this->visible = visible; }

// DIBUJADO
void TMalla::beginDraw(Shader *shader) {// Depende del tipo de entidad
	/* Llamar al metodo de dibujado del recurso que se encarga del dibujado
	en OpenGL de los poligonos a partir de los vertices, las normales y
	las coordenadas de textura(Gestor) */
	if (visible)
		draw(shader);
}

void TMalla::draw(Shader *shader) {
	for (GLuint i = 0; i < malla.size(); i++) {
		//Activamos el shader
		shader->use();
		//Pasamos la modelMatrix de la malla al shader
		shader->setMat4("model", modelMatrix);
		//Pasamos la mvp al shader para calcular la posicion final de objeto
		glm::mat4 mvp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getActiveViewMatrix() * modelMatrix;
		shader->setMat4("mvp", mvp);
		//Pasamos la transpuesta de la inversa de la model matrix al shader para el calculo de las normales
		shader->setMat4("transInvModel", glm::transpose(glm::inverse(modelMatrix)));
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

		//Se llama al dibujado del bounding box
		if (TMotor::instancia().getBoundingBoxes() == true) {
			if (malla.size() != 2 && malla.size() != 4 && malla.size() != 5) bBoxes.at(i)->draw(modelMatrix);
		}

		if (malla.at(i)->getText()->getNombre() != NULL) {
			//Desactivamos las texturas usadas
			malla.at(i)->getText()->disableTexture();
		}

		//Desacivamos el buffer VAO antes del dibujado de la siguiente malla
		malla.at(i)->getMesh()->disableVAO();
	}

}

void TMalla::endDraw() {}