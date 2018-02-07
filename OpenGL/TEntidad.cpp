#include "TEntidad.hpp"

//-------------------//
//-----CONSTRUCTOR---//
//-------------------//
TEntidad::TEntidad(){
	// Genera una matriz difícil de leer pero al menos es 4x4
	projectionMatrix = glm::perspective(
		glm::radians(90.0f), 	// El campo de visión vertical:zoom 90° (extra ancho) y 30° (zoom aumentado)
		4.0f / 3.0f,				// Proporción.ventana 4/3 == 800/600  // Se debera controlar en opciones si se hace ventana redimensionable
		0.1f,						// Plano de corte cercano. Tan grande como sea posible o tendrás problemas de precisión.
		100.0f						// Plano de corte lejano. Tan pequeño como se pueda.
	);
	
}
//-------------------//
//-----DESTRUCTOR----//
//-------------------//
TEntidad::~TEntidad() {

}
//--------------------//
//-----MEDOTOS SET----//
//--------------------//
void TEntidad::setModelView(GLfloat x, GLfloat y, GLfloat z) {
	viewMatrix = glm::translate(glm::mat4(), glm::vec3(x,y,z));
}

void TEntidad::setMVPMatrix(glm::mat4 MVP) {//Almacenar la matriz MVP p
	MVPmatrix = MVP;
}
//-------------------//
//---METODOS GET-----//
//-------------------//
glm::mat4 TEntidad::getprojectionMatrix() {
	return projectionMatrix;
}
glm::mat4 TEntidad::getViewMatrix() {
	return viewMatrix;
}
glm::mat4 TEntidad::getMVPMatrix() {
	return MVPmatrix;
}
//-------------------//
//------DIBUJADO-----//
//-------------------//
void TEntidad::beginDraw() {

}
void TEntidad::endDraw() {

}