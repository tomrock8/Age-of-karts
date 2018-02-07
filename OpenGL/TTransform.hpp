#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "TEntidad.hpp"

using namespace std;

class TTransform : public TEntidad
{
public:
	//----constructor---//
	TTransform();
	//--Gestionar la matriz--//
	void cargar(glm::mat4 mat);				//recoge una matriz y la carga
	//---Transformacion-----//
	void transponer();
	void identidad();						//carga la identidad de la matriz
	void trasladar(GLfloat x, GLfloat y, GLfloat z);
	void rotar(GLfloat x, GLfloat y, GLfloat z, GLfloat grados);
	void escalar(GLfloat x, GLfloat y, GLfloat z);

	//--Sobrecarga de metodos de dibujado de entidad--//
	void beginDraw();
	void endDraw();
private:
	//TMatriz4x4 matriz
	glm::mat4 matriz;
}; 

