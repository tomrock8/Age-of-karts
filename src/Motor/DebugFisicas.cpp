#include "DebugFisicas.hpp"

void DebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	/*
	SColor newColor(255, color[0] * 255.0, color[1] * 255.0, color[2] * 255.0);
	this->driver->draw3DLine(vector3df(from[0], from[1], from[2]),
		vector3df(to[0], to[1], to[2]), newColor);
	*/

		unsigned int VBO,VAO;
		GLfloat points[12];

		points[0] = from.getX();
		points[1] = from.getY();
		points[2] = from.getZ();
		points[3] = color.getX();
		points[4] = color.getY();
		points[5] = color.getZ();

		points[6] = to.getX();
		points[7] = to.getY();
		points[8] = to.getZ();
		points[9] = color.getX();
		points[10] = color.getY();
		points[11] = color.getZ();

		cout << "UNA LINEA" << endl;
		cout << "Dibujando linea de (" << points[0] <<" , "<< points[1] <<" , "<< points[2] <<") a ("<< points[6] <<" , "<< points[7] <<" , "<< points[8] <<")\n";

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		/*
		TMotor::instancia().getShaderDebug()->use();

		glm::mat4 proje = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix();
		glm::mat4 view = TMotor::instancia().getActiveCamera()->getEntidad()->getViewMatrix();
		cout << view[0][0] <<" - ";
		cout << view[1][1]<<" - ";
		cout << view[2][2]<<" - "; 
		cout << view[3][3]<<" - "<< endl;

		TMotor::instancia().getShaderDebug()->setMat4("projection", proje);
		TMotor::instancia().getShaderDebug()->setMat4("view", view);
		*/
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
}

void DebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
	btScalar distance, int lifeTime, const btVector3 &color)
{
	cout << "PUNTO" << endl;
/*
	static const SColor CONTACTPOINT_COLOR(255, 255, 255, 0);
	const btVector3 to(PointOnB + normalOnB * distance);
	this->driver->draw3DLine(vector3df(PointOnB[0], PointOnB[1], PointOnB[2]),
		vector3df(to[0], to[1], to[2]), CONTACTPOINT_COLOR);
*/		
}

void DebugDraw::reportErrorWarning(const char *text)
{

	//this->logger->log(text, ELL_ERROR);
}

void DebugDraw::draw3dText(const btVector3 &location, const char *text)
{
}