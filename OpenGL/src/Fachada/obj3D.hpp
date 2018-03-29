#ifndef TOBJ3D_H 
#define TOBJ3D_H 

#include "libGL.hpp" 
#include "TMalla.hpp" 
#include "TTransform.hpp" 
#include "TNodo.hpp" 

using namespace std;

class obj3D
{
public:
	obj3D(TNodo *mesh, const char *name, GLuint id);
	~obj3D();

	void translate(glm::vec3);
	void rotate(glm::vec3 axis, GLfloat angle);

	// METODOS GET
	GLuint getID();
	const char * getName();
	glm::vec3 getPosition();
	glm::vec3 getRotation();

	//METODOS SET
	void setID(GLuint);

private:
	GLuint id;
	glm::vec3 position;
	glm::vec3 rotation;
	const char *name;
	TNodo *mesh;
};
#endif