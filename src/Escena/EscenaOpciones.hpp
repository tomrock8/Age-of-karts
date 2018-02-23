#ifndef ESCENAOPCIONES_H
#define ESCENAOPCIONES_H

#include "Escena.hpp"

class EscenaOpciones : public Escena {
public:

	~EscenaOpciones();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;

	// METODOS GET

	// METODOS SET

private:


};

#endif /* ESCENAOPCIONES_H */