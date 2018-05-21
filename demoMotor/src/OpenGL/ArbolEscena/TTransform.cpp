#include "TTransform.hpp"

TTransform::TTransform() {//la inicializacion sera la de identidad
	identidad();
}

TTransform::~TTransform(){
	std::cout << "Destructor de TTransform\n";
}

// GESTIONAR LA MATRIZ
void TTransform::cargar(glm::mat4 mat) { matriz = mat; }
glm::mat4 TTransform::getMatriz() { return matriz; }
void TTransform::setMatriz(glm::mat4 p) {
	matriz = p;
}
// TRANSFORMACIONES
/* vector transformacion = vectorTraslacion * vectorRotacion * vectorEscalado * Vector Original; */
void TTransform::identidad() {
	/*
		| 1  0  0  0  |   |x|
		| 0  1  0  0  | x |y|
		| 0  0  1  0  |	  |z|
		| 0  0  0  1  |   |w|
	*/
	matriz = glm::mat4x4(1.0f);
}
void TTransform::trasladar(GLfloat x, GLfloat y, GLfloat z) {
	/*
		 | 1  0  0	tx | tx = traslacion en eje x
		 | 0  1  0	ty | ty = traslacion en eje y
		 | 0  0  1	tz | tz = traslacion en eje z
		 | 0  0  1	0  |

		 Hay que incluir despues de glm.hpp :	<glm/gtx/transform.hpp>
	*/
	matriz = glm::translate(matriz, glm::vec3(x, y, z));
}

void TTransform::setPosition(GLfloat x, GLfloat y, GLfloat z) {
	matriz = glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

void TTransform::rotar(GLfloat x, GLfloat y, GLfloat z, GLfloat g) {
	/*
		La funcion recogera que eje esta implicado en la rotacion , indicado con un 1

		----para el eje X-----
		1	0		0		0
		0	cos(g)	sin(g)	0
		0	-sin(g)	cos(g)	0
		0	0		0		1

		----para el eje Y----
		cos(g)	0	-sin(g)	0
		0		1	0		0
		sin(g)	0	cos(g)	0
		0		0	0		1

		----para el eje Z----
		cos(g)	sin(g)	0	0
		-sin(g)	cos(g)	0	0
		 0		0		1	0
		 0		0		0	1
	*/

	glm::vec3 myRotationAxis(x, y, z);
	matriz = glm::rotate(matriz, glm::radians(g), myRotationAxis);
}

void TTransform::setRotation(GLfloat x, GLfloat y, GLfloat z, GLfloat g) {
	glm::vec3 myRotationAxis(x, y, z);
	matriz = glm::rotate(glm::mat4(), glm::radians(g), myRotationAxis);
}

void TTransform::escalar(GLfloat x, GLfloat y, GLfloat z) {
	/*
		|sX	0	0	0|
		|0	sY	0	0|
		|0	0	sZ	0|
		|0	0	0	1|
	*/
	matriz = glm::scale(matriz, glm::vec3(x, y, z));
}

void TTransform::setScale(GLfloat x, GLfloat y, GLfloat z) {
	matriz = glm::scale(glm::mat4(), glm::vec3(x, y, z));
}

//----------------------------//
//----------METODOS SET-------//
//----------------------------//

/*
En la funci�n display() se llaman dos funciones de matrices:  glPhusMatrix() y glPopMatrix.
Las funciones de traslaci�n multiplican la matriz actual por la matriz de transaci�n creada con los argumentosque se le pasan.
OpenGL dispone de unos stacks o pilas de matrices, que permiten almacenar y recuperar una matriz anterior.
El nombre de las pilas son GL_MODELVIEW y GL_PROJECTION, solo se suele utilizar la pila de GL_MODELVIEW.
La matriz superior de la pila es sobre la que se aplican las distintas transformaciones, multiplicando la por la matriz
que generan las distintas funciones.
Para guardar y recuperar las matrices OpenGL usa glPushMatrix() y glPopMatrix():
glPushMatrix() realiza una copia de la matriz superior y la pone encima de la pila, de tal forma que las dos matrices superiores son iguales.
glPopMatrix() elimina la matriz superior, quedando en la parte superior la pila de la matriz que estava en el momento de la llamada
a la funcion glPushMatrix().
*/

void TTransform::beginDraw(Shader *shader) {
	//Apilar matriz actual
	matStack.push(modelMatrix);//no vale
	modelMatrix = matriz * modelMatrix;
}

void TTransform::endDraw() {
	//Desapilar matriz y ponerla como actual
	modelMatrix = matStack.top();
	matStack.pop();
	glPopMatrix();
}