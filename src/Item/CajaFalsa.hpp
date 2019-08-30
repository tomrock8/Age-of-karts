#ifndef CAJAFALSA_H
#define CAJAFALSA_H

#include "Item.hpp"

class CajaFalsa : public Item
{
public:
	CajaFalsa(btVector3 posicion, btVector3 escala, btScalar masa,
		float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo,
		btScalar radio, float alturaLanzamiento, int idNodo);

	virtual void lanzarItem(int direccion, btVector3 orientacion, btTransform objeto) override;
	virtual void updateHijos() override;
};

#endif

