#ifndef CTECLADO_H
#define CTECLADO_H

#include <iostream>
#include "IrrlichtLib.hpp"

class CTeclado : public IEventReceiver
{
  public:
<<<<<<< HEAD
	bool isKeyDown(EKEY_CODE keyCode) const;
	bool isKeyup(EKEY_CODE keyCode) const;
	bool OnEvent(const SEvent &event);

	//metodos GET
	static CTeclado *getInstancia();

	//metodos SET

  private:
	bool KeyDown[KEY_KEY_CODES_COUNT];
	static CTeclado *instancia;
=======
	CTeclado();
  bool isKeyDown(EKEY_CODE keyCode) const;
  bool isKeyup(EKEY_CODE keyCode) const;
  bool OnEvent(const SEvent &event);

	//metodos SET

>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.

  private:
    bool KeyDown[KEY_KEY_CODES_COUNT];

};

#endif /* CTECLADO */
