#include "TMotor.hpp"

//Constructor que crea un sistema de particulas que siguen a un objeto
particleSystem::particleSystem(obj3D *o){
    //Inicializamos el vector que contendra todas las particulas
    for (int i = 0; i < numMaxParticles; i++){
        particulas.push_back(Particula());
    }
    //Enlazamos el objeto 
    elemento = o;
    //Establecemos los buffers de OpenGL
    setBuffersOpenGL();
    //Inicializamos el contador a 0 de las distintas variables que utilizaremos
    lastParticleFound = 0;
    particlesAlive = 0;
    //Y el numero de particulas nuevas que habran en cada iteracion
    newParticlesPerIteration = 5000;
    delta = 0.01f;
}

//Destructor para eliminar los datos utilizados
particleSystem::~particleSystem(){
    //Eliminar los distintos buffers utilizados por OpenGL
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO_position);
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

//Funcion que devuelve el indice de la primera particula muerta o no usada que encuentre en el contenedor de particulas
int particleSystem::findLastDeadParticle(){
    //Primero, buscamos a partir de la ultima posicion guardada que tengamos
    for (int i = lastParticleFound; i < numMaxParticles; i++){
        //Si la vida de la particula es menor o igual que 0...
        if (particulas.at(i).life <= 0.0f){
            //Guardamos el indice en la variable 
            lastParticleFound = i;
            //Lo devolvemos
            return i;
        }
    }

    //En caso de no encontrar ninguna particula muerta, se realiza una busqueda lineal desde el principio
    for (int i = 0; i < numMaxParticles; i++){
        //Si la vida de la particula es menor o igual que 0...
        if (particulas.at(i).life <= 0.0f){
            //Guardamos el indice en la variable 
            lastParticleFound = i;
            //Lo devolvemos
            return i;
        }
    }

    //Finalmento, si todas estan vivas, se devuelve el indice de la primera particula del contenedor
    lastParticleFound = 0;
    return 0;
}

//Funcion que inicializa una nueva particula del sistema
void particleSystem::rebirthParticle(Particula p){
    //Valor random (0-50) para calcular los distintos datos de la particula
    float random = ((rand() % 100) - 50) / 10.0f;
    //La posicion de la particula dependera del objeto al que este enlazada
    p.position = glm::vec3(elemento->getPosition()[0], elemento->getPosition()[1], elemento->getPosition()[2]);
    p.position += random; //Le sumamos el valor random
    //Reseteamos la vida 
    p.life = 1.0f;
}

//Funcion que actualiza el contenedor de particulas en cada iteracion para producir el ciclo de vida/muerte de las mismas
void particleSystem::update(){
    //Primero, añadimos las nuevas particulas de cada iteracion
    for (int i = 0; i < newParticlesPerIteration; i++){
        rebirthParticle(particulas[findLastDeadParticle()]);
    }
    //Despues, actualizamos todas las particulas del sistema
    particlesAlive = 0; //Reseteamos el numero de particulas vivas
    for (int i = 0; i < numMaxParticles; i++){
        Particula p = particulas[i]; //Recogemos la particula del contenedor
        p.life -= delta; //Reducimos su vida

        if (p.life > 0.0f){ //Si aun sigue viva...
            p.velocity = glm::vec3(0.0f,-9.81f, 0.0f) * delta * 0.5f; //Velocidad en funcion de la gravedad
            p.position += p.velocity * delta; //Modificamos la posicion de la particula en funcion de la velocidad

            //Llenamos los buffers que anteriormente hemos declarado
            // --- POSICION Y TAMANYO ---
            position_data[4*particlesAlive+0] = p.position[0];
            position_data[4*particlesAlive+1] = p.position[1];
            position_data[4*particlesAlive+2] = p.position[2];
            position_data[4*particlesAlive+3] = p.size;
        }

        particlesAlive++; //Aumentamos el numero de particulas vivas
    }
}

//Funcion para dibujar las particulas
void particleSystem::draw(Shader *s){
    //Primero, actualizamos las particulas del contenedor
    update();

    //Despues, pasamos a dibujar
    glBindVertexArray(VAO); //Enlazamos el VAO que antes hemos rellenado

    //Activamos el buffer que guarda la posicion de las particulas
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position); 
    glBufferData(GL_ARRAY_BUFFER, numMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesAlive * sizeof(GLfloat) * 4, position_data); //Lo llenamos con el array que hemos rellenado en la funcion update

    //Le decimos a OpenGL como tratar la informacion de cada buffer
    glVertexAttribDivisor(0, 0); //El atributo 0 (forma base) se utiliza de igual forma para todos
    glVertexAttribDivisor(1, 1); //El atributo 1 (posicion y tamayo) se usa cada vector de 4 por particula o quad (forma base)

    //Dibujamos las particulas -> INSTANCED == se dibuja la misma forma tanta veces como se le diga 
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesAlive); 

    //Desenlazamos el VAO hasta el siguiente dibujado
    glBindVertexArray(0);
}