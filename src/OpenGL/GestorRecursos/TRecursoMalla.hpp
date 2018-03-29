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
	TRecursoMalla(aiMesh *mesh);
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
	//vector<vertex> *getVertices();
	//vector<GLuint> *getIndices();
	virtual const char *getNombre() override;



	// DIBUJADO
	void draw();
	void inicializar();

private:
	//std::vector<vertex> vertices;
	//std::vector<GLuint> indices;
	//-------------------------------------------------//
	//--vectores para almacenar los datos de la malla--//
	//-------------------------------------------------//
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> tangents; //tangentes     | 
	std::vector<glm::vec3> bitangents; //bitangentes |necesarios para los mapas de normales 
	std::vector<unsigned short> indices;

	GLuint buffer[6];  // buffer para almacenar los datos de la malla y pasarselos a los shaders 
	//variables de renderizado
	GLuint VAO;// Vertex Array Object*/
	//GLuint VBO;//|
	//GLuint EBO;//|Vertex Buffer Object
};
#endif