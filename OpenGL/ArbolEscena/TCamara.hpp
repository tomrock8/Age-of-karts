#ifndef TCAMARA_H
#define TCAMARA_H
#include "libGL.hpp"
#include "TEntidad.hpp"
#include "TNodo.hpp"
#include  <iostream>



using namespace std;

class TCamara : public TEntidad
{
public:
	//-----constructor----//
	TCamara();
	//-----destructor-----//	
	~TCamara();
	//-----matriz de proyeccion---//
	void setPerspectiva(GLfloat Height, GLfloat Widht, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);
	void setParalela(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);
	
	void setPadre(TNodo *n);

	//--Sobrecarga de metodos de dibujado de entidad--//
	virtual void beginDraw() override;
	virtual void endDraw() override;
private:
	bool esPerspectiva;
	GLfloat cercano, lejano, distancia;
	glm::mat4 projMatrix;
	
};
#endif