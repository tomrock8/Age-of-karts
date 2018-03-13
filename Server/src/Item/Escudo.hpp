#ifndef ESCUDO_H
#define ESCUDO_H

#include "Item.hpp"


using namespace std;

class Escudo : public Item
{
public:
  Escudo(ISceneNode* n,btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,
  float alturaLanzamiento);
  ~Escudo();
  virtual void lanzarItem(int direccion,btVector3 orientacion) override;
  virtual void updateHijos() override;
  void movimiento();

private:

IMesh *objetoHabilidad;
ISceneNode* NodoVehiculo;

};

#endif