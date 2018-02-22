#include "TEntidad.hpp"

std::stack<glm::mat4> TEntidad::matStack;
glm::mat4 TEntidad::viewMatrix;
glm::mat4 TEntidad::projectionMatrix;
glm::mat4 TEntidad::MVPmatrix;


/*
//-------------------//
//-----CONSTRUCTOR---//
//-------------------//
TEntidad::TEntidad(){
	

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
//	viewMatrix = glm::translate(glm::mat4(), glm::vec3(x,y,z));
}

void TEntidad::setMVPMatrix(glm::mat4 MVP) {//Almacenar la matriz MVP p
	MVPmatrix = MVP;
}
//-------------------//
//---METODOS GET-----//
//-------------------//
glm::mat4 TEntidad::getprojectionMatrix() {
	//return projectionMatrix;
}
glm::mat4 TEntidad::getViewMatrix() {
	//return viewMatrix;
}
glm::mat4 TEntidad::getMVPMatrix() {
	//return MVPmatrix;
}
//-------------------//
//------DIBUJADO-----//
//-------------------//
void TEntidad::beginDraw() {

}
void TEntidad::endDraw() {

}
*/