#include "TTransform.hpp"

//----------------------------//
//-----CONSTRUCTOR------------//
//----------------------------//
TTransform::TTransform() {//la inicializacion sera la de identidad
	identidad();
}
//----------------------------//
//-----GESTIONAR LA MATRIZ----//
//----------------------------//

void TTransform::cargar(glm::mat4 mat) {
	matriz = mat;
}
glm::mat4 TTransform::getMatriz() {
	return matriz;
}
//----------------------------//
//-----TRANSFORMACIONES-------//
//----------------------------//
/* vector transformacion = vectorTraslacion * vectorRotacion * vectorEscalado * Vector Original; */
void TTransform::transponer() {//cambiar filas por columnas
	glm::mat4x4 traspuesta;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			traspuesta[j][i] = matriz[i][j];
		}
	}
	matriz = traspuesta;
}
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
		 | 1  0  0	tx |
		 | 0  1  0	ty |
		 | 0  0  1	tz |
		 | 0  0  1	0  |

		 tx = valor a modificar en la matriz para trasladar en eje x
		 ty = valor a modificar en la matriz para trasladar en eje y
		 tz = valor a modificar en la matriz para trasladar en eje z
		 Hay que incluir despues de glm.hpp :	<glm/gtx/transform.hpp>
	*/

	matriz = glm::translate(glm::mat4(), glm::vec3(x, y, z)) ;

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
	matriz =  glm::rotate(glm::radians(g), myRotationAxis);
}
void TTransform::escalar(GLfloat x, GLfloat y, GLfloat z) {
	/*
		|sX	0	0	0|
		|0	sY	0	0|
		|0	0	sZ	0|
		|0	0	0	1|
		hay que incluir despues de glm.hpp :	<glm/gtx/transform.hpp>
												<glm/gtc/matrix_transform.hpp>
	*/
	//matriz =  glm::scale(x, y, z) * matriz;
}
//----------------------------//
//----------METODOS SET-------//
//----------------------------//

/*
En la función display() se llaman dos funciones de matrices:  glPhusMatrix() y glPopMatrix.
Las funciones de traslación multiplican la matriz actual por la matriz de transación creada con los argumentosque se le pasan.
OpenGL dispone de unos stacks o pilas de matrices, que permiten almacenar y recuperar una matriz anterior.
El nombre de las pilas son GL_MODELVIEW y GL_PROJECTION, solo se suele utilizar la pila de GL_MODELVIEW.
La matriz superior de la pila es sobre la que se aplican las distintas transformaciones, multiplicando la por la matriz
que generan las distintas funciones.
Para guardar y recuperar las matrices OpenGL usa glPushMatrix() y glPopMatrix():
glPushMatrix() realiza una copia de la matriz superior y la pone encima de la pila, de tal forma que las dos matrices superiores son iguales.
glPopMatrix() elimina la matriz superior, quedando en la parte superior la pila de la matriz que estava en el momento de la llamada
a la funcion glPushMatrix().
*/

void TTransform::beginDraw(Shader *shader)
{	//Apilar matriz actual
	//std::cout << "(TTransform::beginDraw) transformacion" << std::endl;
	matStack.push(modelMatrix);//no vale
	modelMatrix = modelMatrix * matriz;
	
}
void TTransform::endDraw() {
	//Desapilar matriz y ponerla como actual
	//std::cout << "(TTransform::endDraw) Desapilo" << std::endl;
	modelMatrix = matStack.top();
	matStack.pop();
	glPopMatrix();
}