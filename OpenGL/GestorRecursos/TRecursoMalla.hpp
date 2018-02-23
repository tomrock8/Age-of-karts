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
struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 textCoord;
};
struct texture {
	GLuint id;
	string type; //difusa, especular
};
class TRecursoMalla :public TRecurso
{
	
public:
	//---constructor---//
	TRecursoMalla(string path);
	//---destructor---//
	~TRecursoMalla();
	//--------------------------------//
	//-------------ASSIMP-------------//
	//--------------------------------//
	void loadModel(string n);
	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
	//--------------------------------//
	//-------HERENCIA DE RECURSOS-----//
	//--------------------------------//
	virtual const char *getNombre() override;
	virtual void setNombre(const char *s) override;
	//-------------------------------//
	//----------METODOS DE GET-------//
	//-------------------------------//
	vector<vertex> *getVertices();
	vector<GLuint> *getIndices();

	//-------------------------------//
	//----------DIBUJADO-------------//
	//-------------------------------//
	void draw(Shader shader);


private:
	std::vector<vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<texture> textures;

	
	long nTriangle;
	//char *nombre;
	//----funciones privadas------//
};
#endif

