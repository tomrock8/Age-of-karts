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

	IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)

	stringw texto; // Texto a mostrar en pantalla
	IGUIStaticText *textoUI; // Nodo de irrlicht
	

};

#endif /* ESCENAMENU_H */