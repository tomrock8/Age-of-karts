#ifndef CAMARA3PERSONA_H
#define CAMARA3PERSONA_H

#include <cstdlib>
#include <iostream>
#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "Corredor.hpp"
#include "Graphics.hpp"

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
  float XCamera3;
  float YCamera3;
  float ZCamera3;
  float XCamera1;
  float YCamera1;
  float ZCamera1;
  float auxX;
  float auxZ;
};

#endif /* CAMARA3PERSONA_H */