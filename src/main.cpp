#include "Motor3d.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"

#include "Corredor.hpp"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(int argc, char* argv[]) {
	Motor3d m = Motor3d::instancia();
	GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::CARRERA);

	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	while (m.getDevice()->run()) {
		GestorEscena::instancia().update();
	}

	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena());

	cout << "\n\ncaca";

	return 0;
}
