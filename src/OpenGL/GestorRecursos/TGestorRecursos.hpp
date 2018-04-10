#ifndef TGESTORRECURSOS_H
#define TGESTORRECURSOS_H
#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

class TGestorRecursos
{
public:
	TGestorRecursos();
	~TGestorRecursos();


	//-----------ASSIMP-----------//
	std::vector<mesh*> loadMesh(const char *nombre);
	void processNode(aiNode *node, const aiScene *scene);
	
	
	// ASSIMP:MALLA
	TRecursoMalla *processMesh(aiMesh *mesh, const aiScene *scene, const char* nombre);
	// ASSIMP:MATERIAL
	TRecursoMaterial *getRecursoMaterial(const char *nombre, const aiMaterial * mat);
	TRecursoMaterial *processMaterial(aiMesh *mesh, const aiScene *scene); 
	// ASSIMP:TEXTURA
	TRecursoTextura *processTextures(aiMesh * mesh, const aiScene * scene);
	
	void loadTexture(const aiScene *scene, const aiMesh *mesh, aiTextureType textureType, GLuint &textName, TRecursoTextura *recText);
	GLuint textureFromFile(const std::string & file);
	
	// METODOS GET
	TRecursoTextura *getRecursoTextura(const char * nombre, aiMesh *mesh, const aiScene *scene);
	TRecursoMalla *getRecursoMalla(const char *nombre, aiMesh *mesh, const aiScene *scene);
	TRecursoMaterial *getRecursoMaterial(const char *nombre);

	std::vector<TRecurso *> getRecursoMallas();

private:
	
	std::vector<mesh *> objMeshes;
	std::vector<TRecurso *> recursoMallas;
	std::vector<TRecurso *> recursoMaterials;
	std::vector<TRecurso *> recursoTexturas;

	std::string *dir;
	std::map<std::string, GLuint> textures;
};
#endif