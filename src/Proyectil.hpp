#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "Item.hpp"

using namespace std;

class Proyectil : public Item
{
public:
  Proyectil(Motor3d *ventana, vector3df posicionItem, int idItem);
  void lanzarItem();

private:
};

#endif