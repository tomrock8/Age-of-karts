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

	//CREAMOS GESTOR DE SONIDO SINGLETON
	GestorSonido::getInstacia();

	obj3D *cam = TMotor::instancia().newCameraNode("camara_libre", "escena_raiz");
	cam->translate(glm::vec3(0.0f, 12.0f, 15.0f));
	
	//Evento de glfw para redimensionar la pantalla
	glfwSetFramebufferSizeCallback(TMotor::instancia().getVentana(), resize_callback);




	// OBJETOS CARGADOS
	//TMotor::instancia().precarga("assets/Mapa01/isla.obj");
	
    obj3D *pirata = TMotor::instancia().newMeshNode("pirata", "assets/Pirata/p2.obj", "escena_raiz");
	//obj3D *chino = TMotor::instancia().newMeshNode("chino", "assets/Chino/Chino.obj", "escena_raiz");
	obj3D *wall = TMotor::instancia().newMeshNode("wall", "assets/wall/wall.obj", "escena_raiz");
	obj3D *cube = TMotor::instancia().newMeshNode("cubo", "assets/wall/wall.obj", "escena_raiz");
	obj3D *cube2 = TMotor::instancia().newMeshNode("cubo2", "assets/wall/wall.obj", "escena_raiz");
	obj3D *cube3 = TMotor::instancia().newMeshNode("cubo3", "assets/wall/wall.obj", "escena_raiz");
	obj3D *cube4 = TMotor::instancia().newMeshNode("cubo4", "assets/wall/wall.obj", "escena_raiz");
	//chino->setPosition(5.5, 0, 0);
	pirata->setPosition(-20, 10, 0);
	wall->setScale(5.0, 1.0, 5.0);
	wall->setPosition(0, 4.0, 0);
	cube->setPosition(0, 10.0, 0);
	cube2->setScale(0.5, 0.5, 0.5);
	cube2->setPosition(-1.75, 8.0, 0);
	cube3->setPosition(0, 2.0, 60);
	cube4->setPosition(0, 0.5, 100);
	
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
	
	
	//obj3D *cubnot = TMotor::instancia().newMeshNode("cubote", "assets/wall/wall.obj", "escena_raiz");
	//cubnot->setPosition(1.0f, 5.0f, 7.0f);
	//obj3D *cubsuelo = TMotor::instancia().newMeshNode("cubote", "assets/wall/wall.obj", "escena_raiz");

	
	//glm::vec3 escala = cubnot->getEscala();
	//std::cout << escala.x << " - " << escala.y << " - " << escala.z << endl;
	//obj3D *coche = TMotor::instancia().newMeshNode("coche", "assets/coche.obj", "escena_raiz");


	TMotor::instancia().setActiveCamera(TMotor::instancia().getNode("camara_libre"));
	obj3D *activeCamera = TMotor::instancia().getObjActiveCamera();

	//obj3D *activeCamera = NULL;
	//activeCamera = TMotor::instancia().newCameraNode(TMotor::instancia().getActiveCamera()->getName(), TMotor::instancia().getActiveCamera()->getPadre()->getName());
	// -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	int tipo = 0;
	while (tipoActual != Escena::tipo_escena::SALIR) {
		if (strcmp(activeCamera->getName() , TMotor::instancia().getActiveCamera()->getName())) {
			if (activeCamera->getName() == "camara_libre") activeCamera = TMotor::instancia().getObjActiveCamera();
		}
		glfwSetCharCallback(TMotor::instancia().getVentana(), character_callback);
		//activeCamera = TMotor::instancia().newCameraNode(TMotor::instancia().getActiveCamera()->getName(), TMotor::instancia().getActiveCamera()->getPadre()->getName());
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_I) == GLFW_PRESS) {
			if(activeCamera->getName()=="camara_libre") activeCamera->translate(glm::vec3(0, 0, -.4f));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_K) == GLFW_PRESS) {
			if (activeCamera->getName() == "camara_libre")activeCamera->translate(glm::vec3(0, 0, .4f));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_J) == GLFW_PRESS) {
			if (activeCamera->getName() == "camara_libre")activeCamera->translate(glm::vec3(-.4f, 0, 0));
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_L) == GLFW_PRESS) {
			if (activeCamera->getName() == "camara_libre")activeCamera->translate(glm::vec3(.4f, 0, 0));
		}

		
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS) {
			if (activeCamera->getName() == "camara_libre") activeCamera->rotate(glm::vec3(1, 0, 0), .5f);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (activeCamera->getName() == "camara_libre") activeCamera->rotate(glm::vec3(1, 0, 0), -.5f);
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
			if (activeCamera->getName() == "camara_libre")activeCamera->rotate(glm::vec3(0, 1, 0), -1);
			cout << "estoy rotando bien la camara : " << activeCamera->getName() << endl;
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS) {
			if (activeCamera->getName() == "camara_libre") activeCamera->rotate(glm::vec3(0, 1, 0), 1);
		}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_C) == GLFW_PRESS) {
			TMotor::instancia().setActiveCamera(TMotor::instancia().getNode("camara_libre"));
		}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_X) == GLFW_PRESS) {
			TMotor::instancia().setActiveCamera(TMotor::instancia().getNode("camara_jugador3apersona"));
		}
		tipo = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
		
		TMotor::instancia().draw(tipo);
		tipoActual = GestorEscena::instancia().update();
		
//		activeCamera = TMotor::instancia().newCameraNode(TMotor::instancia().getActiveCamera()->getName(), TMotor::instancia().getActiveCamera()->getPadre()->getName());
		glfwPollEvents(); // Comprobacion de eventos (Teclado, raton, joystick)
	}
	delete GestorSonido::getInstacia();
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
