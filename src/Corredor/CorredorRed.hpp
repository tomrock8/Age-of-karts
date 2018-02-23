#include "Corredor.hpp"
#include "EstadosJugador.hpp"
#include "Turbo.hpp"
#include "Caja.hpp"
#include "CajaFalsa.hpp"
#include "Aceite.hpp"
#include "Pista.hpp"

class CorredorRed : public Corredor
{
private:

public:
  CorredorRed(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo);
  //virtual void update() override;
  void updateRed();
  void movimiento();
  void acelerarRed();
  void frenarRed();
  void girarDRed();
  void girarIRed();
  void lanzarItemRed();

};