#include "TMalla.hpp"
#include "TMotor.hpp"
#include "mesh.hpp"
#include "boundingBox.hpp"
#include "libGL.hpp" 

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
	//std::cout << "Destructor de TMalla\n";
	visible = false; // Ocultar para no dibujar mas
	
	// Eliminar los punteros del vector
	if (malla.size() > 0) {
		//cout<<"NUMERO DE MESHES PREMO:::"<< malla.size() <<endl;
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
	//Calculamos la posicion de la camara
	glm::vec4 defaultVector(0, 0, 0, 1);
	glm::vec4 posC = glm::inverse(TMotor::instancia().getActiveViewMatrix()) * defaultVector;
	for (GLuint i = 0; i < malla.size(); i++) {
		//Calculamos la distancia entre la camara y el punto medio de cada objeto para el Level Of Detail
		glm::vec3 worldPos = glm::vec3(modelMatrix * glm::vec4(malla.at(i)->getMesh()->getCenter(), 1.0));
		float dist = glm::distance(glm::vec3(posC), worldPos);

		string nom(malla.at(i)->getMesh()->getNombre()); //Nombre de cada malla dibujada
		//Si el Level Of Detail esta desactivado, establecemos la distancia a 0.0 para que todos los objetos siempre se dibujen
		if ( TMotor::instancia().getLevelOfDetail() == false){
			dist = 0.0f;
		}
		//No qurememos que el Level of Detail afecta a los elementos del terreno como la carretera, la montana y el mar
		if (nom.find("mar") != std::string::npos || nom.find("montana") != std::string::npos || nom.find("carretera") != std::string::npos){
			dist = 0.0f;}
		//Si la distancia a la que se encuentra la malla, respecto a la camara, es menor que la distancia maxima de dibujado, entonces pintamos el modelo guardado
		if (dist < TMotor::instancia().getDrawingDistance()){
			//Activamos el shader
			shader->use();
			//Pasamos la modelMatrix de la malla al shader
			shader->setMat4("model", modelMatrix);
			//Pasamos la mvp al shader para calcular la posicion final de objeto
			glm::mat4 mvp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getActiveViewMatrix() * modelMatrix;
			shader->setMat4("mvp", mvp);
			//Pasamos la transpuesta de la inversa de la model matrix al shader para el calculo de las normales
			shader->setMat4("transInvModel", glm::transpose(glm::inverse(modelMatrix)));
			//Si estamos dibujando las sombras proyectadas, pasamos tambien el tamanyo de la malla
			if (strcmp(shader->getName(), "shaderProjectedShadows") == 0){
				shader->setFloat("heightMesh", malla.at(i)->getMesh()->getSize()[1]);
			}
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
				bBoxes.at(i)->draw(modelMatrix);
			}

			if (malla.at(i)->getText()->getNombre() != NULL) {
				//Desactivamos las texturas usadas
				malla.at(i)->getText()->disableTexture();
			}

			//Desacivamos el buffer VAO antes del dibujado de la siguiente malla
			malla.at(i)->getMesh()->disableVAO();
		} else { //En caso contrario, dibujamos el cubo (malla con menos poligonos)
			TMotor::instancia().drawCube(modelMatrix, malla.at(i)->getMesh()->getCenter());
		}
	}

}

void TMalla::endDraw() {}