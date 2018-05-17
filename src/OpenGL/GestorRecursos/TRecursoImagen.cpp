#include "TRecursoImagen.hpp"

//Constructor que carga la imagen a partir de la libreria STB_IMAGE
TRecursoImagen::TRecursoImagen(const char* img){

    //Enlazar la textura con OpenGl para obtener la id
    glGenTextures(1, &idText);
    
    //Para que stb cargue la imagen correctamente
	stbi_set_flip_vertically_on_load(true); 
    
    //Cargar la imagen a partir de la libreria stb
	imgTexture = stbi_load(img, &w, &h, &comp, 4);

    //Guardamos la ruta de la imagen
    imagePath = img;

    //Le decimos a OpenGL que es una textura 2D
	glBindTexture(GL_TEXTURE_2D, idText);
    //Creamos la textura en OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgTexture);
    //Parametros de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Desactivamos la textura hasta el dibujado
	glBindTexture(GL_TEXTURE_2D, 0);

    //Liberamos la memoria reservada por la libreria stb
	stbi_image_free(imgTexture);

}

//Destructor que elimina la texturas de OpenGL de cada recurso imagen
TRecursoImagen::~TRecursoImagen(){
    glDeleteTextures(1, &idText);
}

//====================================
// METODOS GET
//====================================

//Funcion para devolver la imagen cargada por la libreria
unsigned char *TRecursoImagen::getTexture(){
    return imgTexture;
}

//Funcion para devolver la anchura de la imagen
int TRecursoImagen::getWidht(){
    return w;
}

//Funcion para devolver la altura de la imagen
int TRecursoImagen::getHeight(){
    return h;
}

//Funcion para devolver la ruta de la imagen
const char *TRecursoImagen::getPath(){
    return imagePath;
}

//Funcion para devolver la id de la textura de OpenGL
GLuint TRecursoImagen::getID(){
    return idText;
}