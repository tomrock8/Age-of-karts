#ifndef ESCENAOPCIONES_H
#define ESCENAOPCIONES_H

#include "Escena.hpp"

class EscenaOpciones : public Escena {
public:

	EscenaOpciones(Escena::tipo_escena tipo);
	~EscenaOpciones();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();
	AlSource *fuenteMusica;
	AlSource *fuenteEfectos;

	// METODOS GET

	// METODOS SET

private:
	bool end;

};

#endif /* ESCENAOPCIONES_H */