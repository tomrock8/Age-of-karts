#include "IrrlichtLib.hpp"
#include "Waypoint.hpp"

Waypoint::Waypoint()
{
    Motor3d *m = Motor3d::getInstancia();
    waypoint = m->getScene()->addCubeSceneNode(3.f);
}

void Waypoint::SetPosicion(float x, float y, float z)
{
    waypoint->setPosition(vector3df(x, y, z));
}

vector3df Waypoint::GetPosicion()
{

    return waypoint->getPosition();
}
