#include "particleSystem.hpp"
#include "TMotor.hpp"
#include "TRecursoImagen.hpp"
#include "obj3D.hpp"
#include "Shader.hpp"
#include "TGestorImagenes.hpp"

//Constructor que crea un sistema de particulas que siguen a un objeto
particleSystem::particleSystem() {
	//Inicializamos el vector que contendra todas las particulas
	for (int i = 0; i < numMaxParticles; i++) {
		Particula* p = new Particula();
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
	position_data = new GLfloat[4 * numMaxParticles]();
}

//Destructor para eliminar los datos utilizados
particleSystem::~particleSystem() {
	//Eliminar los distintos buffers utilizados por OpenGL
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_position);
}

//Funcion para setear los buffers de OpenGl que se usaran para pasar os datos del sistema del particulas
void particleSystem::setBuffersOpenGL() {

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

	//Desactivamos el VAO hasta el dibujado del quad
	glBindVertexArray(0);
}

//Funcion que devuelve el indice de la primera particula muerta o no usada que encuentre en el contenedor de particulas
int particleSystem::findLastDeadParticle() {
	//Primero, buscamos a partir de la ultima posicion guardada que tengamos
	for (int i = lastParticleFound; i < numMaxParticles; i++) {
		//Si la vida de la particula es menor o igual que 0...
		if (particulas.at(i)->life <= 0.0f) {
			//Guardamos el indice en la variable 
			lastParticleFound = i;
			//Lo devolvemos
			return i;
		}
	}

	//En caso de no encontrar ninguna particula muerta, se realiza una busqueda lineal desde el principio
	for (int i = 0; i < numMaxParticles; i++) {
		//Si la vida de la particula es menor o igual que 0...
		if (particulas.at(i)->life <= 0.0f) {
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
void particleSystem::rebirthParticle(Particula* p) {
	//Establecemos la posicion de la particula en funcion de su posicion y orientacion
	float random = (rand() % 100 - 1) / 50.0f; //Valor random para modificar la posicion de las particulas dentro de un area
	float dist_x = distanceHeight[0] + random; //Distancia en x
	float dist_z = distanceHeight[2] + random; //Distancia en z
	p->position = glm::vec3(posParticle.x + dist_x * oriParticle.x, posParticle.y - distanceHeight[1], posParticle.z + dist_z * oriParticle.z);
	//Reseteamos la vida y el tamaño de la particula
	p->life = 1.0f;
	p->size = 0.65f;
}

//Funcion para seterar la posicion del sistema de particulas
void particleSystem::setPosition(glm::vec3 p) {
	posParticle = p;
}

//Funcion para setear la orientacion del sistema de particulas
void particleSystem::setOrientation(glm::vec3 o) {
	oriParticle = o;
}

//Funcion para setear el tamaño del sistema de particulas
void particleSystem::setSize(float s) {
	sizeParticle = s;
}

//Funcion para setear el color del sistema de particulas
void particleSystem::setColor(glm::vec3 c) {
	colorParticle = c;
}

//Funcion para setear la distancia y altura relativa del sistema de particulas
void particleSystem::setDistanceHeight(glm::vec3 d) {
	distanceHeight = d;
}

//Funcion para setear el tipo de sistema de particulas
void particleSystem::setType(int i) {
	typeParticle = i;
	//Modificamos los parametros segun el tipo de sistema definido
	if (i == 0) {
		reduceLife = 0.1f;
		newParticlesPerIteration = 5;
	}
	else {
		reduceLife = 0.02f;
		newParticlesPerIteration = 6;
	}
}

//Funcion que actualiza el contenedor de particulas en cada iteracion para producir el ciclo de vida/muerte de las mismas
void particleSystem::update() {
	//Primero, añadimos las nuevas particulas de cada iteracion
	for (int i = 0; i < newParticlesPerIteration; i++) {
		rebirthParticle(particulas.at(findLastDeadParticle()));
	}

	//Despues, actualizamos todas las particulas del sistema
	particlesAlive = 0; //Reseteamos el numero de particulas vivas

	for (int i = 0; i < numMaxParticles; i++) {
		Particula* p = particulas.at(i); //Recogemos la particula del contenedor
		p->life -= reduceLife; //Reducimos su vida


		if (p->life > 0.0f) { //Si aun sigue viva...
			p->size += sizeParticle; //Modificamos el tamaño de la particulas con el tiempo
			if (typeParticle == 1) p->position[1] += 0.5; //Si el sistema es de tipo 1, modificamos la 1 de las particulas para que vayan hacia arriba
			//Llenamos los buffers que anteriormente hemos declarado
			// --- POSICION Y TAMANYO ---
			position_data[4 * particlesAlive + 0] = p->position[0];
			position_data[4 * particlesAlive + 1] = p->position[1];
			position_data[4 * particlesAlive + 2] = p->position[2];
			position_data[4 * particlesAlive + 3] = p->size;
		}

		particlesAlive++; //Aumentamos el numero de particulas vivas
	}
}

//Funcion para dibujar las particulas
void particleSystem::draw(Shader* s) {

	//Primero, actualizamos las particulas del contenedor
	update();

	//Activamos la textura 0 y enlazamos la imagen del elemento hud
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TGestorImagenes::getInstancia()->cargarImagen("assets/smoke.png")->getID());
	//Le pasamos la textura al shader
	s->setInt("image", 0);

	//Calculamos la matriz viewProjection y se la pasamos al shader
	glm::mat4 vp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getActiveViewMatrix();
	s->setMat4("vp", vp);

	//Calculamos el vector derecha de la camara y se lo pasamos al shader
	glm::vec3 cR = glm::vec3(TMotor::instancia().getActiveViewMatrix()[0][0], TMotor::instancia().getActiveViewMatrix()[1][0], TMotor::instancia().getActiveViewMatrix()[2][0]);
	s->setVec3("camRight", cR);

	//Calculamos el vector arriba de la camara y se lo pasamos al shader
	glm::vec3 cU = glm::vec3(TMotor::instancia().getActiveViewMatrix()[0][1], TMotor::instancia().getActiveViewMatrix()[1][1], TMotor::instancia().getActiveViewMatrix()[2][1]);
	s->setVec3("camUp", cU);

	//Le pasamos el color de las particulas al shader
	s->setVec3("color", colorParticle);

	//Despues, pasamos a dibujar
	glBindVertexArray(VAO); //Enlazamos el VAO que antes hemos rellenado

	//Activamos el buffer que guarda la posicion de las particulas
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, numMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesAlive * sizeof(GLfloat) * 4, position_data); //Lo llenamos con el array que hemos rellenado en la funcion update

	//Le decimos a OpenGL como tratar la informacion de cada buffer
	glVertexAttribDivisor(0, 0); //El atributo 0 (forma base) se utiliza de igual forma para todos
	glVertexAttribDivisor(1, 0); //El atributo 1 (coordenadas de textura) se utiliza de igual forma para todos
	glVertexAttribDivisor(2, 1); //El atributo 2 (posicion y tamayo) se usa cada vector de 4 por particula o quad (forma base)

	//Dibujamos las particulas -> INSTANCED == se dibuja la misma forma tanta veces como se le diga 
	glDrawArraysInstanced(GL_TRIANGLES, 0, 18, particlesAlive);

	//Desenlazamos el VAO hasta el siguiente dibujado
	glBindVertexArray(0);

}