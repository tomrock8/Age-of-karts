#ifndef TRECURSO_H
#define TRECURSO_H
#include  <iostream>
using namespace std;

class TRecurso
{

public:
	virtual const char *getNombre() = 0;
	virtual void setNombre(const char *s) = 0;
	//virtual void cargarFichero(char *n);

protected:
	const char *nombre;
};
#endif