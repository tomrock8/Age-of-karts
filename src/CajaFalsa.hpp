#ifndef CAJAFALSA_H
#define CAJAFALSA_H

#include "Estatico.hpp"

using namespace std;

class CajaFalsa : public Estatico
{
  public:
    CajaFalsa(vector3df posicionItem, int idItem);
    //void soltarItem();
    //btRigidBody *inicializarFisicas();

  private:
    //bool colision;
};

#endif

