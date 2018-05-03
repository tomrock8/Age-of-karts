#ifndef TCAMARA_H
#define TCAMARA_H

#include "libGL.hpp"
#include "TEntidad.hpp"
#include "Shader.hpp"

class TCamara : public TEntidad
{
public:
	TCamara();
	~TCamara();

	// METODOS GET
	bool getActive();

	// METODOS SET
	void setActive(bool s);
	// matriz de proyeccion
	void setPerspective(GLfloat Height, GLfloat Widht, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);
	void setParalela(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);

	// DIBUJADO
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;

private:
	bool active;
	GLfloat cercano, lejano, distancia;
};
#endif