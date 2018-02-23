#ifndef ESCENAMENU_H
#define ESCENAMENU_H

#include "Escena.hpp"

#include "Motor3d.hpp"
#include "Graphics.hpp"

class EscenaMenu : public Escena {
public:
	EscenaMenu();
	~EscenaMenu();

	void init();
	void dibujar();
	void limpiar();
	void update();
	Escena::tipo_escena comprobarInputs();

	// METODOS GET

	// METODOS SET

private:

	IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)

	bool pressed;

	stringw texto; // Texto a mostrar en pantalla
	IGUIStaticText *textoUI; // Nodo de irrlicht
	ITexture* logoAOK; // Textura del logo del juego

};

#endif /* ESCENAMENU_H */