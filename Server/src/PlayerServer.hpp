#include "RaknetIncludes.hpp"
#include <iostream>

class PlayerServer : public RakNet::NetworkIDObject
{
private:

int *posicion;


public:
  PlayerServer(int *pos);
  int* getPosition();
  void acelerar();
  void frenar();
  void girarA();
  void girarB();



};