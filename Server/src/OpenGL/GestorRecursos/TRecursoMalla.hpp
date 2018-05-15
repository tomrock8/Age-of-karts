#ifndef TRECURSOMALLA_H
#define TRECURSOMALLA_H

#include "libGL.hpp"
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "TRecurso.hpp"
#include "Shader.hpp"

using namespace std;



class TRecursoMalla :public TRecurso
{

public:
	TRecursoMalla(aiMesh *mesh, const char* nombre);
	void setFaces(std::vector<unsigned short> indices);
	~TRecursoMalla();
	void activeVAO(); // Activar el buffer de vertices
	void disableVAO(); // Desactivar el buffer de vertices

	// Guardado de elementos 
	void reserveVectorArrays(GLuint numeroVertices);
	void saveVectorTangentes(glm::vec3 vector);
	void saveVectorBitangentes(glm::vec3 vector);
	void saveVectorPosition(glm::vec3 vector);
	void saveVectorUV(glm::vec2 vector);
	void saveVectorNormal(glm::vec3 vector);
	void saveVectorIndices(GLuint vector);

	// METODOS SET
	virtual void setNombre(const char *s) override;

	// METODOS GET
	virtual const char *getNombre() override;
	std::vector<glm::vec3> getVertex();
	std::vector<glm::ivec3> getFaces();
	std::vector<unsigned short> getIndices();
	glm::vec3 getSize();
	glm::vec3 getCenter();

	// DIBUJADO
	void draw();
	void inicializar();

private:
	//-------------------------------------------------//
	//--vectores para almacenar los datos de la malla--//
	//-------------------------------------------------//
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> tangents; //tangentes     | 
	std::vector<glm::vec3> bitangents; //bitangentes |necesarios para los mapas de normales 
	std::vector<unsigned short> indices;
	std::vector<glm::ivec3> faces;

	//Buffers de OpenGL
	GLuint VAO; //Vertex Array Object
	GLuint buffer[6]; //Buffers para almacenar los datos de la malla y pasarselos a los shaders 

	//Atributos de la malla
	glm::vec3 sizeMalla; //Vector con el tamanyo en cada eje de la malla
	glm::vec3 centerMalla; //Vector que contiene el punto central de la malla
	void calculateSizeAndCenter(); //Funcion para calcular el tamanyo y el centro de la malla
};
#endif