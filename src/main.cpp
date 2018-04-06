#include "TMotor.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"

#include "Corredor.hpp"


//Callback para el redimensionamiento de la pantalla
void resize_callback(GLFWwindow* window, int width, int height);
void character_callback(GLFWwindow *window, unsigned int codepoint);

int main(int argc, char* argv[]) {

	TMotor::instancia();
	//GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::CARRERA);
	GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::MENU);

	obj3D *cam = TMotor::instancia().newCameraNode("camara_libre", "escena_raiz");
	cam->translate(glm::vec3(0.0f, 0.0f, 10.0f));

	//Evento de glfw para redimensionar la pantalla
	glfwSetFramebufferSizeCallback(TMotor::instancia().getVentana(), resize_callback);




	// OBJETOS CARGADOS
	//obj3D *mapa = TMotor::instancia().newMeshNode("mapa", "assets/Mapa01/isla.obj", "escena_raiz");
	//obj3D *cubnot = TMotor::instancia().newMeshNode("cubote", "assets/wall/wall.obj", "escena_raiz");
	//obj3D *coche = TMotor::instancia().newMeshNode("coche", "assets/coche.obj", "escena_raiz");





	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	int tipo = 0;
	while (tipoActual != Escena::tipo_escena::SALIR) {
		glfwSetCharCallback(TMotor::instancia().getVentana(), character_callback);

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
void character_callback(GLFWwindow *window, unsigned int codepoint)	//metodo para obtener la tecla pulsada y mostrarla por pantalla 
{																		//(usada para introducir datos como la ip)
	if (codepoint >= 48 && codepoint <= 57) {
		std::cout << codepoint - 48 << std::endl;
	}
}

//Manejador para el evento de redimensionamiento de la pantalla
void resize_callback(GLFWwindow* ventana, int ancho, int alto)
{
	//Se redimensiona el viewport de Opengl en funcion de las medidas de la ventana
	glViewport(0, 0, ancho, alto);
}
