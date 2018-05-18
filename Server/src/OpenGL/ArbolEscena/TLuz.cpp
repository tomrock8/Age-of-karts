#include "TMotor.hpp"

/*
COMPONENTES DE LUZ:
	Ambiente: La luz que llega rebota en los objetos. La luz mas debil
	Difusa: La luz que llega directamente desde la fuente de luz, y rebota en todas las direcciones
	Especular: La luz que llega directamente desde la fuente de luz y rebota en una direccion(Brillo)
	Posicion: Posicion del punto de luz en el espacio
	Direccion: 	En caso de tratarse de una luz dirigida, la direccion del foco de la luz. Por ejemplo
				si esta apuntado al eje z negativo seria (0,0,-1)
	Atenuacion: Variable de control de la atenuacion	
				Cuanta mas bajo, mas distancia que engloba la luz, por lo tanto menos atenuacion
				Valores recomendados entre 0.025 y 0.005
	Corte: En caso de tratarse de una luz dirigida, coseno del angulo que define el radio del circulo del foco


	Color final del objeto = Ambiente + Difusa + Especular
*/

/*
	Para definir una luz puntual, hay que pasarle un vector direccion todo 0 y un corte igual a 0
	Para definir una luz dirigida, hay que pasarle el vector direccion con la componente w siendo igual a 1 y un corte determinado
*/

TLuz::TLuz() {
	ambiente = glm::vec3(0.2f, 0.2f, 0.2f); //Valor recomendado 0.2f para no saturar la escena
	difusa = glm::vec3(0.5f, 0.5f, 0.5f);
	especular = glm::vec3(0.8f, 0.8f, 0.8f);
	
	//Atenuacion de la luz -> cuanta mas bajo, mas distancia, menos atenuacion
	//Valores recomendados -> entre 0.025 y 0.005
	attenuation = 0.01f; 
}

TLuz::TLuz(glm::vec3 ambiente, glm::vec3 difusa, glm::vec3 especular, glm::vec4 dir, float at, float corte, bool shadow, bool on) {
	this->lightDirection = dir;
	this->ambiente = ambiente;
	this->difusa = difusa;
	this->especular = especular;
	this->attenuation = at;
	this->cut = corte;
	this->sombras = shadow;
	this->active = on;
	//Llamamos a la funcion para crear el mapa de profundidad diferenciando segun sea una luz puntual o dirigida
	if (dir[3] < 0.1){
		calculateCubeDepthMap();
	}else{
		calculateDepthMap();
	}
}

TLuz::~TLuz(){
	//std::cout << "Destructor de TLuz\n";
	lightMatrixes.clear(); // Vaciar el vector
}

// METODOS SET
void TLuz::setLightPosition(glm::vec3 pos) { lightPosition = pos; }
void TLuz::setActive(bool active) { this->active = active; }

// METODOS GET
bool TLuz::getActive() { return active; }
float TLuz::getLightType() { return lightDirection[3]; }
glm::vec3 TLuz::getPosition(){ return lightPosition; }
glm::vec4 TLuz::getDirection(){ return lightDirection; }

//------------------------------------------//
//---------------SET DE LUCES---------------//
//------------------------------------------//
void TLuz::setLuz(Shader *shader, const char *nombre) {
	string name;
	if (lightDirection[3] < 0.1){
		name = "point_lights[";
	}else{
		name = "spot_lights[";
	} 
	//Recoger el numero de luz actual
	char num = nombre[6];
	name += num;
	name += "].";

	numLight = num;

	//Pasar los distintos datos al shader
	shader->setVec3( name + "position", lightPosition);
	shader->setVec4( name + "direction", lightDirection);
	shader->setVec3( name + "ambient", ambiente);
	shader->setVec3( name + "diffuse", difusa);
	shader->setVec3( name + "specular", especular);
	shader->setFloat( name + "k", attenuation);
	shader->setFloat( name + "cut", cut);
	shader->setBool( name + "sombra", sombras);
	shader->setBool( name + "activa", active);
}

/*La posicion y orientacion de la camara y de las luces se maneja a traves de
las transformaciones de nodo padre, como para el resto de entidades.
Las camaras y las luces se manejan de forma especial,
puesto que se pueden encontrar en cualquier nodo del arbol y, sin embargo,
al dibujar la escena deben definirse antes de dibujar el resto de entidades.
Los metodos beginDraw y endDraw de las camaras y las luces estan vacios.*/
void TLuz::beginDraw(Shader *shader) { }
void TLuz::endDraw() { }

//===========================================
//CALCULO DE LAS SOMBRAS
//===========================================

//Funcion para renderizar el mapa de profundidad
void TLuz::renderMap(){
	//Segun sea puntual o dirigida, se renderiza un cubo o un simple mapa
	if (lightDirection[3] < 0.1){
		renderCubeDepthMap();
		
	}else{
		renderDepthMap();
	}
}

