#ifndef ESTATICO_H
#define ESTATICO_H

#include "Item.hpp"
#include "GestorIDs.hpp"
using namespace std;

class Estatico : public Item
{
  public:
    Estatico(vector3df posicionItem);
    void soltarItem();
    btRigidBody *inicializarFisicas();
	//~Estatico();
  private:
	  btDefaultMotionState * MotionState;
	  btCollisionShape *Shape;
    bool colision;
};

#endif