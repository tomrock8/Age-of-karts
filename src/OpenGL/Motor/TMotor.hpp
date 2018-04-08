#pragma once
#include "libGL.hpp"
#include "TMalla.hpp"
#include "TCamara.hpp"
#include "TLuz.hpp"
#include "TTransform.hpp"
#include "TGestorRecursos.hpp"
#include "TNodo.hpp"
#include "obj3D.hpp"
#include "hud.hpp"
#include <iostream>
#include <stack>

#define WIDTH 1600
#define HEIGHT 900
#define PI 3.14159265358979323846  /* pi */
#define RADTODEG 180.0f / PI

class TMotor {

private:


public:
	static TMotor &instancia();
	TMotor();
	void resizeScreen(int width, int height);
	void close();

	//obj3D *newCameraNode(glm::vec3 traslation, const char * name, const char* parentNode);
	//obj3D *newLightNode(glm::vec3 traslation, const char * name, glm::vec4 dir, float att, float corte, const char* parentNode);
	//TNodo *newMeshNode(glm::vec3 traslation, const char * name, const char * path);
	obj3D *newCameraNode(const char * name, const char* parentNode);
	obj3D *newLightNode(const char * name, glm::vec4 dir, float att, float corte, const char* parentNode);
	obj3D *newMeshNode(const char * name, const char * path, const char* parentNode);

	void newHud(const char* n);
	double *toEuler(double pich, double yaw, double roll);

	// METODOS GET
	GLFWwindow *getVentana();
	GLfloat getWidth();
	GLfloat getHeight();
	TNodo *getSceneNode();
	TNodo *getActiveCamera();
	hud *getHud(const char* n);
	hud *getActiveHud();
	Shader *getShaderHUD();
	Shader *getShaderDebug();
	std::vector <TNodo *> getActiveLights();
	TNodo *getNode(const char * nombre);

	// METODOS SET
	void setActiveCamera(TNodo *c);
	void setActiveLight(TNodo *l);
	void setActiveHud(const char *n);

	// DIBUJADO
	void clean();
	void draw(int tipo);
	void drawCamera();
	void drawLight();

protected:
	TNodo * scene; //Nodo raiz
	Shader *shader; // Shader que se usa en el motor en la parte 3D
	Shader *shaderHUD; //Shader que usa el motor para el dibujado del HUD
	Shader *shaderDebug; //Shader que usa el motor para el modo debug

	//Camaras
	std::vector<TNodo *> cameras;   //punteros que guardan la direccion de las camaras, para actualizarlas segun registro (nombre)
	TNodo *activeCamera;       // Camara activa del vector

	//Luces
	std::vector <TNodo *> lights;
	std::vector <TNodo *> activeLights;

	//Hud
	std::vector <hud*> HUDs; //Vector que guarda los diferentes huds del juego
	hud *activeHud; //Hud activo en cada momento

	TGestorRecursos *gestorRecursos;
	GLFWwindow *ventana;
	int screenHEIGHT;
	int screenWIDTH;

	GLuint contID;
	// ----------------------
	//  METODOS PRIVADOS
	// ----------------------

	// Malla
	TMalla  *createMesh(const char *fich);
	TNodo * createMeshNode(TNodo * padre, TMalla * mesh, const char * name);
	// Transformacion
	TTransform * createTransformation();
	TNodo   * createTransformationNode(TNodo * padre, TTransform * transf, const char* name);
	// Camara
	TCamara *createCam();
	TNodo   *createCamNode(TNodo *padre, TCamara *cam, const char* name);
	// Luz
	TLuz    *createLight();
	TNodo   *createLightNode(TNodo * padre, TLuz * luz, const char* name);
};