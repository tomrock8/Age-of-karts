#ifndef CAMERATHIRD_H 
#define CAMERATHIRD_H 

#include "libGL.hpp" 
#include "obj3D.hpp" 
#include "btBulletDynamicsCommon.h"
#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#endif

class cameraThird {
public:
	cameraThird(const char *name, const char *parentName);
	~cameraThird();

	void setPosition(glm::vec3 posicion,glm::vec3 rotacion,btVector3 direccion);
	void lookAt(glm::vec3 posicion);
	void comprobarInputs();
	btRigidBody * initializePhysics();
	
	void moveCamera(btRigidBody * pj1, btVector3 dir);
	void movefpsCamera(btRigidBody * pj1);
	void moveCameraControl(btRigidBody * pj1);

	// METODOS GET
	GLuint getID();
	const char * getName();
	glm::mat4 getParentPosition();
	glm::mat4 getParentRotation();


	//METODOS SET
	void setID(GLuint);
	void setName(const char *nombre);
	void setRotation(glm::vec3 axis, GLfloat angle);
	void setPosition(GLfloat X, GLfloat Y, GLfloat Z);

	void setParentNode(TNodo * p);
	

private:

	GLuint id;
	glm::mat4 position;
	glm::mat4 rotation;
	const char *name;
	//Corredor* nodo;
	obj3D* camara;

	float direction;
	float zdirection;
	bool fpsActiva;
	btVector3 direccion2;

	float auxX;
	btDefaultMotionState *MotionState;
	btCollisionShape *Shape;
	btRigidBody *rigidBody;

};
#endif