#include "Corredor.hpp"
#include "EstadosJugador.hpp"

class CorredorRed : public Corredor
{
private:

public:
  CorredorRed(stringw rutaObj, vector3df pos);
  void updateRed();
  void movimiento();
  void acelerarRed();
  void frenarRed();
  void girarDRed();
  void girarIRed();

};