#ifndef CTECLADO_H
#define CTECLADO_H

#include <iostream>
#include "irrlichtlib.hpp"


class CTeclado : public IEventReceiver
{
  public:
	CTeclado();
  bool isKeyDown(EKEY_CODE keyCode) const;
  bool isKeyup(EKEY_CODE keyCode) const;
  bool OnEvent(const SEvent &event);

	//metodos SET


  private:
    bool KeyDown[KEY_KEY_CODES_COUNT];

};

#endif /* CTECLADO */

