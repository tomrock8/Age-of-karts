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

    //metodos GET
    IMeshSceneNode *getWaypoint();
    vector3df getPosicion();

  private:
    IMeshSceneNode *waypoint;

    Waypoint *siguiente; // Siguiente punto

};

#endif /* WAYPOINT_H */