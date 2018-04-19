#ifndef CAMERATHIRD_H 
#define CAMERATHIRD_H 

#include "libGL.hpp" 
#include "TMalla.hpp" 
#include "TTransform.hpp" 
#include "obj3D.hpp" 
#include "btBulletDynamicsCommon.h"
#include "MotorFisicas.hpp"


using namespace std;

class cameraThird
{
public:
	cameraThird(const char *name, const char *parentName);
	~cameraThird();

	void update(glm::vec3 posicion,glm::vec3 rotacion,btVector3 direccion);

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
	void lookAt(glm::vec3 posicion);

private:
	
	GLuint id;
	glm::mat4 position;
	glm::mat4 rotation;
	const char *name;
	//Corredor* nodo;
	obj3D* camara;

	float direction;
	float zdirection;
	//ICameraSceneNode *camera;
	bool fpsActiva;
	float XCamera3;
	float YCamera3;
	float ZCamera3;
	float XCamera1;
	float YCamera1;
	float ZCamera1;
	float auxX;
	btDefaultMotionState *MotionState;
	btCollisionShape *Shape;
	btRigidBody *rigidBody;

	



};
#endif