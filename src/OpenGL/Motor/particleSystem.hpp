#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "obj3D.hpp"
#include "Shader.hpp"

//Estructura para definir una particula individual
struct Particula{
    glm::vec3 position, velocity; //Posicion y velocidad de la particular
    float size; //Tamaño de la particular en coordenadas del mundo, no de pantalla
    float life; //Vida de la particular, se utiliza para que las particulas vayan creandose y muriendo continuamente
    float distanceToCamera; //Distancia de la particula a la camara

    //Constructor para definir una particula
    Particula(){
        position = glm::vec3(0.f);
        velocity = glm::vec3(0.f);
        size = 0.65f;
        life = 0.0f;
        distanceToCamera = 0.0f;
    }
};

const int numMaxParticles = 50; //Numero maximo de particulas del sistema

class particleSystem 
{
    public:
        particleSystem(); //Constructor
        ~particleSystem(); //Destructor

        void draw(Shader *s); //Dibujado 

        //Metodos SET
        void setPosition(glm::vec3 p);
        void setOrientation(glm::vec3 o);
        void setSize(float s);
        void setColor(glm::vec3 c);

    private:
        // --- VARIABLES ---
        std::vector<Particula *> particulas; //Contenedor de todas las particulas del sistema

        int lastParticleFound; //Ultima particula recuperada
        int particlesAlive; //Particulas vivas en cada iteracion
        int newParticlesPerIteration; //Numero de particulas nuevas por iteracion

        glm::vec3 posParticle; //Posicion del sistema de particulas
        glm::vec3 oriParticle; //Orientacion del sistema de particulas
        float sizeParticle; //Tamanyo del sistema de particulas
        glm::vec3 colorParticle; //Color del sistema de particulas

        GLuint VAO, VBO, VBO_position; //Buffers de Opengl
        GLfloat *position_data; //Array de datos de posicion que se rellena en cada iteracion y despues se le pasa al buffer

        // --- METODOS ---
        void setBuffersOpenGL();
        int findLastDeadParticle();
        void rebirthParticle(Particula *p);
        void update();
};

#endif