#ifndef CLOUDGENERATOR_H
#define CLOUDGENERATOR_H

class Shader;

/* Clase que crea un area cubica, especificando ancho, alto y largo,
	donde crea un numero determinado de nubes, situandolas
	aleatoriamente en dicha area */
class cloudGenerator {
public:
	cloudGenerator(int minW, int maxW, int minH, int maxH, int minL, int maxL, int numC); //Constructor
	void draw(Shader* s); //Dibujado

private:
	// --- VARIABLES ---
	int numClouds; //Numero de nubes creadas en el area de 
	std::vector<glm::vec3> positionClouds; //Array para guardar la posicion de cada nube
	GLuint VAO, VBO; //Buffers de OpenGL

	// --- METODOS ---
	void setCloudBuffers();

};

#endif
