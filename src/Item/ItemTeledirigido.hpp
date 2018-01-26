#ifndef ITEMTELEDIRIGIDO_H
#define ITEMTELEDIRIGIDO_H

#include "Item.hpp"
#include "Proyectil.hpp"
class ItemTeledirigido : public Item
{
    public:
    ItemTeledirigido(vector3df posicionItem);
    void lanzarItemTeledirigido(int num);

    private:

};

#endif
