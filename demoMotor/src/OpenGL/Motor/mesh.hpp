#pragma once
#ifndef MESH
#define MESH

#include "libGL.hpp" 
#include "TRecursoMalla.hpp" 
#include "TRecursoMaterial.hpp" 
#include "TRecursoTextura.hpp" 

using namespace std;

class mesh
{
public:
	mesh();
	~mesh();

	void setMesh(TRecursoMalla *m);
	void setMat(TRecursoMaterial *mat);
	void setTex(TRecursoTextura *t);

	TRecursoMalla * getMesh();
	TRecursoMaterial * getMat();
	TRecursoTextura * getText();
	

private:
	TRecursoMalla * meshe;
	TRecursoMaterial *mat;
	TRecursoTextura *text;
};
#endif