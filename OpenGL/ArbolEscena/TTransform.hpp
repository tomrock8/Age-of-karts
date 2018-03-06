	#ifndef TTRANSFORM_H
#define TTRANSFORM_H

#include <iostream>
#include "TEntidad.hpp"

using namespace std;

class TTransform : public TEntidad
{
public:
	//----constructor---//
	TTransform();
	//----destructor----//
	~TTransform();
	//--Gestionar la matriz--//
	void cargar(glm::mat4 mat);				//recoge una matriz y la carga
	//---Transformacion-----//
	void transponer();
	void identidad();						//carga la identidad de la matriz
	void trasladar(GLfloat x, GLfloat y, GLfloat z);
	void rotar(GLfloat x, GLfloat y, GLfloat z, GLfloat grados);
	void escalar(GLfloat x, GLfloat y, GLfloat z);
	glm::mat4 getMatriz();
	//--Sobrecarga de metodos de dibujado de entidad--//
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;
private:
	//TMatriz4x4 matriz
	glm::mat4 matriz;
}; 
#endif

