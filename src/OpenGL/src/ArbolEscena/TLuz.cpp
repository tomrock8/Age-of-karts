#include "TLuz.hpp"

/*
COMPONENTES DE LUZ:
	Ambiente: La luz que llega rebota en los objetos. La luz mas debil
	Difusa: La luz que llega directamente desde la fuente de luz, y rebota en todas las direcciones
	Especular: La luz que llega directamente desde la fuente de luz y rebota en una direccion(Brillo)
	Posicion: Posicion del punto de luz en el espacio
	Direccion: 	En caso de tratarse de una luz dirigida, la direccion del foco de la luz. Por ejemplo
				si esta apuntado al eje z negativo seria (0,0,-1)
	Atenuacion: Variable de control de la atenuacion	
				Cuanta mas bajo, mas distancia que engloba la luz, por lo tanto menos atenuacion
				Valores recomendados entre 0.025 y 0.005
	Corte: EN caso de tratarse de una luz dirigida, coseno del angulo que define el radio del circulo del foco


	Color final del objeto = Ambiente + Difusa + Especular
*/

/*
	Para definir una luz puntual, hay que pasarle un vector direccion todo 0 y un corte igual a 0
	Para definir una luz dirigida, hay que pasarle el vector direccion con la componente w siendo igual a 1 y un corte determinado
*/

TLuz::TLuz() {
	ambiente = glm::vec3(0.2f, 0.2f, 0.2f); //Valor recomendado 0.2f para no saturar la escena
	difusa = glm::vec3(0.5f, 0.5f, 0.5f);
	especular = glm::vec3(0.8f, 0.8f, 0.8f);
	
	//Atenuacion de la luz -> cuanta mas bajo, mas distancia, menos atenuacion
	//Valores recomendados -> entre 0.025 y 0.005
	attenuation = 0.01f; 
}

TLuz::TLuz(glm::vec3 ambiente, glm::vec3 difusa, glm::vec3 especular, glm::vec4 dir, float at, float corte) {
	this->lightDirection = dir;
	this->ambiente = ambiente;
	this->difusa = difusa;
	this->especular = especular;
	this->attenuation = at;
	this->cut = corte;
}

// METODOS SET
void TLuz::setIntensidad(glm::vec4 color) { TColor = color; }
void TLuz::setLightPosition(glm::vec3 pos) { lightPosition = pos; }
void TLuz::setActive(bool active) { this->active = active; }

// METODOS GET
glm::vec4 TLuz::getIntensidad() { return TColor; }
bool TLuz::getActive() { return active; }

//------------------------------------------//
//---------------SET DE LUCES---------------//
//------------------------------------------//
void TLuz::setLuzPuntual(Shader *shader, const char *nombre) {
	//Recoger el numero de luz actual
	char num = nombre[6];
	string name = "light[";
	name += num;
	name += "].";

	//Pasar los distintos datos al shader
	shader->setVec3( name + "position", lightPosition);
	shader->setVec4( name + "direction", lightDirection);
	shader->setVec3( name + "ambient", ambiente);
	shader->setVec3( name + "diffuse", difusa);
	shader->setVec3( name + "specular", especular);
	shader->setFloat( name + "k", attenuation);
	shader->setFloat( name + "cut", cut);
}

/*La posicion y orientacion de la camara y de las luces se maneja a traves de
las transformaciones de nodo padre, como para el resto de entidades.
Las camaras y las luces se manejan de forma especial,
puesto que se pueden encontrar en cualquier nodo del arbol y, sin embargo,
al dibujar la escena deben definirse antes de dibujar el resto de entidades.
Los metodos beginDraw y endDraw de las camaras y las luces estan vacios.*/
void TLuz::beginDraw(Shader *shader) { }
void TLuz::endDraw() { }

