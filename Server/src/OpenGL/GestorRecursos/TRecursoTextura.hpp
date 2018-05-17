#ifndef TRECURSOTEXTURA_H
#define TRECURSOTEXTURA_H


#include "libGL.hpp"
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "TRecurso.hpp"
#include "Shader.hpp"
#include <stdio.h>
#include <string.h>
#include <map>



using namespace std;

class TRecursoTextura : public TRecurso
{
public:
	TRecursoTextura(const aiMaterial *mat);
	~TRecursoTextura() override;
	void activeTexture(Shader *shader);
	void disableTexture();

	// METODOS GET
	virtual const char *getNombre() override;
	GLuint getDiffuseTextureID();
	GLuint getSpecularTextureID();
	GLuint getNormalTextureID();

	// METODOS SET
	virtual void setNombre(const char *s) override;
	void setTexture(std::string s, GLuint id);


private:
	std::map<std::string, GLuint> textures;

	aiString name;
	const char* text;
	GLuint diffuseTexture = 0;
	GLuint specularTexture = 1;
	GLuint normalTexture = 2;
};
#endif