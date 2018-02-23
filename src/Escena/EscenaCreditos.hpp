#ifndef ESCENACREDITOS_H
#define ESCENACREDITOS_H

#include "Escena.hpp"

#include "Motor3d.hpp"
#include "Graphics.hpp"

class EscenaCreditos : public Escena {
public:
	EscenaCreditos();
	~EscenaCreditos();

	void init();
	void dibujar();
	void limpiar();
	void update();
	Escena::tipo_escena comprobarInputs();

	// METODOS GET

	// METODOS SET

private:

	IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)

	stringw texto; // Texto a mostrar en pantalla
	IGUIStaticText *textoUI; // Nodo de irrlicht
	ITexture* logoAOK; // Textura del logo del juego

};

#endif /* ESCENACREDITOS_H */