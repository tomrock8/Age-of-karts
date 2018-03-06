#ifndef TMALLA_H
#define TMALLA_H

#include "libGL.hpp"
#include <iostream>
#include <list>
#include <vector>
#include "TEntidad.hpp"
#include "TRecursoMalla.hpp"
#include "TRecursoTextura.hpp"

using namespace std;
//declarado fuera de la clase para poder acceder desde TRecursoMalla

class TMalla : public TEntidad
{

public:

	//constructor
	TMalla(TRecursoMalla * malla, TRecursoTextura * text);

	//--Sobrecarga de metodos de dibujado de entidad--//
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;
	void draw(Shader *shader);
	
	TRecursoMalla *getMalla();

private:

	TRecursoMalla *malla; //Puntero al recurso malla ( se recogera en el gestor de recursos)
	TRecursoTextura *texture;
};
#endif