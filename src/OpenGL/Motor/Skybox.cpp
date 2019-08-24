#include "TMotor.hpp"
#include <iostream>
#include "Skybox.hpp"
#include "stb_image.h"
#include "Shader.hpp"
#include "TNodo.hpp"
#include "TEntidad.hpp"

//Constructor de la clase Skybox que se encarga del creado del mismo
Skybox::Skybox() {
	createCubeMap();
	setCubeVertices();
}

Skybox::~Skybox() {

	//Eliminar los distintos buffers utilizados por OpenGL
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &skyboxCubeTexture);
}

//Funcion que carga una imagen a partir de la libreria stb
unsigned char* Skybox::loadImage(const char* p, int i) {
	//Cargamos la imagen, guardando los distintos datos de la misma en la posicion de los arrays que pertoque
	unsigned char* imgLoaded = stbi_load(p, &w[i], &h[i], &comp[i], 4);
	//Devolvemos la imagen cargada
	return imgLoaded;
}

//Funcion que se encarga de inicializar y preparar las texturas que se van a usar en el skybox
void Skybox::createCubeMap() {
	//Cargamos las 6 imagenes distintas que se usaran en cada cara del cubo
	textures[0] = loadImage("assets/skybox/SkyBox_Right.bmp", 0);
	textures[1] = loadImage("assets/skybox/SkyBox_Left.bmp", 1);
	textures[2] = loadImage("assets/skybox/SkyBox_Top.bmp", 2);
	textures[3] = loadImage("assets/skybox/SkyBox_Bottom.bmp", 3);
	textures[4] = loadImage("assets/skybox/SkyBox_Front.bmp", 4);
	textures[5] = loadImage("assets/skybox/SkyBox_Back.bmp", 5);
	//Creamos la textura cubo 
	setupCubeMap();
	//Liberamos la memoria usada en la carga de las imagenes
	freeImages();
}

//Funcion que crea la textura de OpenGL que despues se usara para el skybox
void Skybox::setupCubeMap() {
	//Creamos una textura
	glGenTextures(1, &skyboxCubeTexture);

	//Decimos que esta textura se trata de una de mapa de textura en forma de cubo (6 texturas = una textura por cada cara del cubo)
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeTexture);

	//Generamos las 6 texturas diferentes
	//Para cada lado del cubo, especificado por su direccion en cada, le asignamos una de las imagenes cargadas junto con su respectivo ancho y alto
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, w[0], h[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, w[1], h[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, w[2], h[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, w[3], h[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, w[4], h[4], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, w[5], h[5], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[5]);

	//Establecemos los parametros de cada textura
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Desactivamos el cubemap hasta que volvamos a usarlo
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//Funcion que libera la memoria que ha usado stb para cargar las imagenes, tras generar las texturas de OpenGL ya no hacen falta
void Skybox::freeImages() {
	for (int i = 0; i < 6; i++) {
		stbi_image_free(textures[i]);
	}
}

//Funcion que establece los diferentes datos del cubo del skybox en los buffers
void Skybox::setCubeVertices() {

	//Posicion de los vertices que se usan para crear el cubo
	GLfloat vertices_cubo[] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f
	};

	//Establecemos los buffers que pasaran los datos anteriores a los shaders para su dibujado
	//Creamos un VAO y un VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Activamos el VAO
	glBindVertexArray(VAO);

	//Activamos el VBO, al que se le pasan los datos de posicion de cada vertice
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cubo), vertices_cubo, GL_STATIC_DRAW);

	//POSICION
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//Desactivamos el VAO hasta que dibujemos
	glBindVertexArray(0);
}

//Funcion que dibuja el skybox
void Skybox::drawSkyBox() {
	//Activamos la textura 0
	glActiveTexture(GL_TEXTURE0);

	//Activamos la textura cubo del skybox que hemos generado anteriormente
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeTexture);

	//Calculamos la matriz model del elemento
	glm::mat4 model;

	//Trasladamos el elemento para situarla correctamente
	model = glm::translate(model, glm::vec3(0, -500, 0));

	//Lo escalamos para que englobe totalmente al mapa
	model = glm::scale(model, glm::vec3(5000, 5000, 5000));

	//Calculamos la matriz mvp a partir de la projection y view de la camara activa
	glm::mat4 mvp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getActiveViewMatrix() * model;

	//Pasamos los datos necesarios al shader 
	TMotor::instancia().getShaderSkybox()->setMat4("mvp", mvp); //Matriz mvp
	glUniform1i(glGetUniformLocation(TMotor::instancia().getShaderSkybox()->getID(), "skybox"), 0); //Cubo de texturas
	
																									//Activamos el VAO que hemos seteado antes
	glBindVertexArray(VAO);
	
	//Dibujamos el skybox
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	//Desactivamos el VAO y la textura usados
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}