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

TLuz::TLuz(glm::vec3 ambiente, glm::vec3 difusa, glm::vec3 especular, glm::vec4 dir, float at, float corte) {
	this->lightDirection = dir;
	this->ambiente = ambiente;
	this->difusa = difusa;
	this->especular = especular;
	this->attenuation = at;
	this->cut = corte;
	//Llamamos a la funcion para crear el mapa de profundidad
	calculateDepthMap();
}

// METODOS SET
void TLuz::setIntensidad(glm::vec4 color) { TColor = color; }
void TLuz::setLightPosition(glm::vec3 pos) { lightPosition = pos; }
void TLuz::setActive(bool active) { this->active = active; }

// METODOS GET
glm::vec4 TLuz::getIntensidad() { return TColor; }
bool TLuz::getActive() { return active; }

//------------------------------------------//
//---------------SET DE LUCES---------------//
//------------------------------------------//
void TLuz::setLuzPuntual(Shader *shader, const char *nombre) {
	//Recoger el numero de luz actual
	char num = nombre[6];
	string name = "light[";
	name += num;
	name += "].";

	//Pasar los distintos datos al shader
	shader->setVec3( name + "position", lightPosition);
	shader->setVec4( name + "direction", lightDirection);
	shader->setVec3( name + "ambient", ambiente);
	shader->setVec3( name + "diffuse", difusa);
	shader->setVec3( name + "specular", especular);
	shader->setFloat( name + "k", attenuation);
	shader->setFloat( name + "cut", cut);
}

/*La posicion y orientacion de la camara y de las luces se maneja a traves de
las transformaciones de nodo padre, como para el resto de entidades.
Las camaras y las luces se manejan de forma especial,
puesto que se pueden encontrar en cualquier nodo del arbol y, sin embargo,
al dibujar la escena deben definirse antes de dibujar el resto de entidades.
Los metodos beginDraw y endDraw de las camaras y las luces estan vacios.*/
void TLuz::beginDraw(Shader *shader) { }
void TLuz::endDraw() { }


//CALCULO DE LAS SOMBRAS

//Funcion para preparar la creacion del mapa de profundidad
void TLuz::calculateDepthMap(){
	//Creamos un framebuffer para renderizar el mapa de profundidad
	glGenFramebuffers(1, &depthMap);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMap); //Activamos el framebuffer del mapa de profundidad

	//Preparamos la textura que recogera el mapa de profundidad
	glGenTextures(1, &depthTexture); //Una sola textura
	glBindTexture(GL_TEXTURE_2D, depthTexture); //Se trata de una textura 2D
	//Especificamos que la textura solo va a recoger valores de profundidad (GL_DEPTH_COMPONENT)
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
	//Calculamos la matriz proyeccion = no necesitamos perspectiva ya que la luz es puntual y los rayos son paralelos
	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 50.5f);

	//Matriz view de la luz a partir de la posicion y la direccion de la luz puntual
	glm::mat4 view = glm::lookAt(lightPosition, glm::vec3(lightDirection), glm::vec3( 0.0f, 1.0f,  0.0f));  

	//Matriz de luz = multiplicacion de las dos anteriores
	lightMatrix = projection * view;
							
}

//Funcion para renderizar el mapa de profundidad -> se llama desde el draw del motor
void TLuz::renderDepthMap(){
	//Establecemos el viewport a las medidas del mapa de profundidad
	glViewport(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
	//Calculamos la matriz de luz
	calculateLightMatrix();
	//Activamos el buffer del mapa de profundidad
	glBindFramebuffer(GL_FRAMEBUFFER, depthMap);
	//Limpiar los buffers de color y profundidad de Opengl	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Pasamos la matriz de luz al shader
	TMotor::instancia().getShaderDepth()->use();
	TMotor::instancia().getShaderDepth()->setMat4("lightMatrix", lightMatrix);
}

//Funcion para desenlazar el buffer del mapa de profundidad
void TLuz::unbindDepthBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Funcion para configurar los datos del shader una vez renderizado el mapa de profundidad
//y calcular asi las sombras
void TLuz::configureShadow(){
	//Pasamos la matriz de luz al shader
	TMotor::instancia().getShader()->setMat4("lightMatrix", lightMatrix);
	//Activamos la textura para el mapa de profundidad
	glActiveTexture(GL_TEXTURE0);
	//Enlazamos la textura con el mapa de profundidad a partir de su id anteriormente creado
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	//Le pasamos al shader la textura con el mapa de profundidad
	glUniform1i(glGetUniformLocation(TMotor::instancia().getShader()->ID, "depthMap"), 0);
}