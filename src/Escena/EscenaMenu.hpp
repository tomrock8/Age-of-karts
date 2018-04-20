#ifndef ESCENAMENU_H
#define ESCENAMENU_H

#include "Escena.hpp"

class EscenaMenu : public Escena {
public:
	EscenaMenu();
	~EscenaMenu();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();

	// METODOS GET
	std::string getIpConexion();

	// METODOS SET

private:

	//IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)
	bool pressed = false;
	bool end;
	//stringw texto; // Texto a mostrar en pantalla
	//IGUIStaticText *textoUI; // Nodo de irrlicht
	//ITexture* logoAOK; // Textura del logo del juego
	std::string ipConexion;

	int optionMenu;

	int sonidoOpcion;
	AlSource *fuenteOpcion;
	AlSource *fuenteMenu;

	void initHud();
};

#endif /* ESCENAMENU_H */