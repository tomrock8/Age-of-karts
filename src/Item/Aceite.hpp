#ifndef ACEITE_H
#define ACEITE_H

#include "Item.hpp"

using namespace std;

class Aceite : public Item
{
  public:
    Aceite(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,
    float alturaLanzamiento);
    virtual void lanzarItem(int direccion,btVector3 orientacion) override;
    virtual void updateHijos() override;
    //void soltarItem();
    //btRigidBody *inicializarFisicas();

  private:
    //bool colision;
};

#endif

