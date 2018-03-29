#ifndef TMALLA_H
#define TMALLA_H

#include "libGL.hpp"
#include <iostream>
#include <list>
#include <vector>
#include "TEntidad.hpp"
#include "mesh.hpp"

using namespace std;

class TMalla : public TEntidad
{
public:
	TMalla(std::vector<mesh *> m);

	// METODOS GET
//	TRecursoMalla *getMalla();
//	TRecursoMaterial *getMaterial();
//	TRecursoTextura *getTextures();

	// DIBUJADO
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;
	void draw(Shader *shader);

private:
	std::vector<mesh *> malla;
};
#endif