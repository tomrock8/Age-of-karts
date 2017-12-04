#ifndef PISTA_H
#define PISTA_H

#include <iostream>
#include "IrrlichtLib.hpp"

class Pista
{
  public:
    Pista(IrrlichtDevice *device);
    ITriangleSelector *setColisiones(IrrlichtDevice *device, ITriangleSelector *selector);

  private:
    IMesh *mapa;
    IMeshSceneNode *mapaNodo;

    bool inicializar(IrrlichtDevice *device);
};

#endif /* PISTA */
