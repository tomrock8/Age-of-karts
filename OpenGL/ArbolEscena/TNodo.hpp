#ifndef TNODO_H
#define TNODO_H



#include <iostream>
#include <fstream>
#include <vector>
#include "TEntidad.hpp"

using namespace std;
template<class T,class Allocator = std::allocator<T>> class vector;

class TNodo
{
public:
	//-----CONSTRUCTOR----//
	TNodo();
	//-----DESTRUCTOR-----//
	~TNodo();
	//--GESTION DE HIJOS--//
	int addHijo(TNodo *n);
	int remHijo(TNodo *n);
	//--------------------//
	//---METODOS GET------//
	//--------------------//
	TEntidad *getEntidad();
	TNodo    *getPadre();
	//--------------------//
	//---METODOS SET------//
	//--------------------//
	bool setEntidad(TEntidad *n);
	void setPadre(TNodo *p);
	//inicializar motor// no va a qui pero para probar es lo que hay
	void inicializarMotor();
	//--Dibujar la entidad asociada al nodo--//
	void draw();
private:
	TEntidad * entidad;
	std::vector<TNodo *> hijos;
	TNodo *padre;

};
#endif