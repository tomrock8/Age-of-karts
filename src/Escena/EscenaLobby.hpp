#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H

#include "Escena.hpp"

#include "Motor3d.hpp"
#include "Timer.hpp"

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
	std::string getIpConexion();

	// METODOS SET

private:
	int nElementos; // Numeros para la IP (los ultimos 3)
	std::string ipConexion;

	IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)

	stringw texto; // Texto a mostrar en pantalla
	IGUIStaticText *textoUI; // Nodo de irrlicht
	ITexture* logoAOK; // Textura del logo del juego
};

#endif /* ESCENALOBBY_H */