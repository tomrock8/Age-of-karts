#ifndef ESCUDO_H
#define ESCUDO_H

#include "Item.hpp"

#define PI 3.14159265358979323846  /* pi */
#define RADTODEG 180.0f / PI

using namespace std;

class Escudo : public Item
{
public:
  Escudo(btRigidBody* n,btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
  float alturaLanzamiento,int idNodo);
  ~Escudo();
  virtual void lanzarItem(int direccion,btVector3 orientacion,btTransform objeto) override;
  virtual void updateHijos() override;
  virtual void deleteHijos() override;
  void movimiento();

private:


btRigidBody* NodoVehiculo;

};

#endif