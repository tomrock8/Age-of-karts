#ifndef TTRANSFORM_H
#define TTRANSFORM_H

#include "TEntidad.hpp"

class TTransform : public TEntidad {
public:
	TTransform();
	~TTransform();

	// Gestionar la matriz
	void cargar(glm::mat4 mat);	//recoge una matriz y la carga
	glm::mat4 getMatriz();

	void setMatriz(glm::mat4 p);

	// TRANSFORMACIONES
	void identidad();	//carga la identidad de la matriz
	void trasladar(GLfloat x, GLfloat y, GLfloat z);
	void rotar(GLfloat x, GLfloat y, GLfloat z, GLfloat grados);
	void escalar(GLfloat x, GLfloat y, GLfloat z);

	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setRotation(GLfloat x, GLfloat y, GLfloat z, GLfloat g);
	void setScale(GLfloat x, GLfloat y, GLfloat z);

	// DIBUJADO
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;

private:
	glm::mat4 matriz;

};
#endif

