#ifndef PISTA_H
#define PISTA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "Motor3d.hpp"


using namespace std;

class Pista
{

  public:
    Pista(vector3df Posicion, vector3df Scala);
    ~Pista();
    void InicializarFisicas(list<btRigidBody*> &objetos, btDynamicsWorld *mundo);
    btTriangleMesh *getBulletTriangleMesh(IMesh *const mesh, vector3df escala);
    void BorrarFisicas();

  private:

  btRigidBody* CuerpoColisionMapa;
  btCollisionShape *FormaColision;
  btDefaultMotionState *MotionState; // posicion del cuerpo de colision
  ISceneNode *MapaNodo;
  btScalar Masa;


};

#endif /* PISTA_H */
