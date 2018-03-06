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

void createCamera(float x, float y, float z, const char *name) {
	//----------------------------------------------------------------------//
	//-------------------------------CAMARA---------------------------------//
	//----------------------------------------------------------------------//

	//---transformaciones de la cama---

	TTransform *rotateCam = TMotor::instancia().createTransformation();
	rotateCam->rotar(0, 0, 1, 1);
	TNodo *rotationNodeCam = TMotor::instancia().createTransformationNode(TMotor::instancia().getSceneNode(), rotateCam, "rotacion_camara");

	TTransform *traslationCam = TMotor::instancia().createTransformation();
	traslationCam->trasladar( x, y, z);
	TNodo *traslationNodeCam = TMotor::instancia().createTransformationNode(rotationNodeCam, traslationCam, "traslacion_camara");

	//---camara---
	TCamara *cam1 = TMotor::instancia().createCam();
	float height = TMotor::instancia().getHeight();
	float width = TMotor::instancia().getWidth();
	cam1->setPerspective(height, width, 0.1f, 100.0f, 70.0f);
	TNodo *Cam1Node = TMotor::instancia().createCamNode(traslationNodeCam, cam1, 0, name);
}
void createLight(float x, float y, float z, const char *name) {
	//---transformaciones de la luz
	TTransform *rotateLight = TMotor::instancia().createTransformation();
	rotateLight->rotar(0, 0, 1, 45);
	TNodo *rotationNodeLight = TMotor::instancia().createTransformationNode(TMotor::instancia().getSceneNode(), rotateLight, "rotacion_Luz");

	TTransform *traslationLight = TMotor::instancia().createTransformation();
	traslationLight->trasladar(x,y,z);
	TNodo *traslationNodeLight = TMotor::instancia().createTransformationNode(rotationNodeLight, traslationLight, "traslacion_Luz");
	//---Luz-------
	TLuz  *luz1 = new TLuz();
	TNodo *luz1Node = TMotor::instancia().createLightNode(traslationNodeLight, luz1, 0, name);
}
void createMesh(float x, float y, float z, const char *name, const char *path) {
	//---transformaciones de la Malla-----
	TTransform *rotateMesh = TMotor::instancia().createTransformation();
	//rotateMesh->rotar(0, 0, 1, -90);
	TNodo *rotationNodeMesh = TMotor::instancia().createTransformationNode(TMotor::instancia().getSceneNode(), rotateMesh, "rotacion_malla_caja");

	TTransform *traslationMesh = TMotor::instancia().createTransformation();
	traslationMesh->trasladar(x, y, z);
	TNodo *traslationNodeMesh = TMotor::instancia().createTransformationNode(rotationNodeMesh, traslationMesh, "traslacion_malla_caja");

	//--- Malla
	TMalla *caja = TMotor::instancia().createMesh(path);
	TNodo  *cajaNode = TMotor::instancia().createMeshNode(traslationNodeMesh, caja, 0, name);
}
void arbolEscena() {
	
	createCamera(0.0f, 0.0f, 0.0f, "camara_libre");
	createLight(0.0f, 40.0f, -100.0f, "luz1");
	createMesh(0.0f, 0.0f, -100.0f, "Barco", "assets/BARCO.obj");
	


}

int main(void) {
	TMotor::instancia();
	//----------------------------------------------------------------------//
	//-------------------------------TIMER----------------------------------//
	//----------------------------------------------------------------------//
	double sec = 0.0;									//glfwSetTime para empezar donde se quiera
	Shader *shader = new Shader("assets/shaders/vertexShader.txt", "assets/shaders/fragmentShader.txt");

	
	//----------------------------------------------------------------------//
	//--------------------------BUCLE DEL JUEGO-----------------------------//
	//----------------------------------------------------------------------//


	arbolEscena();
	while (!glfwWindowShouldClose(TMotor::instancia().getVentana())) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);					//Limpia el buffer del color
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
