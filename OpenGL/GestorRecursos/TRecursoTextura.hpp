#ifndef TRECURSOTEXTURA_H
#define TRECURSOTEXTURA_H


#include "libGL.hpp"
#include <iostream>
#include <vector>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "TRecurso.hpp"
#include "Shader/Shader.hpp"

#include <map>



using namespace std;

class TRecursoTextura : public TRecurso
{
public:

	TRecursoTextura(const char* path);
	//ojo al string que es posiblequ se tenga qe cambiar a const char 
	~TRecursoTextura();
	//--------------------------------//
	//-------HERENCIA DE RECURSOS-----//
	//--------------------------------//
	virtual const char *getNombre() override;
	virtual void setNombre(const char *s) override;
	//--------------------------------//
	//-------------ASSIMP-------------//
	//--------------------------------//
	void loadModel(string n);
	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
	void loadMaterial(const aiScene *scene, const aiMesh *mesh, aiTextureType textureType, GLuint &textName);
	GLuint textureFromFile(const std::string & file);
	void activeTexture(GLuint program);
	void disableTexture();
	//std::vector<texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	//
	//GLuint textureFromFile(const char *path);x

protected:
	//----------------------------------------------------------//
	//--------------------VARIABLES DE SUPERFICIE---------------//
	//----------------------------------------------------------//
	float shiny, hardShiny;
	float color_diffuse[4] = { 1, 1, 1, 1 };//inicializado a 1
	GLuint diffuseTexture = 0;
	map<string, GLuint> textures;
	std::string dir;
	//std::vector<texture> normalMaps;
};
#endif