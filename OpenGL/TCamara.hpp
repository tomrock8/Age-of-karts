#pragma once

#include  <GLFW/glfw3.h>
#include  <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "TEntidad.hpp"
#include "TNodo.hpp"

using namespace std;

class TCamara : public TEntidad
{
public:
	void setPerspectiva(GLfloat Height, GLfloat Widht, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);
	void setParalela(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);
	void setPadre(TNodo *n);
	//--Sobrecarga de metodos de dibujado de entidad--//
	void beginDraw();
	void endDraw();
private:
	bool esPerspectiva;
	GLfloat cercano, lejano, distancia;
	glm::mat4 projMatrix;
	
};