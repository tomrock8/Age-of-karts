#include "boundingBox.hpp"
#include "TMotor.hpp"

// Constructor que recibe la posicion y tamanyo del bounding box
boundingBox::boundingBox(glm::vec3 p, glm::vec3 s){
    //Guardamos los datos de posicion y tamanyo
    positionBbox = p;
    sizeBbox = s;

    //Inicializamos los buffers de OpenGL
    setBuffersOpenGL();
}

//Destructor que elimina los buffers creados de OpenGL
boundingBox::~boundingBox(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Funcion que inicializa los buffers necesarios de OpenGL y le pasa los datos de los vertices del bounding box
void boundingBox::setBuffersOpenGL(){
    //Datos de posicion de los vertices del cubo que forma el bounding box
    static GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f
    };

    //Inicalizar cada uno de los buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Enlazar el VAO en el que se guardan los distintos datos
    glBindVertexArray(VAO);

    //Enlazar el VBO al que se le pasan los datos del cubo del bounding box
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    //Pasamos los datos de posicion de cada vertice del cubo al buffer VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Desactivamos el VAO hasta el dibujado del quad
    glBindVertexArray(0); 
}

//Funcion que dibuja el bounding box a partir de la posicion en el mundo de la malla qu engloba
void boundingBox::draw(glm::vec3 posMesh){
    //Activamos el shader para el dibujado de los boundign boxes
    TMotor::instancia().getShaderDebugBbox()->use();

    //Calculamos la matriz model del bounding box
    glm::mat4 model; //Matriz identidad
    model = glm::scale(model, sizeBbox); //Primero escalamos el bounding box
    model = glm::translate(model, positionBbox); //Despues, lo trasladamos al centro (o donde deberia estar el centro) de la malla
    model = glm::translate(model, posMesh); //Y por ultimo, lo traslamos al lugar que ocupa la malla en el mundo

    //Calculamos la mvp del bounding box y se la pasamos al shader
    glm::mat4 mvp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getActiveViewMatrix() * model;
    TMotor::instancia().getShaderDebugBbox()->setMat4("mvp", mvp);

    //Vamos a pintar los bounding boxes de azul
    TMotor::instancia().getShaderDebugBbox()->setVec3("color", glm::vec3(0.0, 0.0, 1.0));
    cout << "pollas" << endl;
    //Activamos el VAO con los datos de los vertices del bounding box
    glBindVertexArray(VAO);

    //Dibujamos el bounding box
    glDrawArrays(GL_LINES, 0, 108);
    cout << "pollas_1" << endl;
    //Desactivamos el VAO hasta el siguiente dibujado
    glBindVertexArray(0);
}