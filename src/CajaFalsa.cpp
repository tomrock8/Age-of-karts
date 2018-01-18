#include "CajaFalsa.hpp"

CajaFalsa::CajaFalsa(vector3df posicionItem, int idItem) : Estatico(posicionItem, idItem)
{
    nombre = "CajaFalsa";
	//nodo->setName("CajaFalsa");

	id = idItem;
	nodo->setID(id);
}