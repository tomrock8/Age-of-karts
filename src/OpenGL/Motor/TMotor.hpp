#pragma once
#include "libGL.hpp"
#include "Skybox.hpp"
#include "TMalla.hpp"
#include "TCamara.hpp"
#include "TLuz.hpp"
#include "TTransform.hpp"
#include "TGestorRecursos.hpp"
#include "TNodo.hpp"
#include "TAnimacion.hpp"
#include "cameraThird.hpp"
#include "hud.hpp"
#include "GestorSonido.hpp"
#include <iostream>
#include <stack>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "billboard.hpp"
#include "particleSystem.hpp"

#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  
#endif

#define WIDTH 1600
#define HEIGHT 900

class TMotor {
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
	TNodo * createAnimationNode(TNodo * padre, TAnimacion * mesh, const char * name);
	obj3D *newLightNode(const char * name, glm::vec4 dir, float att, float corte, bool shadow, bool active, const char* parentNode);
	obj3D * newAnimation(const char * name, const char * path, const char * parentNode, int framesIni, int framesFin);
	TAnimacion * createAnimation(const char * path, int framesIni, int framesFin);
	obj3D *newMeshNode(const char * name, const char * path, const char* parentNode, bool sta);
	void precarga(const char * modelo);

	void newHud(const char* n);
	billboard *newBillboard(obj3D *o);
	particleSystem *newParticleSystem();
	//animaciones y malla
	TNodo * createStaticMeshNode(TNodo * padre, const char * path, const char * name);
	//float *toEuler(double pich, double yaw, double roll);

	// METODOS GET
	GLFWwindow *getVentana();
	GLfloat getWidth();
	GLfloat getHeight();
	TNodo *getSceneNode();
	TNodo *getActiveCamera();
	glm::mat4 getV();
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
	Shader *getShaderSilhouette();
	std::vector <TNodo *> getActiveLights();
	TNodo *getNode(const char * nombre);
	TGestorRecursos *getGR();
	bool getRenderDebug();
	GestorSonido *getGestorSonido();

	// METODOS SET
	void setActiveCamera(TNodo *c);
	void setActiveLight(TNodo *l);
	void setActiveHud(const char *n);
	void setRenderDebug(bool renderDebug);
	void setDebugBullet(bool b);
	void setVerticesDebug(float a, float b, float c, float x, float y, float z);

	// DIBUJADO
	void clean();
	void draw(int tipo);
	void drawCamera();
	void drawLight(Shader *s);

	void toEulerAngle(float x, float y, float z, float w, float& roll, float& pitch, float& yaw);

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
	Shader *shaderCartoon; //Shader para el efecto cartoon
	Shader *shaderSilhouette; //Shader para crear el contorno de los objetos en el efecto cartoon
	Shader *shaderBillboard; //Shader para dibujar los diferentes billboards 
	Shader *shaderParticles; //Shader para el dibujado de las particulas
	Shader *shaderGbuffer; //Shader para renderizar la escena en el buffer que despues se usara en el deferred shading
	Shader *shaderDeferred; //Shader que renderiza en la pantalla a partir de los datos guardados por el gBuffer

	//Tipo de shader usado
	const char* shaderName;

	//Camaras
	std::vector<TNodo *> cameras;   //punteros que guardan la direccion de las camaras, para actualizarlas segun registro (nombre)
	TNodo *activeCamera;       // Camara activa del vector
	glm::mat4 v;  //Matriz view de la camara activa

	//Luces
	std::vector <TNodo *> lights;
	std::vector <TNodo *> activeLights;

	//Hud
	std::vector <hud*> HUDs; //Vector que guarda los diferentes huds del juego
	hud *activeHud; //Hud activo en cada momento

	//Sonido
	GestorSonido *gestorSonido;

	//Skybox del mapa
	Skybox *skybox;

	//Debug Bullet
	std::vector <GLfloat> vertices; //Array de vertices para los puntos de las lineas

	//Billboards
	std::vector <billboard *> billboards; //Array de los diferentes billboards utilizados en el motor

	//Particles
	std::vector <particleSystem *> particleSystems; //Array con las diferentes particulas usadas en el motor

	//Objetos
	std::vector <obj3D *> notShadowObjects; //Array que contendra los objetos del mundo que no deben proyectar sombras (Mapa, elementos...)

	//Deferred shading
	GLuint defBuffer; //Buffer que contendra todas las texturas con las posicion, normales y colores para renderizar posterioremente mediante deferred shading
	GLuint defPosition, defNormal, defDiffuseSpecular; //Cada una de las texturas que contendran la informacion anterior

	bool renderDebug;
	bool debugBullet = false;
	TGestorRecursos *gestorRecursos;
	GLFWwindow *ventana;
	int screenHEIGHT;
	int screenWIDTH;

	GLuint contID;

	// ----------------------
	//  METODOS PRIVADOS
	// ----------------------

	// Malla
	TMalla  *createMesh(const char *fich, bool sta);
	// Nodo
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
	//Deferred shading
	void setDeferredBuffers();
};