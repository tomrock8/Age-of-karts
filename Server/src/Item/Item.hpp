#ifndef ITEM_H
#define ITEM_H

#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "GestorIDs.hpp"
#include "Timer.hpp"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

typedef enum {CUBO,ESFERA,CILINDRO} forma_Colision;

class Item
{
public:

  
  Item(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,float alturaLanzamiento);
  void inicializarFisicas();
  virtual void lanzarItem(int direccion,btVector3 orientacion)=0;
  virtual void updateHijos()=0;
  bool update();
  void Delete();

  IMeshSceneNode *getNodo();
  btRigidBody *getRigidBody();
  int getID();
  void setLanzado(bool b);
  void setColision(int id);


  bool getLanzado();
  void setNombre(const char* nombre);
  const char* getNombre();
  void setMasa(btScalar masa);
  bool comprobarDestructor();



protected:

  //NODO
  IMeshSceneNode *nodo;

  //Propiedades rigidbody
  btRigidBody *rigidBody;
  btDefaultMotionState *MotionState;
  btCollisionShape *Shape;
  btScalar masa;
  btVector3 escala;
  btVector3 posicion;

  int idwaypoint;
  float alturaLanzamiento;
  forma_Colision fcolision;
  btScalar radio;
  float tamanyoNodo;
  float tiempoDesctruccion;
  const char *nombre;
  int id;
  bool lanzado;
  bool colision;
  clock_t tiempoLanzado;
  
};

#endif