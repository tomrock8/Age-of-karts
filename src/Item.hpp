#ifndef ITEM_H
#define ITEM_H

#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Motor3d.hpp"
#include <iostream>

using namespace std;

class Item
{
  public:
    Item(Motor3d *ventana, vector3df posicionItem, int idItem);
    btRigidBody* inicializarFisicas();
    void lanzarItem();
    void Delete(core::list<btRigidBody *> &objetosMundo, btDiscreteDynamicsWorld *mundoMetodo);
    btRigidBody *getRigidBody();
  protected:
    ISceneNode *nodo;
    btRigidBody *rigidBody;
    btScalar masa;
    f32 tamanyo;
    vector3df escala;
    vector3df posicion;
    char *nombre;
    int id;
};

#endif