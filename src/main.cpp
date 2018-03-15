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
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	while (m.getDevice()->run() && tipoActual != Escena::tipo_escena::SALIR) {
		tipoActual = GestorEscena::instancia().update();

	}
	
	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena());
	Motor3d::instancia().cerrar();


	return 0;
}
