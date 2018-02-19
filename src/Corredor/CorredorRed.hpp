#include "Corredor.hpp"
#include "EstadosJugador.hpp"
#include "Turbo.hpp"
#include "Caja.hpp"
#include "CajaFalsa.hpp"
#include "Aceite.hpp"
#include "Pista.hpp"

class CorredorRed : public Corredor, RakNet::NetworkIDObject
{
private:

public:
  CorredorRed(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo);
  void updateRed();
  void movimiento();
  void acelerarRed();
  void frenarRed();
  void girarDRed();
  void girarIRed();
  void lanzarItemRed();

};