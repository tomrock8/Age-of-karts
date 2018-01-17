#ifndef TEXTOPANTALLA_H
#define TEXTOPANTALLA_H

#include <iostream>
#include <string>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"

class TextoPantalla
{
public:
	void limpiar();
	void agregar(std::string texto);
	//destructor
	~TextoPantalla();
	static TextoPantalla *getInstancia();

private:
	static TextoPantalla *instancia;
	stringw texto;
	IGUIStaticText *textoUI;
	IGUIFont *fuente;

	TextoPantalla();
};

#endif /* TEXTOPANTALLA_H */