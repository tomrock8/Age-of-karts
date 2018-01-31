#include "Motor3d.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"

#include "Corredor.hpp"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(int argc, char* argv[]) {
	Motor3d m = Motor3d::instancia();
	//GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::CARRERA);
	GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::MENU);

	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	bool finJuego = false;
	while (m.getDevice()->run() && !finJuego) {
		finJuego = GestorEscena::instancia().update();

	}
	
	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena());
	Motor3d::instancia().cerrar();

	cout << "\n\ncaca";

	return 0;
}
