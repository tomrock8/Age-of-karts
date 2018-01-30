#ifndef CAJAFALSA_H
#define CAJAFALSA_H

#include "Estatico.hpp"

using namespace std;

class CajaFalsa : public Estatico
{
  public:
    CajaFalsa(btVector3 posicionItem);
    //void soltarItem();
    //btRigidBody *inicializarFisicas();

  private:
    //bool colision;
};

#endif

