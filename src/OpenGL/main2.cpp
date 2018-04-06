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
#include "elementoHud.hpp"
#include "Shader.hpp"
#include <iostream>
#include <math.h>

int hola(void) {
	/*
	TMotor::instancia();

	// TIMER
	double sec = 0.0;	//glfwSetTime para empezar donde se quiera

	// CAMARA
	obj3D *cam = TMotor::instancia().newCameraNode("camara_libre", "escena_raiz");
	cam->translate(glm::vec3(6.0f, 0.0f, 3.0f));
	obj3D * luzInfinita = TMotor::instancia().newLightNode("light_3", glm::vec4(0.0f, 0.f, -1.0f, 1.0f), 0.25f, glm::cos(glm::radians(30.0f)), "escena_raiz");
	luzInfinita->translate(glm::vec3(0.0f, 30.0f, 0.0f));
	// LUCES
	//TMotor::instancia().newLightNode(glm::vec3(0.0f, 30.0f, 0.0f), "light_0", glm::vec4(0.0f, 0.f, -1.0f, 1.0f), 0.25f, glm::cos(glm::radians(30.0f)), "escena_raiz");
	//obj3D *luz0 = TMotor::instancia().newLightNode("light_0", glm::vec4(.0f, 0.f, 0.f, 0.0f), 0.01f, 0.0f, "escena_raiz");
	//luz0->translate(glm::vec3(-290.0f, 100.0f, 30.61f));
	//TMotor::instancia().newLightNode(glm::vec3(0.0f, 15.0f, 0.0f), "light_1", glm::vec4(.0f, 0.f, 0.f, 0.0f), 0.01f, 0.0f, "escena_raiz");

	// OBJETOS
	//obj3D *caja2 = TMotor::instancia().newMeshNode(glm::vec3(-303.0f, 20.0f, 26.61f), "escudo2", "assets/KartPirata.obj", "escena_raiz");
	//obj3D *box = TMotor::instancia().newMeshNode(glm::vec3(0.0f, -0.0f, -0.0f), "cubo", "assets/Mapa01/palmeras.obj", "escena_raiz");
    // obj3D *caja = TMotor::instancia().newMeshNode(glm::vec3(0.0f, 0.0f, -0.0f), "escudo2", "assets/materiales.obj", "escena_raiz");

	obj3D *caja2 = TMotor::instancia().newMeshNode( "escudo2", "assets/wall/wall.obj", "escena_raiz");
	TNodo *aux = TMotor::instancia().getNode("rotacion_escudo2");
	caja2->translate(glm::vec3(0.0f, 0.0f, -6.61f));
	TMotor::instancia().newHud("HudPrueba");
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "algo", "assets/awesomeface.png");
	TMotor::instancia().newHud("Hud");
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "algo", "assets/awesomeface.png");
	TMotor::instancia().getHud("HudPrueba")->addElement(1.5f, 1.5f, "algoMas", "assets/426.png");
	TMotor::instancia().getHud("HudPrueba")->scaleElement("algo", 0.5f, 0.5f);
	TMotor::instancia().getHud("HudPrueba")->traslateElement("algo", 0.7f, 0.5f);
	TMotor::instancia().getHud("HudPrueba")->rotateElement("algo", 180.0f);
	TMotor::instancia().getHud("HudPrueba")->traslateElement("algoMas", -0.6f, -0.5f);
	TMotor::instancia().getHud("HudPrueba")->scaleElement("algoMas", 0.5f, 0.5f);
	TMotor::instancia().setActiveHud("HudPrueba");
 


	//----------------------------
	// BUCLE DEL JUEGO
	//----------------------------
	cam->rotate(glm::vec3(0, 1, 0), -272.0f);
	float rot = 0.5f;
	while (!glfwWindowShouldClose(TMotor::instancia().getVentana())) {

		caja2->rotate(glm::vec3(1, 0, 0), rot);
		//luzInfinita->translate(glm::vec3(cam->getPosition().x, cam->getPosition().y+5, cam->getPosition().z));

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
		//--------------------------------//
		//	R E N D E R I Z A D O
		//--------------------------------//
		TMotor::instancia().draw();						

		glfwPollEvents(); // Comprobacion de eventos (Teclado, raton, joystick)
	}

	TMotor::instancia().close();
	*/
	return 0;
	
}
