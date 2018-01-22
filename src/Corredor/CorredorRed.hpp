#include "Corredor.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CorredorRed : public Corredor
{
private:

public:
  CorredorRed(stringw rutaObj, vector3df pos);
  void acelerar();
  void frenar();
  void girarA();
  void girarB();

};