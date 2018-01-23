#ifndef ESCENAJUEGO_H
#define ESCENAJUEGO_H

#include "Escena.hpp"

class EscenaJuego : public Escena {
public:
	EscenaJuego(tipo_escena tipo);
	~EscenaJuego();

	void init();
	void dibujar();
	void limpiar();
	void update();

	// METODOS GET


	// METODOS SET

private:


};

#endif /* ESCENAJUEGO_H */