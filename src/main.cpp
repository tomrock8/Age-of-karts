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

	//obj3D *cam = TMotor::instancia().newCameraNode("camara_libre", "escena_raiz");
	//cam->translate(glm::vec3(0.0f, 0.0f, 15.0f));
	
	//Evento de glfw para redimensionar la pantalla
	glfwSetFramebufferSizeCallback(TMotor::instancia().getVentana(), resize_callback);




	// OBJETOS CARGADOS
	//TMotor::instancia().precarga("assets/Mapa01/isla.obj");
	
	/*obj3D *pirata = TMotor::instancia().newMeshNode("pirata", "assets/Pirata/p2.obj", "escena_raiz");
	obj3D *chino = TMotor::instancia().newMeshNode("chino", "assets/Chino/Chino.obj", "escena_raiz");
	obj3D *wall = TMotor::instancia().newMeshNode("wall", "assets/wall/wall.obj", "escena_raiz");
	chino->setPosition(5.5, 0, 0);
	pirata->setPosition(-5.5, 0, 0);
	pirata->setRotation(90.0f,0,0);
	wall->setPosition(5.5, 4, 0);*/
	
	/*
	obj3D *cubnot = TMotor::instancia().newMeshNode("cubote", "assets/wall/wall.obj", "escena_raiz");
	//cubnot->setScale(1.0f,3.0f,1.0f);
	cubnot->setPosition(1.0f, 0.0f, 7.0f);
	cubnot->setRotation(glm::vec3(0,0,1),33.0f);
	obj3D *cubsuelo = TMotor::instancia().newMeshNode("cubote", "assets/wall/wall.obj", "escena_raiz");
	//cubnot->setScale(1.0f,3.0f,1.0f);
	cubsuelo->setScale(3.0f,1.0f,3.0f);
	cubsuelo->setRotation(glm::vec3(0,0,1),15.0f);
	cubsuelo->setPosition(0.0f, -2.0f, 7.0f);*/
	
	//glm::vec3 escala = cubnot->getEscala();
	//std::cout << escala.x << " - " << escala.y << " - " << escala.z << endl;
	//obj3D *coche = TMotor::instancia().newMeshNode("coche", "assets/coche.obj", "escena_raiz");


	//obj3D *activeCamera = new obj3D(TMotor::instancia().getActiveCamera(), TMotor::instancia().getActiveCamera()->getName(),99);

	//obj3D *activeCamera = NULL;
	//activeCamera = TMotor::instancia().newCameraNode(TMotor::instancia().getActiveCamera()->getName(), TMotor::instancia().getActiveCamera()->getPadre()->getName());
	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	int tipo = 0;
	while (tipoActual != Escena::tipo_escena::SALIR) {
		/*if (strcmp(activeCamera->getName() , TMotor::instancia().getActiveCamera()->getName())) {
			activeCamera = new obj3D(TMotor::instancia().getActiveCamera(), TMotor::instancia().getActiveCamera()->getName(), 99);
		}
		glfwSetCharCallback(TMotor::instancia().getVentana(), character_callback);
		//activeCamera = TMotor::instancia().newCameraNode(TMotor::instancia().getActiveCamera()->getName(), TMotor::instancia().getActiveCamera()->getPadre()->getName());
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_I) == GLFW_PRESS) {
			activeCamera->translate(glm::vec3(0, 0, -.5f));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_K) == GLFW_PRESS) {
			activeCamera->translate(glm::vec3(0, 0, .5f));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_J) == GLFW_PRESS) {
			activeCamera->translate(glm::vec3(-.5f, 0, 0));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_L) == GLFW_PRESS) {
			activeCamera->translate(glm::vec3(.5f, 0, 0));
		}

		
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS) {
			activeCamera->rotate(glm::vec3(1, 0, 0), .5f);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			activeCamera->rotate(glm::vec3(1, 0, 0), -.5f);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
			activeCamera->rotate(glm::vec3(0, 1, 0), -1);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS) {
			activeCamera->rotate(glm::vec3(0, 1, 0), 1);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_C) == GLFW_PRESS) {
			TMotor::instancia().setActiveCamera(TMotor::instancia().getNode("camara_libre"));
		}*/
		tipo = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
		tipoActual = GestorEscena::instancia().update();
		TMotor::instancia().draw(tipo);
//		activeCamera = TMotor::instancia().newCameraNode(TMotor::instancia().getActiveCamera()->getName(), TMotor::instancia().getActiveCamera()->getPadre()->getName());
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
void resize_callback(GLFWwindow* ventana, int ancho, int alto) {
	//Se redimensiona el viewport de Opengl en funcion de las medidas de la ventana
	glViewport(0, 0, ancho, alto);
}
