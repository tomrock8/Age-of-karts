#include "Proyectil.hpp"

Proyectil::Proyectil(vector3df posicionItem, int idItem) : Item(posicionItem, idItem)
{
    nombre = "Proyectil";
    nodo->setName("Proyectil");
}

void Proyectil::lanzarItem()
{
    //Se ha mudado al corredor :(
}