//Funcion para desenlazar el buffer del mapa de profundidad
void TLuz::unbindDepthBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Funcion para configurar los datos del shader una vez renderizado el mapa de profundidad
//y calcular asi las sombras
void TLuz::configureShadow(Shader *s){
	//Segun sea puntual o dirigida, se pasan unos datos
	if (lightDirection[3] < 0.1){
		configurePointShadow(s);
	}else{
		configureDirectionalShadow(s);
	}
}

// ---- LUZ DIRIGIDA ----

//Funcion para preparar la creacion del mapa de profundidad
void TLuz::calculateDepthMap(){
	//Creamos un framebuffer para renderizar el mapa de profundidad
	glGenFramebuffers(1, &depthMap);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMap); //Activamos el framebuffer del mapa de profundidad

	//Preparamos la textura que recogera el mapa de profundidad
	glGenTextures(1, &depthTexture); //Una sola textura
	glBindTexture(GL_TEXTURE_2D, depthTexture); //Se trata de una textura 2D
	//Especificamos que la textura solo va a recoger valores de profundidad (GL_DEPTH_COMPONENT -16 bits- )
	//y que va a tener el ancho y alto especificados en el header
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 
				DEPTH_WIDTH, DEPTH_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//Parametros de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//Establecemos un borde con todos a 1.0 para que todas las coordenadas fuera del mapa de profundidad
	//tengan una profundidad de 1.0f, es decir, no estan en sombra
	float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border); 

	//Finalmente, enlazamos la textura con el framebuffer creado al principio
	//Enlazamos textura y buffer especificando que se va a utilizar para recoger el componente de profundidad
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	//Le decimos a OpenGL que no queremos informacion del color
	glDrawBuffer(GL_NONE);  glReadBuffer(GL_NONE);
	//Comprobar si el framebuffer funciona correctamente
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
 	cout << "Framebuffer no funciona!" << endl;
	//Desactivamos el framebuffer hasta que volvamos a usarlo en el draw de la escena
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

//Funcion que calcular la matriz de luz necesaria para calcular el mapa de profundidad
void TLuz::calculateLightMatrix(){
	//Calculamos la matriz proyeccion = no necesitamos perspectiva ya que la luz es dirigida y los rayos son paralelos
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)DEPTH_WIDTH/(float)DEPTH_HEIGHT, 2.0f, 50.0f);
	int nZeros = 0;
	glm::vec3 lDir;
	if (lightDirection.x == 0.0f) nZeros++;
	if (lightDirection.y == 0.0f) nZeros++;
	if (lightDirection.z == 0.0f) nZeros++;
	if (nZeros == 2){
		lDir = glm::vec3(lightDirection);
	}else{
		lDir = glm::vec3(lightPosition + glm::vec3(lightDirection));
	}
	//Matriz view de la luz a partir de la posicion y la direccion de la luz puntual
	glm::mat4 view = glm::lookAt(lightPosition, glm::vec3(lDir), glm::vec3( 0.0f, 1.0f,  0.0f));  

	//Matriz de luz = multiplicacion de las dos anteriores
	lightMatrix = projection * view;
							
}

//Funcion para renderizar el mapa de profundidad de una luz dirigida
void TLuz::renderDepthMap(){
	//Calculamos la matriz de luz
	calculateLightMatrix();
	//Establecemos el viewport a las medidas del mapa de profundidad
	glViewport(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
	//Activamos el buffer del mapa de profundidad
	glBindFramebuffer(GL_FRAMEBUFFER, depthMap);
	//Limpiar los buffers de color y profundidad de Opengl	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Pasamos la matriz de luz al shader
	TMotor::instancia().getShaderDirectionalDepth()->use();
	TMotor::instancia().getShaderDirectionalDepth()->setMat4("lightMatrix", lightMatrix);
}

//Funcion que configura los datos del shader para una luz dirigida
void TLuz::configureDirectionalShadow(Shader *s){
	//Pasamos la matriz de luz al shader
	int text = numLight - '0'; //Numero de luz dirigida (0-3)
	string l = "spot_lights[";
	l += numLight;
	l += "].lightMatrix";
	s->setMat4(l.c_str(), lightMatrix);
	//Activamos la textura para el mapa de profundidad
	glActiveTexture(GL_TEXTURE8 + text);
	//Enlazamos la textura con el mapa de profundidad a partir de su id anteriormente creado
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	//Le pasamos al shader la textura con el mapa de profundidad
	string n = "shadowMap_";
	n+=numLight;
	glUniform1i(glGetUniformLocation(s->ID, n.c_str()), 8 + text);
}

// ----- LUZ PUNTUAL -----

//Funcion que calcular un cubo de mapas de profundidad, es decir, calcula 6 mapas de profundidad (uno para cada lado del cubo)
void TLuz::calculateCubeDepthMap(){
	//Creamos un framebuffer para renderizar el cubo de profundidad
	glGenFramebuffers(1, &depthCubeMap);
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMap); //Activamos el framebuffer del cubo de profundidad

	//Preparamos la textura que recogera el cubo de profundidad
	glGenTextures(1, &depthCubeTexture); //Una sola textura para todo el cubo
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeTexture); //Le decimos a OpenGL que se va a autilizar para generar las texturas de un cubo de profundidad
	//Como son 6 caras por cubo, recorremos un bucle espcificando las caracteristicas que cada cara
	for (int i = 0; i < 6; i++){
		//Especificamos que cada textura solo va a recoger valores de profundidad (GL_DEPTH_COMPONENT -16 bits- )
		//y que va a tener el ancho y alto especificados en el header
		//GL_TEXTURE_CUBE_MAP_POSITIVE_X = cara del cubo en la direccion del eje x positivo (devuelve 0 como entero) ->se
		//va incrementando con el bucle para ir pasando a las sucesivas caras
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, DEPTH_WIDTH, DEPTH_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);	
	}
	//Parametros de la textura
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Finalmente, enlazamos la textura con el framebuffer creado al principio
	//Enlazamos textura y buffer especificando que se va a utilizar para recoger el componente de profundidad
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeTexture, 0);
	//Le decimos a OpenGL que no queremos informacion del color
	glDrawBuffer(GL_NONE);  glReadBuffer(GL_NONE);
	//Comprobar si el framebuffer funciona correctamente
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
 	cout << "Framebuffer no funciona!" << endl;
	//Desactivamos el framebuffer hasta que volvamos a usarlo en el draw de la escena
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

