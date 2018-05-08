#include "TMotor.hpp"

//Constructor que crea un sistema de particulas que siguen a un objeto
particleSystem::particleSystem(){
    //Inicializamos el vector que contendra todas las particulas
    for (int i = 0; i < numMaxParticles; i++){
        Particula *p = new Particula();
        particulas.push_back(p);
    }
    //Establecemos los buffers de OpenGL
    setBuffersOpenGL();
    //Inicializamos el contador a 0 de las distintas variables que utilizaremos
    lastParticleFound = 0;
    particlesAlive = 0;
    //Y el numero de particulas nuevas que habran en cada iteracion
    newParticlesPerIteration = 5;
    //Inicializamos los arrays que haran de buffers con los datos de posicion y transparencia de cada particula
    position_data = new GLfloat[4*numMaxParticles]();
    transparency_data = new GLfloat[numMaxParticles]();
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

    //Coordenadas para crear el quad que contendra cada particula (solo datos de posicion y coordenadas de textura de cada vertice)
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
    glGenBuffers(1, &VBO_position);

    //Enlazar el VAO en el que se guardan los distintos datos
    glBindVertexArray(VAO);

    //Primero, enlazar el VBO al que se le pasan los datos del quad (forma base de cada particula)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);
    //Lo establecemos como el primero atributo del buffer que le llegara al shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Segundo, las coordenadas de textura para cada vertice
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Tercero, enlazar el VBO al que se le pasan los datos de posicion y tamaño de cada particula
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, numMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); //NULL = lo llenaremos en cada iteracion
    //Lo establecemos como el segundo atributo del buffer que le llegara al shader
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    //Cuarto, enlazar el VBO al que se le pasan los datos de transparencia de cada particula
    glBindBuffer(GL_ARRAY_BUFFER, VBO_transparency);
    glBufferData(GL_ARRAY_BUFFER, numMaxParticles * sizeof(GLubyte), NULL, GL_STREAM_DRAW); //NULL = lo llenaremos en cada iteracion
    //Lo establecemos como el tercer atributo del buffer que le llegara al shader
    glVertexAttribPointer(3, 1, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    //Desactivamos el VAO hasta el dibujado del quad
    glBindVertexArray(0); 
}

