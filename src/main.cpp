#include "Motor3d.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"

#include "Corredor.hpp"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(int argc, char* argv[]) {
	EscenaJuego *juego = new EscenaJuego(Escena::tipo_escena::CARRERA);
	juego->init();

	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Motor3d *m = Motor3d::getInstancia();

	while (m->getDevice()->run()) {
		juego->update();
	}

	delete juego;

	GestorEscena::instancia().getEscena(Escena::tipo_escena::MENU);


	delete Motor3d::getInstancia();
	return 0;
}