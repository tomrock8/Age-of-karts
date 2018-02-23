#ifndef TLUZ_H
#define TLUZ_H

#include <iostream>
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
	//---Luz puntual---//
	void setLuzPuntual(GLfloat posX,GLfloat posY, GLfloat posZ);
	//--Sobrecarga de metodos de dibujado de entidad--//
	virtual void beginDraw() override;
	virtual void endDraw() override;
private:
	glm::vec4 TColor;
	//---Luz puntual
	glm::vec3 lightPosition;	
	

};
#endif