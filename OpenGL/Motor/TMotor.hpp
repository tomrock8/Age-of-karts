#pragma once
#include "libGL.hpp"
#include "TMalla.hpp"
#include "TCamara.hpp"
#include "TLuz.hpp"
#include "TTransform.hpp"
#include "TGestorRecursos.hpp"
#include "TNodo.hpp"
#include <iostream>
#include <stack>

#define WIDTH 640
#define HEIGHT 480
class TMotor {

private:


public:
	static TMotor &instancia();
	TMotor();
	void resizeScreen(int width, int height);

	//----------------------------------------//
	//---------CREACION DE ELEMENTOS----------//
	//----------------------------------------//
	TMalla  *createMesh(const char *fich);
	TNodo * createMeshNode(TNodo * padre, TMalla * mesh, int id, const char * name);
	TTransform * createTransformation();
	TNodo   * createTransformationNode(TNodo * padre, TTransform * transf, const char* name);
	TCamara *createCam();
	TNodo   *createCamNode(TNodo *padre, TCamara *cam,int s , const char* name);
	TLuz    *createLight();
	TNodo   *createLightNode(TNodo * padre, TLuz * luz, int id,const char* name);
	//---------------------------------------//
	//--------------METODOS GET--------------//
	//---------------------------------------//
	GLFWwindow *getVentana();
	GLfloat getWidth();
	GLfloat getHeight();
	TNodo *getSceneNode();
	TNodo *getActiveCamera();
	TNodo *getActiveLight();
	//---------------------------------------//
	//--------------METODOS SET--------------//
	//---------------------------------------//
	void setActiveCamera(TNodo *c);
	void setActiveLight(TNodo *l);
	
	//--------------------------------------//
	//---------------DIBUJADO---------------//
	//--------------------------------------//
	void draw(Shader *shader);
	void drawCamera(TNodo * camera,Shader *shader);
	void drawLight(TNodo * luz);


protected:
	//Nodo raiz
	TNodo * scene;
	//Camaras
	std::vector<TNodo *> cameras;	 //punteros que guardan la direccion de las camaras, para actualizarlas segun registro (nombre)
	TNodo *activeCamera;			 // se guarda la camara activa del vector
	//Luces
	std::vector<TNodo *> lights;
	TNodo *activeLight;
	
	
	TGestorRecursos *gestorRecursos;
	GLFWwindow *ventana;
	int screenHEIGHT;
	int screenWIDTH;
};