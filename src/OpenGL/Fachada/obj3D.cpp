#include "obj3D.hpp"

obj3D::obj3D(TNodo *m, const char *n, GLuint i) {
	mesh = m;
	position = getPosition();
	rotation = glm::vec3(0.0f);
	name = n;
	id = i;
}

obj3D::~obj3D() {
}

void obj3D::rotate(glm::vec3 axis, GLfloat angle) {
	if (axis.x == 1)
		rotation.x += angle;
	if (axis.y == 1)
		rotation.y += angle;
	if (axis.z == 1)
		rotation.z += angle;

	static_cast<TTransform*>(mesh->getPadre()->getPadre()->getEntidad())->rotar(axis.x, axis.y, axis.z, angle);
}

void obj3D::translate(glm::vec3 pos) {
	static_cast<TTransform*>(mesh->getPadre()->getEntidad())->trasladar(pos.x, pos.y, pos.z);
	position = getPosition();
}

GLuint obj3D::getID() {
	return id;
}

const char *obj3D::getName() {
	return name;
}

glm::vec3 obj3D::getPosition() {
	glm::vec4 aux = (static_cast<TTransform*>(mesh->getPadre()->getEntidad())->getMatriz()) * glm::vec4(.0f, .0f, .0f, 1.0f);
	return glm::vec3(aux.x, aux.y, aux.z);
}

glm::vec3 obj3D::getRotation() {
	/*
	Note: Care must be taken if the angle around the y-axis is exactly +/-90.
	In that case all elements in the first column and last row, except the one in the lower corner,
	which is either 1 or -1, will be 0 (cos(1)=0).
	One solution would be to fix the rotation around the x-axis at 180 and
	compute the angle around the z-axis from: atan2(r_12, -r_22).
	*/

	// Como son cosenos y senos, hay que hacer el arctan de los elementos (y alguna cosa mas)
	/*
	glm::mat4 m = static_cast<TTransform*>(mesh->getPadre()->getPadre()->getEntidad())->getMatriz();
	rotation.x = atan2(m[2][1], m[2][2]);
	rotation.y = atan2(-m[2][0], sqrt(pow(m[2][1], 2) + pow(m[2][2], 2)));
	rotation.z = atan2(m[1][0], m[0][0]);
	*/
	return rotation;
}

bool obj3D::isVisible() {
	return static_cast<TMalla*>(mesh->getEntidad())->isVisible();
}




void obj3D::setID(GLuint id) {
	this->id = id;
}

void obj3D::setName(const char *nombre) {
	name = nombre;
	mesh->setName(nombre);
}

void obj3D::setVisible(bool visible) {
	static_cast<TMalla*>(mesh->getEntidad())->setVisible(visible);
}

void obj3D::setRotation(glm::vec3 axis, GLfloat angle) {

	if (axis.x == 1)
		rotation.x = angle;
	if (axis.y == 1)
		rotation.y = angle;
	if (axis.z == 1)
		rotation.z = angle;

	static_cast<TTransform*>(mesh->getPadre()->getPadre()->getEntidad())->setRotation(axis.x, axis.y, axis.z, angle);
}

void obj3D::setRotation(float X, float Y, float Z) {

	setRotation(glm::vec3(1, 0, 0), X);

	setRotation(glm::vec3(0, 1, 0), Y);

	setRotation(glm::vec3(0, 0, 1), Z);

}


void obj3D::setPosition(float X, float Y, float Z) {
	static_cast<TTransform*>(mesh->getPadre()->getEntidad())->setPosition(X, Y, Z);
	position = getPosition();

}