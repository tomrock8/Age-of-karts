#include "TLuz.hpp"


void TLuz::setIntensidad(glm::vec4 color) {
	TColor = color;
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
void TLuz::beginDraw() {

}
void TLuz::endDraw() {

}