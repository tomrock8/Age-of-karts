#include "TMotor.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"
#include "TGestorImagenes.hpp"
#include "Corredor.hpp"


//Callback para el redimensionamiento de la pantalla
void resize_callback(GLFWwindow* window, int width, int height);
int main(int argc, char* argv[]) {

	TMotor::instancia();
	//GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::CARRERA);
	GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::MENU);

	//Creamos el gestor de imagenes para el HUD
	TGestorImagenes::getInstacia();

	obj3D *cam = TMotor::instancia().newCameraNode("camara_libre", "escena_raiz");
	cam->translate(glm::vec3(0.0f, 4.0f, 15.0f));
	
	//Evento de glfw para redimensionar la pantalla
	glfwSetFramebufferSizeCallback(TMotor::instancia().getVentana(), resize_callback);

	TMotor::instancia().setActiveCamera(TMotor::instancia().getNode("camara_libre"));
	obj3D *activeCamera = TMotor::instancia().getObjActiveCamera();

    // -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	int tipo = 0;
	while (tipoActual != Escena::tipo_escena::SALIR) {
		if (strcmp(activeCamera->getName() , TMotor::instancia().getActiveCamera()->getName())) {
			 activeCamera = TMotor::instancia().getObjActiveCamera();
		}

		tipo = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
		
		TMotor::instancia().draw(tipo);
		tipoActual = GestorEscena::instancia().update();
		
		glfwPollEvents(); // Comprobacion de eventos (Teclado, raton, joystick)
	}
	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena());
	TMotor::instancia().close();

	cout << "FIN DE JUEGO\n";
	return 0;
}

//Manejador para el evento de redimensionamiento de la pantalla
void resize_callback(GLFWwindow* ventana, int ancho, int alto) {
	TMotor::instancia().resizeScreen(ancho,alto);
	//Se redimensiona el viewport de Opengl en funcion de las medidas de la ventana
	glViewport(0, 0, ancho, alto);
}
