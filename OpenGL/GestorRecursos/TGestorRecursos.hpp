#ifndef TGESTORRECURSOS_H
#define TGESTORRECURSOS_H
#include "TRecursoMalla.hpp"
#include "TRecursoTextura.hpp"
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

class TGestorRecursos
{
public:
	//-----constructor----//
	TGestorRecursos();
	//-----destructor-----//	
	~TGestorRecursos();
	//Malla
	//TRecursoMalla *getRecurso(string nombre);
	TRecursoMalla *getRecurso(const char *nombre);
	TRecursoTextura *getRecursoTextura(const char *nombre);

private:
	std::vector<TRecurso *> recursoMallas;
	std::vector<TRecurso *> recursoTexturas;
};
#endif