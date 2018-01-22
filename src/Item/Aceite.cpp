#include "Aceite.hpp"

Aceite::Aceite(vector3df posicionItem, int idItem) : Estatico(posicionItem, idItem)
{

    nombre = "Aceite";
	//nodo->setName("Aceite");
	
	id = idItem;
	nodo->setID(id);
	//masa = 0;
}