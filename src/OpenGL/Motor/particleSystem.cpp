#include "TMotor.hpp"

//Constructor que crea un sistema de particulas que siguen a un objeto
particleSystem::particleSystem(obj3D *o){
    //Enlazamos el objeto 
    elemento = o;
    //Establecemos los buffers de OpenGL
    setBuffersOpenGL();
}

//Funcion para setear los buffers de OpenGl que se usaran para pasar os datos del sistema del particulas
void particleSystem::setBuffersOpenGL(){

    //Coordenadas para crear el quad que contendra cada particula (solo datos de posicion de cada vertice)
    static const GLfloat data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };

    //Inicalizar cada uno de los buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_position);

    //Enlazar el VAO en el que se guardan los distintos datos
    glBindVertexArray(VAO);

    //Primero, enlazar el VBO al que se le pasan los datos del quad (forma base de cada particula)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);
    //Lo establecemos como el primero atributo del buffer que le llegara al shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //Segundo, enlazar el VBO al que se le pasan los datos de posicion y tamaño de cada particula
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, numMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); //NULL = lo llenaremos en cada iteracion
    //Lo establecemos como el segundo atributo del buffer que le llegara al shader
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    //Desactivamos el VAO hasta el dibujado del quad
    glBindVertexArray(0); 
}