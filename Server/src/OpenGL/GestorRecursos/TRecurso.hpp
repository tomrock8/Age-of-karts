#ifndef TRECURSO_H
#define TRECURSO_H
#include  <iostream>

class TRecurso
{

public:
	virtual ~TRecurso() {};

	virtual const char *getNombre() = 0;
	virtual void setNombre(const char *s) = 0;
	//virtual void cargarFichero(char *n);

protected:
	std::string *nombre;
};
#endif