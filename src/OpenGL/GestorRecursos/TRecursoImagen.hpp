#ifndef TRECURSOIMAGEN_H
#define TRECURSOIMAGEN_H

#include "libGL.hpp" 

/* Clase para crear los diferentes recursos imagen usados en el hud */
class TRecursoImagen
{
public:
	TRecursoImagen(const char* img); //Constructor del recurso imagen
	~TRecursoImagen(); //Destructor del recurso imagen

	//Metodos GET
	unsigned char* getTexture();
	int getWidht();
	int getHeight();
	const char* getPath();
	GLuint getID();

private:
	const char* imagePath; //Ruta de la imagen

	//Datos que nos proporciona la libreria de carga de imagenes STB_IMAGE
	unsigned char* imgTexture; //Imagen cargada por la libreria
	int w; //Ancho de la imagen
	int h; //Alto de la imagen
	int comp; //Numero de componentes de color de la imagen

	GLuint idText; //Id de la textura al crearla con OpenGL
};
#endif