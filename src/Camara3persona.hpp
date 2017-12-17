#ifndef CAMARA3PERSONA_H
#define CAMARA3PERSONA_H

#include <cstdlib>
#include <iostream>
#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "Corredor.hpp"

class Camara3persona
{
public:
  Camara3persona();
  ~Camara3persona();
  void moveCameraControl(Corredor *pj1, IrrlichtDevice *device);

private:
  float direction;
  float zdirection;
  ICameraSceneNode *camera;
};

#endif /* CAMARA3PERSONA_H */