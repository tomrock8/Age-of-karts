#include "RaknetIncludes.hpp"
#include <iostream>

class PlayerServer : public RakNet::NetworkIDObject
{
private:

float posicion[3];
float rotacion[3];
int id;


public:
  PlayerServer(float *pos, int i);
  float* getPosition();
  void setPositionRotation(float pos[3], float rot[3]);
  int getID();
  void acelerar();
  void frenar();
  void girarA();
  void girarB();



};