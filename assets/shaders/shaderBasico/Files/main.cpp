//la malla lo primero que se llame puesto que al tener GLEW, requiere que este en primera linea( 5 horas para verlo)
#define GLEW_STATIC
#include "libGL.hpp"
#include "Motor/TMotor.hpp"
#include "TEntidad.hpp"
#include "TNodo.hpp"
#include "TTransform.hpp"
#include "TLuz.hpp"
#include "TCamara.hpp"
#include <iostream>
#include <math.h>



void arbolEscena() {

	TMotor::instancia().newCameraNode(glm::vec3(0.0f, 0.0f, 0.0f), "camara_libre");
	TMotor::instancia().newLightNode(glm::vec3(70.0f, -50.0f, 50.0f), "luz1");
	
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL PRIMER OBJETO                                  |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	TMotor::instancia().newMeshNode(glm::vec3(0.0f, -0.0f, -6.0f), "cubo", "assets/BARCO.obj");
	
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL SEGUNDO OBJETO                                 |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	TMotor::instancia().newMeshNode(glm::vec3(2.0f, -1.0f, -5.0f), "escudo2", "assets/caja_0.obj");
	
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL TERCER OBJETO                                  |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	TMotor::instancia().newMeshNode(glm::vec3(0.0f, 2.0f, -7.0f), "cubo2", "assets/escudo_0_caracono.obj");
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL CUARTO OBJETO                                 |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	TMotor::instancia().newMeshNode(glm::vec3(-2.0f, 1.0f, -5.0f), "escudo2", "assets/caja_mutante.obj");
	
}

int main(void) {
	TMotor::instancia();
	//----------------------------------------------------------------------//
	//-------------------------------TIMER----------------------------------//
	//----------------------------------------------------------------------//
	double sec = 0.0;									//glfwSetTime para empezar donde se quiera
	//Shader Basico con materiales y luces
	Shader *shader = new Shader("assets/shaders/shaderBasico/vertexShader.txt", "assets/shaders/shaderBasico/fragmentShader.txt");

	arbolEscena();



	TNodo *cubo = TMotor::instancia().getNode("cubo");
	

	//----------------------------------------------------------------------//
	//--------------------------BUCLE DEL JUEGO-----------------------------//
	//----------------------------------------------------------------------//
	while (!glfwWindowShouldClose(TMotor::instancia().getVentana())) {


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				//Limpia el buffer del color
		shader->use();

		TMotor::instancia().draw(shader);

		//--------------------------------//
		//------R E N D E R I Z A D O-----//
		//--------------------------------//
		//sec = glfwGetTime();
		//std::cout << sec << std::endl;

		//--interpolacion--//
		glfwSwapBuffers(TMotor::instancia().getVentana());						//swap los bufers de pantalla (trasero y delantero)
		glfwPollEvents();						    	//comprobamos si hay algun evento que se haya activado (raton, teclado, joystick)

	}


	glfwTerminate();									//Terminar GLFW, limpiando todos los recursos alocados por GLFW

	return 0;




}
