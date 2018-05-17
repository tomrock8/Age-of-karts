#ifndef HABILIDAD_H
#define HABILIDAD_H

#include "Item.hpp"

#define PI 3.14159265358979323846  /* pi */
#define RADTODEG 180.0f / PI

using namespace std;

class Habilidad : public Item
{
public:
  
  Habilidad(int tipo, btRigidBody* n,btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,btVector3 tamanyoNodo,btScalar radio,
  float alturaLanzamiento,int idNodo);
  virtual void lanzarItem(int direccion,btVector3 orientacion,btTransform objeto) override;
  virtual void updateHijos() override;
  virtual void deleteHijos() override;
  void movimiento();


private:

  int cont2;
  int cont;
	int tipoHabilidad;
  bool habilidadActiva;
	//objetos
  
  btRigidBody* NodoVehiculo;
	btVector3 orientacion;
	
};

#endif