#ifndef ESCUDO_H
#define ESCUDO_H

#include "Item.hpp"


using namespace std;

class Escudo : public Item
{
public:
  Escudo(obj3D* n,btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
  float alturaLanzamiento,int idNodo);
  ~Escudo();
  virtual void lanzarItem(int direccion,btVector3 orientacion) override;
  virtual void updateHijos() override;
  virtual void deleteHijos() override;
  void movimiento();

private:


obj3D* NodoVehiculo;

};

#endif