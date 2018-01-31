#ifndef ESCENAMENU_H
#define ESCENAMENU_H

#include "Escena.hpp"

#include "Motor3d.hpp"

class EscenaMenu : public Escena {
public:
	EscenaMenu(tipo_escena tipo);
	~EscenaMenu();

	void init();
	void dibujar();
	void limpiar();
	void update();
	bool comprobarInputs();

	// METODOS GET

	// METODOS SET

private:


};

#endif /* ESCENAMENU_H */