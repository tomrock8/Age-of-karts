#ifndef ACEITE_H
#define ACEITE_H

#include "Estatico.hpp"

using namespace std;

class Aceite : public Estatico
{
  public:
    Aceite(vector3df posicionItem, int idItem);
    //void soltarItem();
    //btRigidBody *inicializarFisicas();

  private:
    //bool colision;
};

#endif

