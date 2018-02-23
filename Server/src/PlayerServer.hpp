#include "RaknetIncludes.hpp"
#include <iostream>

class PlayerServer : public RakNet::NetworkIDObject
{
private:

float posicion[3];
float rotacion[3];
int id;
int accion;
int estado1;
int estado2;
int estado3;
int estado4;
int obj;


public:
  PlayerServer(float *pos, int i);
  float* getPosition();
  void setPositionRotation(float pos[3], float rot[3]);
  void setEstados(int e1, int e2, int e3, int e4);
  int getID();
  int getAccion();
  void setAccion(int a);
  void acelerar();
  void frenar();
  void girarA();
  void girarB();
  void setObj(int t);



};