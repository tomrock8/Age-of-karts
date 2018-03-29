#ifndef TRECURSOMATERIAL_H
#define TRECURSOMATERIAL_H


#include "libGL.hpp"
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "TRecurso.hpp"
#include "Shader.hpp"
#include <map>

using namespace std;

class TRecursoMaterial : public TRecurso
{
public:

	TRecursoMaterial(const aiMaterial *mat);
	~TRecursoMaterial();

	void activeMaterial(Shader *shader);

	// METODOS GET
	virtual const char *getNombre() override;
	float getShiny();
	float getHardShiny();

	// METODOS SET
	virtual void setNombre(const char *s) override;
	void setColorAmbient(float r, float g, float b, float a) ;
	void setColorDiffuse(float r, float g, float b, float a) ;
	void setColorSpecular(float r, float g, float b, float a) ;
	void setShiny(float n);
	void setHardShiny(float n);

private:
	float shiny, hardShiny;
	aiColor4D ambient;
	aiColor4D diffuse;
	aiColor4D specular;
	aiString name;
	float color_ambient[4] = { 1 };
	float color_diffuse[4] = { 1 };
	float color_specular[4] = { 1 };



	//std::vector<texture> normalMaps;
};
#endif