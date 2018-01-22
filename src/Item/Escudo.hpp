#ifndef ESCUDO_H
#define ESCUDO_H

#include "Item.hpp"


using namespace std;

class Escudo : public Item
{
public:
  Escudo(vector3df posicionItem, ISceneNode* parent);
  ~Escudo();

private:
	
};

#endif