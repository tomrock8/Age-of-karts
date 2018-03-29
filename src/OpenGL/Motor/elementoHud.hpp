#ifndef ELEMENTOHUD_H
#define ELEMENTOHUD_H
 
#include "libGL.hpp" 
#include "stb_image.h"
#include "Shader.hpp"
#include <array>

using namespace std; 
 
/*Clase para definir los elementos del hud. Estos elementos son cuadrados
de un ancho y alto  a los que se asigna una textura. Se trasladasn y rotan mediante
una matriz modelo*/

class elementoHud  
{ 
public: 
    elementoHud(float w, float h, const char* n, const char* p); 
    ~elementoHud();

    const char* getName(); 

    //Dibujado
    void draw(Shader *shader);

    //Transformaciones
    void rotacion(float angle);
    void traslacion(float x, float y);
    void escalado(float x, float y);

private:
    //---- VARIABLES ----
    float width; //Ancho del quad
    float height; //Alto del quad
    const char* name; //Nombre del quad 
    const char* path; //Ruta de la imagen

    array<float, 20> data; //Array con las posiciones y coordenadas de textura de cada vertice del quad
    GLuint indices[6] = { //Indices para crear los triangulos que forman el cuadrado
		0, 1, 3,
		1, 2, 3
	};

    GLuint VAO, VBO, EBO; //Buffers de OpenGL

    GLuint idText; //Id de la textura al crearla con OpenGL

    glm::mat4 tras; //Matriz de traslacion
    glm::mat4 rot; //Matriz de rotacion
    glm::mat4 esc; //Matriz de escalado
    

    //---- FUNCIONES ----
    void setData();
    void setBuffersOpenGL();
    void loadImage();

};
#endif