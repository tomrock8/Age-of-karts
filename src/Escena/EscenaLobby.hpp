#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H

#include "Escena.hpp"

#include "Motor3d.hpp"

class EscenaLobby : public Escena {
public:
	EscenaLobby();
	~EscenaLobby();

	void init();
	void dibujar();
	void limpiar();
	void update();
	Escena::tipo_escena comprobarInputs();


	// METODOS GET

	// METODOS SET

private:
	int nElementos; // Numeros para la IP (los ultimos 3)


	IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)

	stringw texto; // Texto a mostrar en pantalla
	IGUIStaticText *textoUI; // Nodo de irrlicht
	ITexture* logoAOK; // Textura del logo del juego

};

#endif /* ESCENALOBBY_H */