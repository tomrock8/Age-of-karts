#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "libGL.hpp"

class boundingBox {
    public:
        boundingBox(glm::vec3 p, glm::vec3 s); //Constructor
        ~boundingBox(); //Destructor

        void draw(glm::mat4 modelMatrixObject); //Dibujado

    private:
        // --- VARIABLES ---
        glm::vec3 positionBbox; //Posicion del bounding box = centro de la malla que engloba
        glm::vec3 sizeBbox; //Tamanyo del bounding box = tamanyo de la malla que engloba

        GLuint VAO, EBO, VBO; //Buffers de OpenGL para guardar los datos del bounding box

        // --- FUNCIONES ---
        void setBuffersOpenGL(); 
};

#endif