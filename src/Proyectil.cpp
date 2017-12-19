#include "Proyectil.hpp"

Proyectil::Proyectil(Motor3d *ventana, vector3df posicionItem, int idItem):
    Item(ventana, posicionItem, idItem)
{
    nombre = "Proyectil";
    nodo->setName(nombre);

    id = idItem;
    nodo->setID(id);

}

void Proyectil::lanzarItem(){
//Se ha mudado al corredor :(
			
}