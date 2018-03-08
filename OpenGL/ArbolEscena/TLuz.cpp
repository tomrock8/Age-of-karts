#include "TLuz.hpp"
/*COMPONENTES DE LUZ:
	Ambiente: La luz que llega rebota en los objetos . La luz mas debil
	Difusa:	  La luz que llega directamente desde la fuente de luz, y rebota en todas las direcciones
			Ambiente + Difusa = Color de objeto
	Especular:La luz que llega	directamente desde la fuente de luz y rebota en una direccion(Brillo)

*/


TLuz::TLuz() {
	ambiente = glm::vec3(0.2f, 0.2f, 0.2f);
	difusa = glm::vec3(0.5f, 0.5f, 0.5f);
	especular = glm::vec3(0.8f, 0.8f, 0.8f);

	//modelMatrix = glm::mat4();
}
TLuz::TLuz(glm::vec3 ambiente, glm::vec3 difusa, glm::vec3 especular) {
	this->ambiente = ambiente;
	this->difusa = difusa;
	this->especular = especular;

	//modelMatrix = glm::mat4();
}

void TLuz::setIntensidad(glm::vec4 color) {
	TColor = color;
}

//------------------------------------------//
//---------------SET DE LUCES---------------//
//------------------------------------------//
void TLuz::setLuzPuntual(Shader *shader) {
	shader->setVec3("light_1.ambient", ambiente);
	shader->setVec3("light_1.diffuse", difusa);
	shader->setVec3("light_1.specular", especular);
	modelMatrix[3][0] = 0;		// POS X
	modelMatrix[3][1] = 100;	// POS Y (Altura)
	modelMatrix[3][2] = 100;	// POS X
	modelMatrix[3][3] = 1;

	glm::vec3 res = glm::vec3(modelMatrix * glm::vec4(0, 0, 0, 1));
	shader->setVec3("light_1.position", res);
}


glm::vec4 TLuz::getIntensidad() {
	return TColor;
}
/*La posicion y orientacion de la camara y de las
luces se maneja a traves de las transformaciones
de nodo padre, como para el resto de entidades.
Las camaras y las luces se manejan
de forma especial, puesto que se pueden encontrar
en cualquier nodo del arbol y, sin embargo, al
dibujar la escena deben definirse antes de dibujar
el resto de entidades.
Los metodos beginDraw y endDraw de las cámaras y las luces suelen estar vacios.*/
void TLuz::beginDraw(Shader *shader) {
	std::cout << "(TLuz::beginDraw()) Cargo Luz" << std::endl;
	
}
void TLuz::endDraw() {

}
void TLuz::setActive(bool active) {
	this->active = active;
}
bool TLuz::getActive() {
	return active;
}
void TLuz::setLightMatrix(glm::vec4 pos) {
	lightPosition = pos;
}