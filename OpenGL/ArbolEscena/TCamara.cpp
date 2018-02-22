#include "TCamara.hpp"

TCamara::TCamara() {

}

void TCamara::setPerspectiva(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano,GLfloat grados) {
	projectionMatrix = glm::perspective(
		glm::radians(grados), 			// El campo de visión vertical:zoom 90° (extra ancho) y 30° (zoom aumentado)
		Width / Height,					// Proporción ventana 4/3 == 800/600 
		corteCercano,					// Plano de corte cercano. Tan grande como sea posible o tendrás problemas de precisión.
		corteLejano						// Plano de corte lejano. Tan pequeño como se pueda.
	);
	
	
}
void TCamara::setParalela(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados) {
	projectionMatrix = glm::ortho(
		glm::radians(grados), 			// El campo de visión vertical:zoom 90° (extra ancho) y 30° (zoom aumentado)
		Width / Height,					// Proporción.ventana 4/3 == 800/600
		corteCercano,					// Plano de corte cercano. Tan grande como sea posible o tendrás problemas de precisión.
		corteLejano						// Plano de corte lejano. Tan pequeño como se pueda.
	);
	
}
void TCamara::setPadre(TNodo *n){

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
void TCamara::beginDraw() {
	std::cout << "Cargo Camara" << std::endl;

}
void TCamara::endDraw() {

}