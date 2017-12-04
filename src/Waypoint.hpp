#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <iostream>
#include "IrrlichtLib.hpp"

using namespace std;

class Waypoint
{
  public:
    //Constructor
    Waypoint(ISceneManager *smgr);
    //Destructor
    ~Waypoint();

    //auxiliares

    //Update

    //metodos SET
    void SetPosicion(float x, float y, float z);
    //metodos GET
    IMeshSceneNode *GetWaypoint();
    vector3df GetPosicion();

  private:
    IMeshSceneNode *waypoint;
};

#endif /* WAYPOINT_H */