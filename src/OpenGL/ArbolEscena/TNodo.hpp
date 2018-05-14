#ifndef TNODO_H
#define TNODO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "TEntidad.hpp"

using namespace std;
template<class T, class Allocator = std::allocator<T>> class vector;

class TNodo {
public:
	TNodo(const char* n);
	~TNodo();
	void draw(Shader *shader);

	// GESTION DE HIJOS 
	int addHijo(TNodo *n);
	int remHijo(TNodo *n);

	// METODOS GET
	TEntidad *getEntidad();
	TNodo *getPadre();
	TNodo *getNode(const char * nombre);
	const char *getName();
	std::vector<TNodo *> getHijos();

	// METODOS SET
	bool setEntidad(TEntidad *n);
	void setPadre(TNodo *p);
	void setName(const char* n);

private:
	TEntidad * entidad; // Entidad del nodo
	std::vector<TNodo *> hijos; // Vector que contiene los hijos que tiene el nodo
	TNodo *padre; // Nodo padre
	std::string *name; // Nombre del nodo
};
#endif