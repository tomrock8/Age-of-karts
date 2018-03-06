#ifndef HABILIDAD_H
#define HABILIDAD_H

#include "Item.hpp"

using namespace std;

class Habilidad : public Item
{
public:
  
  Habilidad(int tipo, ISceneNode* n,btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,
  float alturaLanzamiento);
  virtual void lanzarItem(int direccion,btVector3 orientacion) override;
  virtual void updateHijos() override;
  void movimiento();


private:

	int tipoHabilidad;
  bool habilidadActiva;
	//objetos
	IMesh *objetoHabilidad;
	ISceneNode* NodoVehiculo;
  btVector3 orientacion;
	
};

#endif