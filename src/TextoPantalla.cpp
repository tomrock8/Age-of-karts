#include "TextoPantalla.hpp"

TextoPantalla *TextoPantalla::instancia = NULL;

TextoPantalla::TextoPantalla() {
	Motor3d *m = Motor3d::getInstancia();
	// -----------------------------//
	// -----------INTERFAZ----------//
	// -----------------------------//
	texto = L"Datos del jugador:\n"; // PARA MODIFICACIONES FUTURAS
	fuente = m->getGUI()->getFont("assets/fuente.bmp");
	textoUI = m->getGUI()->addStaticText(
		texto.c_str(),				 // Texto
		rect<s32>(10, 10, 400, 300), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,							 // Id del elemento
		true);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);
}

TextoPantalla *TextoPantalla::getInstancia() {
	if (instancia == NULL)
		instancia = new TextoPantalla();

	return instancia;
}

void TextoPantalla::limpiar() {
	texto = "";
	textoUI->setText(texto.c_str());
}

void TextoPantalla::agregar(std::string texto) {
	this->texto += texto.c_str();
	textoUI->setText(this->texto.c_str());
}
TextoPantalla::~TextoPantalla() {
	delete instancia;
}