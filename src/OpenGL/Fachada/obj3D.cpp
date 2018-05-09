#include "obj3D.hpp"
#include "TMotor.hpp"

obj3D::obj3D(TNodo *m, const char *n, GLuint i) {
	node = m;
	position = getPosition();
	rotation = glm::vec3(0.0f);
	name = n;
	id = i;

}

obj3D::~obj3D() {
	setVisible(false);
	//delete node;
}
void obj3D::setNode(TNodo *m) {
	node = m;
}
void obj3D::rotate(glm::vec3 axis, GLfloat angle) {
	rotation.x += angle * axis.x;
	rotation.y += angle * axis.y;
	rotation.z += angle * axis.z;
	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->rotar(axis.x, axis.y, axis.z, angle);
}

// Translada el objeto acumulando la posicion anterior
void obj3D::translate(glm::vec3 pos) {
	static_cast<TTransform*>(node->getPadre()->getEntidad())->trasladar(pos.x, pos.y, pos.z);
	position = getPosition();
}

void obj3D::scale(float x, float y, float z) {
	static_cast<TTransform*>(node->getPadre()->getPadre()->getPadre()->getEntidad())->escalar(x, y, z);
	escala = glm::vec3(x, y, z);
}


// METODOS SET
void obj3D::setRotation(glm::vec3 axis, GLfloat angle) {
	rotation.x = angle * axis.x;
	rotation.y = angle * axis.y;
	rotation.z = angle * axis.z;

	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->setRotation(axis.x, axis.y, axis.z, angle);
}

void obj3D::setRotation(float X, float Y, float Z) {
	setRotation(glm::vec3(1, 0, 0), X); // Reiniciar la rotacion
	rotate(glm::vec3(0, 1, 0), Y); // Acumular la rotacion a partir de la reiniciada
	rotate(glm::vec3(0, 0, 1), Z);
}

// METODOS GET
GLuint obj3D::getID() { return id; }
const char *obj3D::getName() { return name; }
TNodo *obj3D::getNode() { return node; }
glm::vec3 obj3D::getScale() { return escala; }
bool obj3D::isVisible() { return static_cast<TMalla*>(node->getEntidad())->isVisible(); }

glm::vec3 obj3D::getPosition() {
	glm::vec4 aux = (static_cast<TTransform*>(node->getPadre()->getEntidad())->getMatriz()) * glm::vec4(.0f, .0f, .0f, 1.0f);
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
	return rotation;
}

// METODOS SET
void obj3D::setID(GLuint id) { this->id = id; }
void obj3D::setVisible(bool visible) { static_cast<TMalla*>(node->getEntidad())->setVisible(visible); }

void obj3D::setName(const char *nombre) {
	name = nombre;
	node->setName(nombre);
}

void obj3D::setPosition(float X, float Y, float Z) {
	static_cast<TTransform*>(node->getPadre()->getEntidad())->setPosition(X, Y, Z);
	position = getPosition();

}

void obj3D::setScale(float x, float y, float z) {
	static_cast<TTransform*>(node->getPadre()->getPadre()->getPadre()->getEntidad())->setScale(x, y, z);
	escala = glm::vec3(x, y, z);
}