#include "TMotor.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"

#include "Corredor.hpp"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(int argc, char* argv[]) {
	
	TMotor::instancia();
	//GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::CARRERA);
	GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::MENU);

	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	int tipo=0;
	while (tipoActual != Escena::tipo_escena::SALIR) {
		tipo=GestorEscena::instancia().getEscenaActiva().getTipoEscena();
		TMotor::instancia().draw(tipo);
		tipoActual = GestorEscena::instancia().update();

	}
	
	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena());
	TMotor::instancia().close();


	return 0;
}