//Funcion que devuelve el indice de la primera particula muerta o no usada que encuentre en el contenedor de particulas
int particleSystem::findLastDeadParticle(){
    //Primero, buscamos a partir de la ultima posicion guardada que tengamos
    for (int i = lastParticleFound; i < numMaxParticles; i++){
        //Si la vida de la particula es menor o igual que 0...
        if (particulas.at(i)->life <= 0.0f){
            //Guardamos el indice en la variable 
            lastParticleFound = i;
            //Lo devolvemos
            return i;
        }
    }

    //En caso de no encontrar ninguna particula muerta, se realiza una busqueda lineal desde el principio
    for (int i = 0; i < numMaxParticles; i++){
        //Si la vida de la particula es menor o igual que 0...
        if (particulas.at(i)->life <= 0.0f){
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
void particleSystem::rebirthParticle(Particula *p){
    //Parametros de cada particula
    float dist = -7.0f; //Distancia donde se moveran las particulas
    float altura = -1.0f; //Altura donde se situaran las particulas
    //Establecemos la posicion de la particula en funcion de su posicion y orientacion
    float random = (rand() % 100 - 1) / 50.0f; //Valor random para modificar la posicion de las particulas dentro de un area
    dist += random; //Se lo sumamos a la distancia
    p->position = glm::vec3(posParticle.x + dist * oriParticle.x, posParticle.y - altura, posParticle.z + dist * oriParticle.z);
    //Reseteamos la vida y el tamaño de la particula
    p->life = 1.0f;
    p->size = 0.65f;
}

//Funcion para seterar la posicion del sistema de particulas
void particleSystem::setPosition(glm::vec3 p){
    posParticle = p;
}

//Funcion para setear la orientacion del sistema de particulas
void particleSystem::setOrientation(glm::vec3 o){
    oriParticle = o;
}

//Funcion para setear el tamaño del sistema de particulas
void particleSystem::setSize(float s){
    sizeParticle = s;
}

//Funcion para setear el color del sistema de particulas
void particleSystem::setColor(glm::vec3 c){
    colorParticle = c;
}

//Funcion que actualiza el contenedor de particulas en cada iteracion para producir el ciclo de vida/muerte de las mismas
void particleSystem::update(){
    //Primero, añadimos las nuevas particulas de cada iteracion
    for (int i = 0; i < newParticlesPerIteration; i++){
        rebirthParticle(particulas.at(findLastDeadParticle())); 
    }
    
    //Despues, actualizamos todas las particulas del sistema
    particlesAlive = 0; //Reseteamos el numero de particulas vivas

    glm::mat4 cameraMatrix = TMotor::instancia().getV();
    glm::vec4 defaultVector(0, 0, 0, 1);
    glm::vec3 posCamera = glm::vec3(cameraMatrix * defaultVector);

    for (int i = 0; i < numMaxParticles; i++){
        Particula *p = particulas.at(i); //Recogemos la particula del contenedor
        p->life -= 0.1; //Reducimos su vida


        if (p->life > 0.0f){ //Si aun sigue viva...
            p->size += sizeParticle; //Modificamos el tamaño de la particulas con el tiempo
            p->distanceToCamera = glm::length2(p->position - posCamera);
            //Llenamos los buffers que anteriormente hemos declarado
            // --- POSICION Y TAMANYO ---
            position_data[4*particlesAlive+0] = p->position[0];
            position_data[4*particlesAlive+1] = p->position[1];
            position_data[4*particlesAlive+2] = p->position[2];
            position_data[4*particlesAlive+3] = p->size;
            // --- TRANSPARENCIA ---
            transparency_data[particlesAlive] = (rand() % 10 - 1) / 10.f;

        }else{
            p->distanceToCamera = glm::length2(p->position - posCamera);
        }

        particlesAlive++; //Aumentamos el numero de particulas vivas
    }
}

//Funcion para dibujar las particulas
void particleSystem::draw(Shader *s){
    
    //Primero, actualizamos las particulas del contenedor
    update();

    //Activamos la textura 0 y enlazamos la imagen del elemento hud
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TGestorImagenes::getInstacia()->cargarImagen("assets/smoke.png")->getID());
    //Le pasamos la textura al shader
    s->setInt("image", 0);

    //Calculamos la matriz viewProjection y se la pasamos al shader
    glm::mat4 vp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getV();
    s->setMat4("vp", vp);

    //Calculamos el vector derecha de la camara y se lo pasamos al shader
    glm::vec3 cR = glm::vec3(TMotor::instancia().getV()[0][0], TMotor::instancia().getV()[1][0], TMotor::instancia().getV()[2][0]);
    s->setVec3("camRight", cR);

    //Calculamos el vector arriba de la camara y se lo pasamos al shader
    glm::vec3 cU = glm::vec3(TMotor::instancia().getV()[0][1], TMotor::instancia().getV()[1][1], TMotor::instancia().getV()[2][1]);
    s->setVec3("camUp", cU);

    //Le pasamos el color de las particulas al shader
    s->setVec3("color", colorParticle);

    /*
    for (int i = 0; i < particulas.size()-1; i++){
        Particula *minP = particulas.at(i);
        int aux = 0;
        for (int j = i + 1; j < particulas.size()-1; j++){
            float dMin = minP->distanceToCamera;
            float dActual = particulas.at(j)->distanceToCamera;
            if (dMin < dActual){
                minP = particulas.at(j);
                aux = j;
            }
        }
        glm::vec4 posAux = glm::vec4(position_data[4*i+0],position_data[4*i+1],position_data[4*i+2],position_data[4*i+3]);
        Particula *p = particulas.at(i);
        particulas[i] = minP;
        position_data[4*i+0] = position_data[4*aux+0];
        position_data[4*i+1] = position_data[4*aux+1];
        position_data[4*i+2] = position_data[4*aux+2];
        position_data[4*i+3] = position_data[4*aux+3];
        particulas[aux] = p;
        position_data[4*aux+0] = posAux.x;
        position_data[4*aux+1] = posAux.y;
        position_data[4*aux+2] = posAux.z;
        position_data[4*aux+3] = posAux.w;
        cout << "distancia : " << particulas.at(i)->distanceToCamera << endl;
    }
    cout << "-----------------------" << endl;*/

    //Despues, pasamos a dibujar
    glBindVertexArray(VAO); //Enlazamos el VAO que antes hemos rellenado

    //Activamos el buffer que guarda la posicion de las particulas
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position); 
    glBufferData(GL_ARRAY_BUFFER, numMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesAlive * sizeof(GLfloat) * 4, position_data); //Lo llenamos con el array que hemos rellenado en la funcion update
    
    //Activamos el buffer que guarda la transparencia de las particulas
    glBindBuffer(GL_ARRAY_BUFFER, VBO_transparency); 
    glBufferData(GL_ARRAY_BUFFER, numMaxParticles * sizeof(GLubyte), NULL, GL_STREAM_DRAW); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesAlive * sizeof(GLubyte), transparency_data); //Lo llenamos con el array que hemos rellenado en la funcion update

    //Le decimos a OpenGL como tratar la informacion de cada buffer
    glVertexAttribDivisor(0, 0); //El atributo 0 (forma base) se utiliza de igual forma para todos
    glVertexAttribDivisor(1, 0); //El atributo 1 (coordenadas de textura) se utiliza de igual forma para todos
    glVertexAttribDivisor(2, 1); //El atributo 2 (posicion y tamayo) se usa cada vector de 4 por particula o quad (forma base)
    glVertexAttribDivisor(3, 1); //El atributo 3 (transparencia) se usa cada valor float por particula o quad (forma base)
    
    //Dibujamos las particulas -> INSTANCED == se dibuja la misma forma tanta veces como se le diga 
    glDrawArraysInstanced(GL_TRIANGLES, 0, 18, particlesAlive); 

    //Desenlazamos el VAO hasta el siguiente dibujado
    glBindVertexArray(0);
    
}