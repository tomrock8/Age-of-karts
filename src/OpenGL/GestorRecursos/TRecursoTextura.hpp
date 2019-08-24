#ifndef TRECURSOTEXTURA_H
#define TRECURSOTEXTURA_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "libGL.hpp"
#include <iostream>
#include <vector>
#include <map>
#include "TRecurso.hpp"

class Shader;

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