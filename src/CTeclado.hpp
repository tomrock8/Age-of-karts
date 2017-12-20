#ifndef CTECLADO_H
#define CTECLADO_H

#include <iostream>
#include "IrrlichtLib.hpp"


class CTeclado : public IEventReceiver
{
public:
	bool isKeyDown(EKEY_CODE keyCode) const;
	bool isKeyup(EKEY_CODE keyCode) const;
	bool OnEvent(const SEvent &event);

	//metodos GET
	static CTeclado* getInstancia();

	//metodos SET

private:
	bool KeyDown[KEY_KEY_CODES_COUNT];
	static CTeclado* instancia;

	CTeclado();
};

#endif /* CTECLADO */

