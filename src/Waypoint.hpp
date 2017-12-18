#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"

using namespace std;

class Waypoint
{
  public:
    //Constructor
    Waypoint();
    //Destructor
    ~Waypoint();

    //auxiliares

    //Update

    //metodos SET
    void setPosicion(float x, float y, float z);
    void setSiguiente(Waypoint *siguiente);
    void setNombre(std::string nombre);
    //metodos GET
    IMeshSceneNode *getWaypoint();
    vector3df getPosicion();
    Waypoint *getNextWaypoint();
    stringw getNombre();
  private:
    IMeshSceneNode *waypoint;
    stringw nombre;
    Waypoint *siguiente; // Siguiente punto

};

#endif /* WAYPOINT_H */