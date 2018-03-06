#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "Item.hpp"

using namespace std;

class Proyectil : public Item
{
public:
  Proyectil(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,float alturaLanzamiento);
  virtual void lanzarItem(int direccion,btVector3 orientacion) override;
  virtual void updateHijos() override;
private:

};

#endif