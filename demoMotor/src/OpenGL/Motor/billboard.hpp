#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "obj3D.hpp"
#include "Shader.hpp"

class billboard {
    public:
        billboard(obj3D *o); //Constructor
        ~billboard(); //Destructor

        void draw(Shader* s); //Dibujado

        void setImage(const char* p); //Establecer imagen del billboard
        void setActive(bool b); //Activar/desactivar el billboard

    private:
        //---- VARIABLES ----
        obj3D *elemento; //Objeto sobre el que estara el billboard

        const char* path; //Ruta de la imagen del billboard
        bool active = true; //Booleano para controlar el dibujado del billboard

        GLuint VAO, VBO; //Buffers de OpenGL

        //---- FUNCIONES ----
        void setBuffersOpenGL(); 


};

#endif