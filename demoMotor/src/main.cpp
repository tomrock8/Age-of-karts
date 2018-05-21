#include "TMotor.hpp"

//Callback para el redimensionamiento de la pantalla
void resize_callback(GLFWwindow* window, int width, int height);

//Main del juego
int main(int argc, char* argv[]) {

	//Se inicializa el motor
	TMotor::instancia();

	TMotor::instancia().swapBuffers();
}

//Manejador para el evento de redimensionamiento de la pantalla
void resize_callback(GLFWwindow* ventana, int ancho, int alto) {
	TMotor::instancia().resizeScreen(ancho,alto);
	//Se redimensiona el viewport de Opengl en funcion de las medidas de la ventana
	glViewport(0, 0, ancho, alto);
}