//Funcion para calcular las matrices de luz del cubo de profundidad
void TLuz::calculateLightMatrixes(){
	//Calculamos la matriz proyeccion = esta vez se usa la perspectiva ya que al ser luz puntual la luz sale en todas las direcciones
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)DEPTH_WIDTH/(float)DEPTH_HEIGHT, 0.1f, 200.0f);

	//Calculamos la matriz view a partir de la posicion de la luz (una para cada lado del cubo)
	lightMatrixes.clear();
	// --- Cara en el eje x positivo ---
	glm::mat4 view = glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightMatrixes.push_back(projection * view);
	// --- Cara en el eje x negativo ---
	view = glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightMatrixes.push_back(projection * view);
	// --- Cara en el eje y positivo ---
	view = glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	lightMatrixes.push_back(projection * view);
	// --- Cara en el eje y negativo ---
	view = glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	lightMatrixes.push_back(projection * view);
	// --- Cara en el eje z positivo ---
	view = glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightMatrixes.push_back(projection * view);
	// --- Cara en el eje z negativo ---
	view = glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightMatrixes.push_back(projection * view);

}

//Funcion para renderizar el cubo de profundidad de una luz puntual
void TLuz::renderCubeDepthMap(){
	//Establecemos el viewport a las medidas del cubo de profundidad
	glViewport(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
	//Calculamos las matrices de luz para cada lado del cubo
	calculateLightMatrixes();
	//Activamos el buffer del cubo de profundidad
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMap);
	//Limpiar los buffers de color y profundidad de Opengl	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Pasamos las distintas matrices de luz al shader
	TMotor::instancia().getShaderPointDepth()->use();
	TMotor::instancia().getShaderPointDepth()->setMat4("lightMatrix[0]", lightMatrixes.at(0));
	TMotor::instancia().getShaderPointDepth()->setMat4("lightMatrix[1]", lightMatrixes.at(1));
	TMotor::instancia().getShaderPointDepth()->setMat4("lightMatrix[2]", lightMatrixes.at(2));
	TMotor::instancia().getShaderPointDepth()->setMat4("lightMatrix[3]", lightMatrixes.at(3));
	TMotor::instancia().getShaderPointDepth()->setMat4("lightMatrix[4]", lightMatrixes.at(4));
	TMotor::instancia().getShaderPointDepth()->setMat4("lightMatrix[5]", lightMatrixes.at(5));
	TMotor::instancia().getShaderPointDepth()->setVec3("lightPosition", lightPosition);
	TMotor::instancia().getShaderPointDepth()->setFloat("planoLejano", 200.0f);

}

//Funcion que configura los datos del shader para una luz puntual
void TLuz::configurePointShadow(Shader *s){
	int text = numLight - '0'; //Numero de luz puntual (0-3)
	//Activamos la textura para el cubo de profundidad
	glActiveTexture(GL_TEXTURE3 + text);
	//Enlazamos la textura con el mapa de profundidad a partir de su id anteriormente creado
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeTexture);
	//Le pasamos al shader la textura con el mapa de profundidad
	string n = "shadowCube_";
	n+=numLight;
	glUniform1i(glGetUniformLocation(s->ID, n.c_str()), 3 + text);
	//Pasamo el plano lejano al shader
	s->setFloat("planoLejano", 200.0f);
}