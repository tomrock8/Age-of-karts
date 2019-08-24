#ifndef SKYBOX_H
#define SKYBOX_H

#include "libGL.hpp"

/*Clase que dibuja un skybox a partir de un cubo y 6 texturas*/

class Skybox {

public:
	Skybox(); //Constructor
	~Skybox(); // Destructor

	unsigned char* loadImage(const char* path, int i);
	void createCubeMap();
	void setupCubeMap();
	void freeImages();
	void setCubeVertices();
	void drawSkyBox();

private:
	//Atributos de cada una de las texturas usadas para crear el skybox
	unsigned char* textures[6]; //Imagenes cargadas a partir de la libreria stb
	int w[6]; //Ancho de las imagenes
	int h[6]; //Alto de las imagenes
	int comp[6]; //Numero de componenetes de color de las imagenes

	GLuint skyboxCubeTexture; //ID de la textura cubo (GL_TEXTURE_CUBE_MAP)
	GLuint VBO, VAO; //Buffers de OpenGl para dibujar el cubo


};

#endif