#ifndef TOBJ3D_H 
#define TOBJ3D_H 

#include "libGL.hpp" 
#include "TMalla.hpp" 
#include "TTransform.hpp" 
#include "TNodo.hpp" 

using namespace std;

class obj3D {
public:
	obj3D(TNodo *mesh, const char *name, GLuint id);
	~obj3D();

	void setNode(TNodo * m);

	void translate(glm::vec3);
	void rotate(glm::vec3 axis, GLfloat angle);
	void scale(float x, float y, float z);
	void rotateFromParent();
	void traslateToParent();

	// METODOS GET
	GLuint getID();
	const char * getName();
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	TNodo * getNode();
	glm::vec3 getScale();
	bool isVisible();

	//METODOS SET
	void setID(GLuint id);
	void setRotation(glm::vec3 axis, GLfloat angle);
	void setPosition(float X, float Y, float Z);
	void setRotation(float X, float Y, float Z);
	void setScale(float X, float Y, float Z);
	void setName(const char *nombre);
	void setVisible(bool visible);


private:
	GLuint id;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 escala;
	const char *name;
	TNodo *node;
};
#endif