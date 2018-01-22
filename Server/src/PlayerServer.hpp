#include "RaknetIncludes.hpp"
#include <iostream>

class PlayerServer : public RakNet::NetworkIDObject
{
private:

float *posicion;
float *rotacion;
int id;


public:
  PlayerServer(int *pos, int i);
  int* getPosition();
  void setPositionRotation(float *pos, float *rot);
  int getID();
  void acelerar();
  void frenar();
  void girarA();
  void girarB();



};