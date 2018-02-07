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
  void moveCamera(Corredor *pj1);
  void movefpsCamera(Corredor *pj1);
  void moveCameraControl(Corredor *pj1);
  void moveCameraControlPointer(Corredor *pj1);

private:
  float direction;
  float zdirection;
  ICameraSceneNode *camera;
  bool fpsActiva;
  float XCamera;
  float YCamera;
  float ZCamera;
};

#endif /* CAMARA3PERSONA_H */