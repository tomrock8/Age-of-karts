#pragma once
#include <iostream>
//#include <GL/glew.h>
#include <glm/glm.hpp>
#include "TEntidad.hpp"


using namespace std;

class TLuz : public TEntidad
{
public:
	//--Constructor--//
	TLuz();
	void setIntensidad(glm::vec4 color);		// recoge la intensidad Tcolor y la almacena en la variable color
	glm::vec4 getIntensidad();					// devuelve la intensidad que sera de tipo color
	//void setPosition();						//pasarle una vector de 3 posiciones

	//--Sobrecarga de metodos de dibujado de entidad--//
	void beginDraw();
	void endDraw();
private:
	glm::vec4 TColor;

};
