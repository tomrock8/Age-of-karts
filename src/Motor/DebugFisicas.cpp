#include "DebugFisicas.hpp"

void DebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	//Se llama a las funciones del motor para guardar los dos puntos y crear sus indices
	TMotor::instancia().setVerticesDebug(from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ());
	TMotor::instancia().setIndexDebug();
}


void DebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
	btScalar distance, int lifeTime, const btVector3 &color)
{
}

void DebugDraw::reportErrorWarning(const char *text)
{
}

void DebugDraw::draw3dText(const btVector3 &location, const char *text)
{
}