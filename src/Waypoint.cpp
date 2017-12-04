#include "IrrlichtLib.hpp"
#include "Waypoint.hpp"

Waypoint::Waypoint()
{
    Motor3d *m = Motor3d::getInstancia();
    waypoint = m->getScene()->addCubeSceneNode(0.1f);
    waypoint->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
    m->getScene()->getMeshManipulator()->setVertexColors(waypoint->getMesh(), SColor(255, 200, 100, 100));
    waypoint->setScale(vector3df(1, 1000, 1));

    siguiente = NULL; // Referenciar despues con la funcion
}

void Waypoint::setPosicion(float x, float y, float z)
{
    waypoint->setPosition(vector3df(x, y, z));
}

vector3df Waypoint::getPosicion()
{
    return waypoint->getPosition();
}

void Waypoint::setSiguiente(Waypoint *siguiente){
    this->siguiente = siguiente;
}