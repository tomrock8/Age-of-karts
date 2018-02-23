#ifndef TENTIDAD_H
#define TENTIDAD_H
#include "libGL.hpp"
#include <iostream>
#include <stack>

class TEntidad {
public:
	 //---COSNTRUCTOR---//
	TEntidad() {};
	 //---DESTRUCTOR---//
	~TEntidad() {};
	 //---Metodos set--//
	 virtual void setModelView(GLfloat x, GLfloat y, GLfloat z) {};
	 virtual void setprojectionMatrix(glm::mat4 PM) {};
	 virtual void setMVPMatrix(glm::mat4 MVP) {};
	 //--Metodos get--//
	/* virtual glm::mat4 getprojectionMatrix(){};
	 virtual glm::mat4 getViewMatrix() {};
	 virtual glm::mat4 getMVPMatrix() {};*/
	 //---METODOS DE DIBUJADO--//
	 virtual void beginDraw() {};		//se ejecuta cuando se baja un nivel en el arbol de la escena
	 virtual void endDraw() {};			//se ejecuta cuando se sube un nivel en el arbol de la escena
	 /*-Normalmente sólo las transformaciones necesitan
	    hacer algo en el método endDraw cuando se sube un
	    nivel del árbol
 	   -Las demás entidades, normalmente tendrán un método endDraw vacío 
	 */
	 

protected://las matrices tienen que ser estaticas como las pilas m compartiudas por todas las entidades
	//se inicializa la pila cuando se crea el motor
	static std::stack<glm::mat4> matStack;	
	static glm::mat4 viewMatrix;				//matriz de vista
	static glm::mat4 projectionMatrix;			//matriz de proyeccion de los puntos en el espacio,  punto de projeccion sera finito
	static glm::mat4 MVPmatrix;					//Matriz Modelo Vista Proyeccion proyeccion * vista * modelo

};
#endif