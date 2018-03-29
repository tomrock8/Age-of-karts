//la malla lo primero que se llame puesto que al tener GLEW, requiere que este en primera linea( 5 horas para verlo)
#define GLEW_STATIC
#include "libGL.hpp"
#include "Motor/TMotor.hpp"
#include "TEntidad.hpp"
#include "TNodo.hpp"
#include "TTransform.hpp"
#include "TLuz.hpp"
#include "TCamara.hpp"
#include "obj3D.hpp"
#include <iostream>
#include <math.h>



void arbolEscena() {

	TMotor::instancia().newCameraNode(glm::vec3(0.0f, 0.0f, 0.0f), "camara_libre");
	//Pasarles nombre a partir de light_0 para delante
	TMotor::instancia().newLightNode(glm::vec3(0.0f, 0.0f, 0.0f), "light_0", glm::vec4(0.0f, 0.f, -1.0f, 1.0f), 0.25f, glm::cos(glm::radians(30.0f)));
	TMotor::instancia().newLightNode(glm::vec3(0.0f, 0.0f, -45.0f), "light_1", glm::vec4(.0f, 0.f, 0.f, 0.0f), 0.01f, 0.0f);
	TMotor::instancia().newLightNode(glm::vec3(0.0f, 0.0f, 5.0f), "light_2", glm::vec4(.0f, 0.f, 0.f, 0.0f), 0.01f, 0.0f);
	
	
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL PRIMER OBJETO                                  |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	TMotor::instancia().newMeshNode(glm::vec3(0.0f, -0.0f, -6.0f), "cubo", "assets/wall/wall.obj");
	
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL SEGUNDO OBJETO                                 |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	obj3D *caja_1 = TMotor::instancia().newMeshNode(glm::vec3(2.0f, -1.0f, -5.0f), "caja_1", "assets/caja_0.obj");
	
	/*
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL TERCER OBJETO                                  |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	obj3D *barco = TMotor::instancia().newMeshNode(glm::vec3(0.0f, 2.0f, -50.0f), "barco", "assets/BARCO.obj");
	*/
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	cout << "|                                   CREANDO EL CUARTO OBJETO                                 |" << endl;
	cout << "|---------------------------------------------------------------------------------------------|" << endl;
	obj3D * caja_2 = TMotor::instancia().newMeshNode(glm::vec3(-2.0f, 1.0f, -5.0f), "caja_2", "assets/caja_mutante.obj");
	
}

int main(void) {
	TMotor::instancia();

	std::cout << glGetString(GL_VERSION) << endl;
	//----------------------------------------------------------------------//
	//-------------------------------TIMER----------------------------------//
	//----------------------------------------------------------------------//
	double sec = 0.0;									//glfwSetTime para empezar donde se quiera
	//Shader Basico con materiales, texturas y luces
	Shader *shader = new Shader("assets/shaders/shaderTextura/vertexShader.txt", "assets/shaders/shaderTextura/fragmentShader.txt");

	arbolEscena();
	obj3D *caja = TMotor::instancia().newMeshNode(glm::vec3(2.0f, 0.0f, -5.0f), "escudo2", "assets/caja_0.obj");
	GLfloat giro = 1;
	obj3D *caja2 = TMotor::instancia().newMeshNode(glm::vec3(-2.0f, 0.0f, -5.0f), "escudo2", "assets/caja_0.obj");


	TNodo *cubo = TMotor::instancia().getNode("cubo");
	

	//----------------------------------------------------------------------//
	//--------------------------BUCLE DEL JUEGO-----------------------------//
	//----------------------------------------------------------------------//
	while (!glfwWindowShouldClose(TMotor::instancia().getVentana())) {


		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				//Limpia el buffer del color
		shader->use();

		TMotor::instancia().draw(shader);
		caja->rotate(glm::vec3(0, 1, 0), giro);
		caja2->rotate(glm::vec3(1,0, 0), giro);

		giro += 0.5f;
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
