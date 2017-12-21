#include "Estatico.hpp"

Estatico::Estatico(vector3df posicionItem, int idItem) : Item(posicionItem, idItem)
{
    nombre = "Estatico";
    nodo->setName("Estatico");

    id = idItem;
    nodo->setID(id);
}

void Estatico::soltarItem()
{
    //otro que se fue al corredor bitches :(
}