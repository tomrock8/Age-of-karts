#ifndef ACEITE_H
#define ACEITE_H

#include "Item.hpp"

using namespace std;

class Aceite : public Item
{
public:
	Aceite(btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio, float alturaLanzamiento, int idNodo);

	virtual void lanzarItem(int direccion, btVector3 orientacion, btTransform objeto) override;
	virtual void updateHijos() override;
};

#endif

