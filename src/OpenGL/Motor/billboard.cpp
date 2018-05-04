#include "TMotor.hpp"
#include "TGestorImagenes.hpp"

//Constructor que crea un billboard enlazandolo a un objeto
billboard::billboard(obj3D *o){
    elemento = o;

    //Establecemos los buffers de openGL para el dibujado del billboard
    setBuffersOpenGL();
}

billboard::~billboard(){
    //Eliminar los distintos buffers utilizados por OpenGL
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

//Funcion que inicializa los buffers de OpenGL y pasa los datos  de los vertices y coordenadas de textura
void billboard::setBuffersOpenGL(){

    //Coordenadas para crear el billboard
    static const GLfloat data[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };

    //Inicalizar cada uno de los buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Enlazar el VAO en el que se guardan los distintos datos
    glBindVertexArray(VAO);

    //Enlazar el VBO al que se le pasan los datos del quad
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

    //Pasamos los datos al buffer VBO
    //Primero, las posiciones de cada vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Segundo, las coordenadas de textura para cada vertice
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Desactivamos el VAO hasta el dibujado del quad
    glBindVertexArray(0); 
}

//Funcion para establecer la imagen del billboard
void billboard::setImage(const char* p){
    path = p;
}

//Funcion para dibujar el billboard encima del elemento 
void billboard::draw(Shader* s){

    //Activamos la textura 0 y enlazamos la imagen del elemento hud
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TGestorImagenes::getInstacia()->cargarImagen(path)->getID());
    //Le pasamos la textura al shader
    s->setInt("image", 0);

    //Calculamos la matriz viewProjection y se la pasamos al shader
    glm::mat4 vp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getV();
    s->setMat4("vp", vp);
    //Le pasamos la posicion del objeto sobre el que se situara el billboard
    s->setVec3("objPosition", elemento->getPosition());

    //Enlazamos el VAO que antes hemos rellenado
    glBindVertexArray(VAO);
    //Dibujamos el billboard
    glDrawArrays(GL_TRIANGLES, 0, 18);

    //Desenlazamos el VAO y la textura de la imagen usada hasta el siguiente dibujado
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}