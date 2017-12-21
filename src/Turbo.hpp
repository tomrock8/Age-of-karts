#ifndef TURBO_H
#define TURBO_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Corredor.hpp"
class Turbo
{
public:

    Turbo(int id, btVector3 pos, bool estado);
    bool getTurboActivo();
    void setTurboActivo(Corredor *corredor, bool s);   
    int getTiempoTurbo();
    void setTurbo(bool);
private:
    IMeshSceneNode *turbo;
    vector3df escala;
    btTransform transform;
    btDefaultMotionState *MotionState;
    btVector3 halfExtents;
    btCollisionShape *Shape;
    btVector3 localInertia;
    btRigidBody * rigidBody;
    bool turboActivo;
    int tiempo;


};

#endif /* TURBO */