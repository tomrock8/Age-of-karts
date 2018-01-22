#include "RaknetIncludes.hpp"
#include <iostream>

class PlayerServer : public RakNet::NetworkIDObject
{
private:

int *posicion;
int id;


public:
  PlayerServer(int *pos, int id);
  int* getPosition();
  void acelerar();
  void frenar();
  void girarA();
  void girarB();
  int getID();



};