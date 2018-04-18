#ifndef CAMERATHIRD_H 
#define CAMERATHIRD_H 

#include "libGL.hpp" 
#include "TMalla.hpp" 
#include "TTransform.hpp" 
#include "TNodo.hpp" 
#include "btBulletDynamicsCommon.h"
#include "MotorFisicas.hpp"

using namespace std;

class cameraThird
{
public:
	cameraThird(TNodo *mesh, TNodo *parentNode, const char *name, GLuint id);
	~cameraThird();


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
	TNodo *node;
	TNodo *parentNode;

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