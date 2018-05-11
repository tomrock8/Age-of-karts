#include "TMotor.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"
#include "TGestorImagenes.hpp"
#include "Corredor.hpp"


//Callback para el redimensionamiento de la pantalla
void resize_callback(GLFWwindow* window, int width, int height);

//Main del juego
int main(int argc, char* argv[]) {

	//Se inicializa el motor
	TMotor::instancia();

	//Parametros del motor
	TMotor::instancia().setShaderActive("shaderCartoon"); //Tipo de shader a usar
	TMotor::instancia().setSkyBox(); //Queremos skybox

	//Empezamos en el menu del juego
	GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::MENU);
	
	//Evento de glfw para redimensionar la pantalla
	glfwSetFramebufferSizeCallback(TMotor::instancia().getVentana(), resize_callback);

	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	//Escena actual = MENU
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	//Mientras la escena sea distinta a SALIR, no se sale del loop del juego
	while (tipoActual != Escena::tipo_escena::SALIR) {
		//Se hace el update de cada escena, que incluye el dibujado de la misma
		tipoActual = GestorEscena::instancia().update();
		//Se llama a la comprobacion de eventos del motor
		TMotor::instancia().getInputs();
	}
	//Si el usuario quiere salir del juego...
	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena()); //Se borra la ultima escena
	TMotor::instancia().close(); //Se cierra correctamente el motor
	return 0; //Se sale del programa
}

//Manejador para el evento de redimensionamiento de la pantalla
void resize_callback(GLFWwindow* ventana, int ancho, int alto) {
	TMotor::instancia().resizeScreen(ancho,alto);
	//Se redimensiona el viewport de Opengl en funcion de las medidas de la ventana
	glViewport(0, 0, ancho, alto);
}
