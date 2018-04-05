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

	obj3D *cam = TMotor::instancia().newCameraNode("camara_libre", "escena_raiz");
	cam->translate(glm::vec3(0.0f, 0.0f, 10.0f));


	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	int tipo=0;
	while (tipoActual != Escena::tipo_escena::SALIR ) {

		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W) == GLFW_PRESS) {
			cam->translate(glm::vec3(0, 0, -.5f));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_S) == GLFW_PRESS) {
			cam->translate(glm::vec3(0, 0, .5f));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_A) == GLFW_PRESS) {
			cam->translate(glm::vec3(-.5f, 0, 0));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_D) == GLFW_PRESS) {
			cam->translate(glm::vec3(.5f, 0, 0));
		}

		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS) {
			cam->rotate(glm::vec3(1, 0, 0), .5f);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			cam->rotate(glm::vec3(1, 0, 0), -.5f);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
			cam->rotate(glm::vec3(0, 1, 0), -1);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS) {
			cam->rotate(glm::vec3(0, 1, 0), 1);
		}
		tipo=GestorEscena::instancia().getEscenaActiva().getTipoEscena();
		TMotor::instancia().draw(tipo);
		tipoActual = GestorEscena::instancia().update();

		glfwPollEvents(); // Comprobacion de eventos (Teclado, raton, joystick)

	}
	
	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena());
	TMotor::instancia().close();


	return 0;
}
