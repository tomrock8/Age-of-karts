#include "DebugFisicas.hpp"

void DebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	/*
	SColor newColor(255, color[0] * 255.0, color[1] * 255.0, color[2] * 255.0);
	this->driver->draw3DLine(vector3df(from[0], from[1], from[2]),
		vector3df(to[0], to[1], to[2]), newColor);
	*/

	cout << from.getX() <<" - " << from.getY() << " - " << from.getZ() << endl;
	/*vector<GLfloat> vertices;
    vector<GLuint> indices;
    unsigned int indexI = 0;
 
    for (vector<PhysicsDebugDraw::LINE>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        PhysicsDebugDraw::LINE l = (*it);
        vertices.push_back(l.a.x);
        vertices.push_back(l.a.y);
        vertices.push_back(l.a.z);
 
        vertices.push_back(l.b.x);
        vertices.push_back(l.b.y);
        vertices.push_back(l.b.z);
 
        indices.push_back(indexI);
        indices.push_back(indexI + 1);
        indexI += 2;
    }*/
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