#include "RaknetIncludes.hpp"
#include "Corredor.hpp"
#include "IrrlichtLib.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CorredorRed : public RakNet::NetworkIDObject , public Corredor
{
private:

public:
  CorredorRed(stringw rutaObj, vector3df pos);
  void acelerar();
  void frenar();
  void girarA();
  void girarB();

};