#ifndef PISTA_H
#define PISTA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"

class Pista
{
  public:
    Pista();
    ITriangleSelector *setColisiones(ITriangleSelector *selector);

  private:
    IMesh *mapa;
    IMeshSceneNode *mapaNodo;

    bool inicializar();
};

#endif /* PISTA */
