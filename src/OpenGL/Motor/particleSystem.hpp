#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "obj3D.hpp"
#include "Shader.hpp"

//Estructura para definir una particula individual
struct Particula{
    glm::vec3 position, velocity; //Posicion y velocidad de la particular
    float size; //Tamaño de la particular en coordenadas del mundo, no de pantalla
    float life; //Vida de la particular, se utiliza para que las particulas vayan creandose y muriendo continuamente
};


        const int numMaxParticles = 100000; //Numero maximo de particulas del sistema

class particleSystem 
{
    public:
        particleSystem(obj3D *o); //Constructor

    private:
        // --- VARIABLES ---
        Particula particulas[numMaxParticles]; //Contenedor de todas las particulas del sistema

        obj3D *elemento; //Objeto al que seguiran las particulas

        GLuint VAO, VBO, VBO_position; //Buffers de Opengl

        // --- METODOS ---
        void setBuffersOpenGL();
};

#endif