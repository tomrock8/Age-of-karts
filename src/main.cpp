#include "Motor3d.hpp"
#include "EscenaJuego.hpp"

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
	return 0;
}