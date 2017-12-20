#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "Item.hpp"

using namespace std;

class Proyectil : public Item
{
public:
  Proyectil(vector3df posicionItem, int idItem);
  void lanzarItem();

private:
};

#endif