#pragma once

#include  <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include  <iostream>

class TEntidad {
public:
	 //---COSNTRUCTOR---//
	 TEntidad();
	 //---DESTRUCTOR---//
	 ~TEntidad();
	 //---Metodos set--//
	 void setModelView(GLfloat x, GLfloat y, GLfloat z);
	 void setprojectionMatrix(glm::mat4 PM);
	 void setMVPMatrix(glm::mat4 MVP);
	 //--Metodos get--//
	 glm::mat4 getprojectionMatrix();
	 glm::mat4 getViewMatrix();
	 glm::mat4 getMVPMatrix();
	 //---METODOS DE DIBUJADO--//
	 virtual void beginDraw(); //se ejecuta cuando se baja un nivel en el arbol de la escena
	 virtual void endDraw();   //se ejecuta cuando se sube un nivel en el arbol de la escena
	 /*-Normalmente sólo las transformaciones necesitan
	    hacer algo en el método endDraw cuando se sube un
	    nivel del árbol
 	   -Las demás entidades, normalmente tendrán un método endDraw vacío 
	 */
protected:
	glm::mat4 viewMatrix;				//matriz de vista
	glm::mat4 projectionMatrix;		//matriz de proyeccion de los puntos en el espacio,  punto de projeccion sera finito
	glm::mat4 MVPmatrix;				//Matriz Modelo Vista Proyeccion proyeccion * vista * modelo

};
