#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "Item.hpp"
#include "GestorIDs.hpp"
using namespace std;

class Proyectil : public Item
{
public:
  Proyectil(vector3df posicionItem);
  void lanzarItem();

private:
};

#endif