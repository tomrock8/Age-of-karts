#include "TCamara.hpp"

TCamara::TCamara() {}
TCamara::~TCamara() {
	setActive(false);
//	std::cout << "Destructor de TCamara\n";
}

bool TCamara::getActive() { return active; }
void TCamara::setActive(bool s) { active = s; }

void TCamara::setPerspective(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados, float aspectRatio) {
	projectionMatrix = glm::perspective(
		glm::radians(grados), 			// El campo de vision vertical:zoom 90 (extra ancho) y 30 (zoom aumentado)
		static_cast<GLfloat>(aspectRatio),					// Proporcion ventana 4/3 == 800/600 
		corteCercano,					// Plano de corte cercano. Tan grande como sea posible o tendras problemas de precision.
		corteLejano						// Plano de corte lejano. Tan enano como se pueda.
	);
}

void TCamara::setParalela(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados) {
	projectionMatrix = glm::ortho(
		glm::radians(grados), 			// El campo de vision vertical:zoom 90 (extra ancho) y 30 (zoom aumentado)
		Width / Height,					// Proporcion.ventana 4/3 == 800/600
		corteCercano,					// Plano de corte cercano. Tan grande como sea posible o tendras problemas de precision.
		corteLejano						// Plano de corte lejano. Tan enano como se pueda.
	);
}

/*La posicion y orientacion de la camara y de las luces se maneja a traves de
las transformaciones de nodo padre, como para el resto de entidades.
Las camaras y las luces se manejan de forma especial,
puesto que se pueden encontrar en cualquier nodo del arbol y, sin embargo,
al dibujar la escena deben definirse antes de dibujar el resto de entidades.
Los metodos beginDraw y endDraw de las camaras y las luces estan vacios.*/
void TCamara::beginDraw(Shader *shader) {}
void TCamara::endDraw() {}
