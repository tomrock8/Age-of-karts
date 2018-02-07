#pragma once

#include  <GLFW/glfw3.h>
#include  <iostream>
#include <list>
#include "TEntidad.hpp"

using namespace std;

class TMalla : public TEntidad
{
public:
	void cargaMalla(string fich);
	//void inicializarFisicas();

	//--Sobrecarga de metodos de dibujado de entidad--//
	void beginDraw();
	void endDraw();
private:
	//TRecursoMalla *malla //Puntero al recurso malla ( se recogera en el gestor de recursos)
	
};
