#ifndef CORREDORJUGADOR_H
#define CORREDORJUGADOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "Corredor.hpp"

using namespace std;

class CorredorJugador : public Corredor
{
  public:
	CorredorJugador(stringw rutaObj, vector3df pos);
  std::string toString();
  
	void movimiento();

  private:
};

#endif /* CORREDORJUGADOR_H */