#include "Corredor.hpp"
#include "EstadosJugador.hpp"

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
  void update();
  void movimiento();
  void acelerarRed();
  void frenarRed();
  void girarDRed();
  void girarIRed();

};