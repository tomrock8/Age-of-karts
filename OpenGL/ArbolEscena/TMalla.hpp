#ifndef TMALLA_H
#define TMALLA_H
#include "libGL.hpp"
#include <iostream>
#include <list>
#include <vector>
#include "TEntidad.hpp"
#include "TRecursoMalla.hpp"



using namespace std;
//declarado fuera de la clase para poder acceder desde TRecursoMalla


class TMalla : public TEntidad
{
	
public:

	//constructor
	TMalla(TRecursoMalla * malla);
	
	void inicializar();
	//--Sobrecarga de metodos de dibujado de entidad--//
	virtual void beginDraw() override;
	virtual void endDraw() override;
	void draw(Shader shader);

	//variables publicas
	std::vector<vertex> *vertices;//deben ser punteros
	std::vector<GLuint> *indices;
	std::vector<texture> textures;

private:
	
	//variables de renderizado
	GLuint VAO;// Vertex Array Object*/
	GLuint VBO;//|
	GLuint EBO;//|Vertex Buffer Object

	TRecursoMalla * malla; //Puntero al recurso malla ( se recogera en el gestor de recursos)
};
#endif