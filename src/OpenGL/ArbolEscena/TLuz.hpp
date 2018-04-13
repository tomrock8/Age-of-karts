#ifndef TLUZ_H
#define TLUZ_H

#include <iostream>
#include "TEntidad.hpp"
#include "Shader.hpp"
#include "TNodo.hpp"

using namespace std;

class TLuz : public TEntidad
{
public:
	TLuz();
	TLuz(glm::vec3 ambiente, glm::vec3 difusa, glm::vec3 especular, glm::vec4 dir, float at, float corte);

	// SOMBRAS
	void calculateDepthMap();
	void calculateLightMatrix();
	void renderDepthMap();
	void unbindDepthBuffer();
	void configureShadow();

	// METODOS GET
	glm::vec4 getIntensidad();	// devuelve la intensidad que sera de tipo color
	bool getActive();

	// METODOS SET
	void setIntensidad(glm::vec4 color);	// recoge la intensidad Tcolor y la almacena en la variable color
	//void setPosition();	//pasarle una vector de 3 posiciones
	// Luz puntual
	void setLuzPuntual(Shader *shader, const char* nombre);
	void setActive(bool active);
	void setLightPosition(glm::vec3 pos);

	// DIBUJADO
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;

private:
	//Atributos y caracteristicas de la luz
	bool active;
	glm::vec4 TColor;
	
	glm::vec3 lightPosition;
	glm::vec4 lightDirection; //Para la luz dirigida -> si la luz es puntual es todo 0.0f

	glm::vec3 ambiente;
	glm::vec3 difusa;
	glm::vec3 especular;
	float attenuation;
	float cut;

	//Sombras
	const int DEPTH_WIDTH = 1024, DEPTH_HEIGHT = 1024; //Alto y ancho del mapa de profundidad
	unsigned int depthMap; //Mapa de profundidad = perspectiva de la escena desde el punto de luz
	unsigned int depthTexture; //Textura que recoge el mapa de profundidad
	glm::mat4 lightMatrix; //Matriz para transformar objetos al punto de vista de la luz
};
#endif