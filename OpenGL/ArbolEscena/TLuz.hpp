#ifndef TLUZ_H
#define TLUZ_H

#include <iostream>
#include "TEntidad.hpp"
#include "shader/Shader.hpp"
#include "TNodo.hpp"


using namespace std;

class TLuz : public TEntidad
{
public:
	//--Constructor--//
	TLuz();
	TLuz(glm::vec3 ambiente, glm::vec3 difusa, glm::vec3 especular);
	void setIntensidad(glm::vec4 color);		// recoge la intensidad Tcolor y la almacena en la variable color
	glm::vec4 getIntensidad();					// devuelve la intensidad que sera de tipo color
	//void setPosition();						//pasarle una vector de 3 posiciones
	//---Luz puntual---//
	
	void setLuzPuntual(Shader *shader);
	bool getActive();
	void setActive(bool active);
	void setLightMatrix(glm::vec4 pos);
	//--Sobrecarga de metodos de dibujado de entidad--//
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;
private:
	bool active;
	glm::vec4 lightPosition;
	glm::vec4 TColor;
	
	//glm::mat4 modelMatrix;
	glm::vec3 ambiente;
	glm::vec3 difusa;
	glm::vec3 especular;
	

};
#endif