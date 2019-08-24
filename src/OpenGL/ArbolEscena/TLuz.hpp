#ifndef TLUZ_H
#define TLUZ_H

#include "libGL.hpp"
#include "TEntidad.hpp"
#include <vector>

class Shader;

class TLuz : public TEntidad {
public:
	TLuz();
	TLuz(glm::vec3 ambiente, glm::vec3 difusa, glm::vec3 especular, glm::vec4 dir, float at, float corte, bool shadow, bool on);
	~TLuz() override;

	// SOMBRAS
	void renderMap();
	void unbindDepthBuffer();
	void configureShadow(Shader* s);

	// --- LUZ DIRIGIDA ---
	void calculateDepthMap();
	void calculateLightMatrix();
	void renderDepthMap();
	void configureDirectionalShadow(Shader* s);

	// --- LUZ PUNTUAL ---
	void calculateCubeDepthMap();
	void calculateLightMatrixes();
	void renderCubeDepthMap();
	void configurePointShadow(Shader* s);

	// METODOS GET
	bool getActive();
	float getLightType();
	glm::vec3 getPosition();
	glm::vec4 getDirection();

	// METODOS SET
	void setLuz(Shader* shader, const char* nombre);
	void setActive(bool active);
	void setLightPosition(glm::vec3 pos);

	// DIBUJADO
	virtual void beginDraw(Shader* shader) override;
	virtual void endDraw() override;

private:
	//Atributos y caracteristicas de la luz
	char numLight; //Numero de luz
	glm::vec3 lightPosition;
	glm::vec4 lightDirection; //Para la luz dirigida -> si la luz es puntual es todo 0.0f

	glm::vec3 ambiente;
	glm::vec3 difusa;
	glm::vec3 especular;
	float attenuation;
	float cut;

	bool sombras; //Booleano para activar o desactivar las sombras
	bool active; //Booleano para apagar o encender una luz

	//SOMBRAS
	const int DEPTH_WIDTH = 1024, DEPTH_HEIGHT = 1024; //Alto y ancho del mapa de profundidad
	// --- LUZ DIRIGIDA ---
	unsigned int depthMap; //Mapa de profundidad = perspectiva de la escena desde el punto de luz
	unsigned int depthTexture; //Textura que recoge el mapa de profundidad
	glm::mat4 lightMatrix; //Matriz para transformar objetos al punto de vista de la luz
	// --- LUZ PUNTUAL ---
	unsigned int depthCubeMap; //Cubo de profundidad = perspectiva de la escena desde el punto de luz (en la direccion positiva-negativa de cada eje)
	unsigned int depthCubeTexture; //Textura que recoge el cubo de profundidad
	std::vector<glm::mat4> lightMatrixes; //Vector que guarda la matriz de luz para cada cara del cubo de profundidad
};
#endif