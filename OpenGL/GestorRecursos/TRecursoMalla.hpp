#ifndef TRECURSOMALLA_H
#define TRECURSOMALLA_H

#include "libGL.hpp"
#include <iostream>
#include <vector>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "TRecurso.hpp"
#include "Shader/Shader.hpp"

using namespace std;



class TRecursoMalla :public TRecurso
{

public:
	//---constructor---//
	TRecursoMalla(string path);
	//---destructor---//
	~TRecursoMalla();

	//--------------------------------//
	//-------HERENCIA DE RECURSOS-----//
	//--------------------------------//
	virtual const char *getNombre() override;
	virtual void setNombre(const char *s) override;
	//-------------------------------//
	//----------METODOS DE GET-------//
	//-------------------------------//
	/*vector<vertex> *getVertices();
	vector<GLuint> *getIndices();*/

	//-------------------------------//
	//----------DIBUJADO-------------//
	//-------------------------------//
	void draw(Shader *shader, glm::mat4 modelMatrix);


private:
	//std::vector<vertex> vertices;
	//std::vector<GLuint> indices;
	//-------------------------------------------------//
	//--vectores para almacenar los datos de la malla--//
	//-------------------------------------------------//
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;
	std::vector<unsigned short> indices;

	GLuint buffer[4];	// buffer para almacenar los ids de los vectores

	//variables de renderizado
	GLuint VAO;// Vertex Array Object*/
	//GLuint VBO;//|
	//GLuint EBO;//|Vertex Buffer Object

	//char *nombre;
	//----funciones privadas------//
	void inicializar();
	//--------------------------------//
	//-------------ASSIMP-------------//
	//--------------------------------//
	void loadModel(string n);
	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
};
#endif