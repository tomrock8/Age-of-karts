#pragma once
#include "libGL.hpp"
#include "Skybox.hpp"
#include "TMalla.hpp"
#include "TCamara.hpp"
#include "TLuz.hpp"
#include "TTransform.hpp"
#include "TGestorRecursos.hpp"
#include "TNodo.hpp"
#include "obj3D.hpp"
#include "cameraThird.hpp"
#include "hud.hpp"
#include <iostream>
#include <stack>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#define WIDTH 1600
#define HEIGHT 900
#define PI 3.14159265358979323846  /* pi */
#define RADTODEG 180.0f / PI

class TMotor {

private:


public:
	static TMotor &instancia();
	TMotor();
	void initDebugWindow();
	void closeDebugWindow();
	void resizeScreen(int width, int height);
	void close();

	//obj3D *newCameraNode(glm::vec3 traslation, const char * name, const char* parentNode);
	//obj3D *newLightNode(glm::vec3 traslation, const char * name, glm::vec4 dir, float att, float corte, const char* parentNode);
	//TNodo *newMeshNode(glm::vec3 traslation, const char * name, const char * path);
	obj3D *newCameraNode(const char * name, const char* parentNode);
	//cameraThird * newCamera3ThPerson(const char * name, const char * parentName);
	obj3D *newLightNode(const char * name, glm::vec4 dir, float att, float corte, bool shadow, bool active, const char* parentNode);
	obj3D *newMeshNode(const char * name, const char * path, const char* parentNode,bool sta);
	void precarga(const char * modelo);

	void newHud(const char* n);
	//float *toEuler(double pich, double yaw, double roll);

	// METODOS GET
	GLFWwindow *getVentana();
	GLfloat getWidth();
	GLfloat getHeight();
	TNodo *getSceneNode();
	TNodo *getActiveCamera();
	obj3D *getObjActiveCamera();
	hud *getHud(const char* n);
	hud *getActiveHud();
	Shader *getShader();
	Shader *getShaderHUD();
	Shader *getShaderProjectedShadows();
	Shader *getShaderDirectionalDepth();
	Shader *getShaderPointDepth();
	Shader *getShaderSkybox();
	Shader *getShaderDebug();
	std::vector <TNodo *> getActiveLights();
	TNodo *getNode(const char * nombre);
	TGestorRecursos *getGR();
	bool getRenderDebug();

	// METODOS SET
	void setActiveCamera(TNodo *c);
	void setActiveLight(TNodo *l);
	void setActiveHud(const char *n);
	void setRenderDebug(bool renderDebug);

	// DIBUJADO
	void clean();
	void draw(int tipo);
	void drawCamera(Shader *s);
	void drawLight();
	
	void toEulerAngle(float x,float y,float z, float w, float& roll, float& pitch, float& yaw);

protected:
	TNodo * scene; //Nodo raiz

	//Shaders
	Shader *shader; // Shader que se usa en el motor en la parte 3D
	Shader *shaderHUD; //Shader que usa el motor para el dibujado del HUD
	Shader *shaderProjectedShadows; //Shader para el calculo de de las sombras proyectadas
	Shader *shaderDirectionalDepth; //Shader para el calculo de las sombras de las luces dirigidas
	Shader *shaderPointDepth; //Shader para el calculo de las sombras de las luces puntuales
	Shader *shaderSkybox; //Shader para el dibujado del skybox
	Shader *shaderDebug; //Shader para el modo debug de Bullet

	//Camaras
	std::vector<TNodo *> cameras;   //punteros que guardan la direccion de las camaras, para actualizarlas segun registro (nombre)
	TNodo *activeCamera;       // Camara activa del vector

	//Luces
	std::vector <TNodo *> lights;
	std::vector <TNodo *> activeLights;

	//Hud
	std::vector <hud*> HUDs; //Vector que guarda los diferentes huds del juego
	hud *activeHud; //Hud activo en cada momento

	//Obj3D
	std::vector <obj3D *> objetos; //Array que almacena los distintos objetos creados a lo largo del programa
	obj3D *getObjeto(const char* name); //Metodo que devuelve un objeto a partir de su nombre

	bool renderDebug;
	TGestorRecursos *gestorRecursos;
	GLFWwindow *ventana;
	int screenHEIGHT;
	int screenWIDTH;

	GLuint contID;

	// ----------------------
	//  METODOS PRIVADOS
	// ----------------------

	// Malla
	TMalla  *createMesh(const char *fich,bool sta);
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
	
	//Skybox del mapa
	Skybox *skybox;
};