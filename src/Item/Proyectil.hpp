#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "Item.hpp"

using namespace std;

class Proyectil : public Item
{
public:
  Proyectil(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,float alturaLanzamiento
  ,int idNodo);
  virtual void lanzarItem(int direccion,btVector3 orientacion,btTransform objeto) override;
  virtual void updateHijos() override;
  virtual void deleteHijos() override;
  void movimiento();
private:

};

#endif