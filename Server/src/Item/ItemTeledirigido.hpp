#ifndef ITEMTELEDIRIGIDO_H
#define ITEMTELEDIRIGIDO_H

#include "Item.hpp"
#include "Proyectil.hpp"
#include "Timer.hpp"
class ItemTeledirigido : public Item
{
    public:
    ItemTeledirigido(btVector3 posicionItem);
    void lanzarItemTeledirigido(int num);
    int getTimer();
    private:
    Timer *tiempo;
    int t;
};

#endif
