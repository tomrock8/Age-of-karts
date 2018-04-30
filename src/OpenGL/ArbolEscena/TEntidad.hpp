#ifndef TENTIDAD_H
#define TENTIDAD_H

#include "libGL.hpp"
#include "Shader.hpp"
#include <stack>

class TEntidad {
public:
	TEntidad() {};
	~TEntidad() {};

	// METODOS SET
	void setModelMatrix(glm::mat4 modelMatrix);
	void setViewMatrix(glm::mat4 viewMatrix);
	void setProjectionMatrix(glm::mat4 projectionMatrix);

	// METODOS GET
	glm::mat4 getModelViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getModelViewProjectionMatrix();
	glm::mat4 getViewMatrix();

	// DIBUJADO
	virtual void beginDraw(Shader *shader) = 0; //se ejecuta cuando se baja un nivel en el arbol de la escena
	virtual void endDraw() = 0; //se ejecuta cuando se sube un nivel en el arbol de la escena

protected://las matrices tienen que ser estaticas como las pilas (compartidas por todas las entidades)
	//se inicializa la pila cuando se crea el motor
	static std::stack<glm::mat4> matStack;		// Pila de OpenGL
	static glm::mat4 modelMatrix;				// Matriz modelo
	static glm::mat4 viewMatrix;				// Matriz de vista
	static glm::mat4 projectionMatrix;			// Matriz de proyeccion de los puntos en el espacio,  punto de projeccion sera finito
	static glm::mat4 MVPmatrix;					// Matriz Modelo Vista Proyeccion proyeccion * vista * modelo

};
#endif