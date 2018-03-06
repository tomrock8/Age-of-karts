#ifndef TCAMARA_H
#define TCAMARA_H
#include "libGL.hpp"
#include "TEntidad.hpp"
#include "Shader\Shader.hpp"
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
	void setPerspective(GLfloat Height, GLfloat Widht, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);
	void setParalela(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados);
	//void lookAt(glm::vec3 camaraPos, glm::vec3 camaraDir, glm::vec3 camaraAlt);

	glm::mat4 getProjectionMatrix();
	

	bool getActive();
	void setActive(bool s);
	//--Sobrecarga de metodos de dibujado de entidad--//
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;
private:
	bool active;
	
	GLfloat cercano, lejano, distancia;
	//glm::mat4 projMatrix;
	//glm::mat4 modelView;
};
#endif