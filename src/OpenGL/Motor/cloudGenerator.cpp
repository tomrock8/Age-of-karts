#include "TMotor.hpp"

//Constructor que establece todos los parametros del area de nubes y la posicion individual de cada nube dentro del area
cloudGenerator::cloudGenerator(int minW, int maxW, int minH, int maxH, int minL, int maxL, int numC){
    //Guardamos el numero de nubes
    numClouds = numC;
    //Seteamos los buffers de OpenGL para la creacion de cada nube
    setCloudBuffers();
    //Situamos cada nube en una posicion aleatoria dentro del area definida por los parametros recibidos
    for (int i = 0; i < numC; i++){
        int x = rand() % (maxW - minW + 1) + minW;
        int z = rand() % (maxL - minL + 1) + minL;
        int y = rand() % (maxH - minH + 1) + minH;
        positionClouds.push_back(glm::vec3(x,y,z));
    }
}

//Funcion que establece los datos de cada quad (nube) en los buffers de OpenGL
void cloudGenerator::setCloudBuffers(){
    //Coordenadas para crear cada nube
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

//Funcion para dibujar cada una de las nubes dentro del area
void cloudGenerator::draw(Shader* s){
    //Bucle para dibujar el numero de nubes del area
    for (int i = 0; i < numClouds; i++){

        //Activamos la textura 0 y enlazamos la imagen del elemento hud
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TGestorImagenes::getInstancia()->cargarImagen("assets/cloud.png")->getID());
        //Le pasamos la textura al shader
        s->setInt("image", 0);

        //Calculamos la matriz viewProjection y se la pasamos al shader
        glm::mat4 vp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getActiveViewMatrix();
        s->setMat4("vp", vp);

        //Le pasamos la posicion y el tamanyo de la nube
        s->setVec3("cloudPosition", positionClouds.at(i));
        s->setVec2("sizeCloud", glm::vec2(0.2, 0.075));

        //Enlazamos el VAO que antes hemos rellenado
        glBindVertexArray(VAO);
        //Dibujamos la nube
        glDrawArrays(GL_TRIANGLES, 0, 18);

        //Desenlazamos el VAO y la textura de la imagen usada hasta el siguiente dibujado
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

