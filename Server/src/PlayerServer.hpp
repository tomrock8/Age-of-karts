#include "RaknetIncludes.hpp"
#include <iostream>

class PlayerServer : public RakNet::NetworkIDObject
{
private:

int posicion[3];


public:
  PlayerServer();
  int* getPosition();
  void setPosition(int posicionSet[]);
  void setPositionX(int x);
  void playerUpdate();

  int move, turn, moveCheck, turnCheck;

};