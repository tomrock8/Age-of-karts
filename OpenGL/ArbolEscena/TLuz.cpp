#include "TLuz.hpp"
/*COMPONENTES DE LUZ:
	Ambiente: La luz que llega rebota en los objetos . La luz mas debil 
	Difusa:	  La luz que llega directamente desde la fuente de luz, y rebota en todas las direcciones
			Ambiente + Difusa = Color de objeto
	Especular:La luz que llega	directamente desde la fuente de luz y rebota en una direccion(Brillo)

*/	

GLfloat ambiente[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat difusa[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat especular[] = { 0.8f, 0.8f, 0.8f, 1.0f};
TLuz::TLuz() {

}
void TLuz::setIntensidad(glm::vec4 color) {
	TColor = color;
}
//------------------------------------------//
//---------------SET DE LUCES---------------//
//------------------------------------------//
void TLuz::setLuzPuntual(GLfloat posX, GLfloat posY, GLfloat posZ) {
	glEnable(GL_LIGHTING);									    //Habilitar iluminacion
	GLfloat puntualLightPosition[] = { posX, posY, posZ, 1.0 }; //ultimo parametro indica que es luz puntual
	glLightfv(GL_LIGHT0, GL_POSITION, puntualLightPosition);	//posicion de la luz
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30);					//apertura en grados de la luz . Si es diferente de 180 sera puntual
	GLfloat dir[] = { 0,-1, 0, 0 };								//Direccion donde apuntara (abajo)
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	glEnable(GL_LIGHT0);									    //Habilitar la luz 0 = puntual se encendera con valores por defecto como la posicion (0,0,0). 
	//glEnable(GL_NORMALIZE);									//Convierte los vectores unitarios en normales(consume bastante)
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
	std::cout << "Cargo Luz" << std::endl;

}
void TLuz::endDraw() {

}