#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"

CTeclado *CTeclado::instancia = NULL;

CTeclado::CTeclado() {
	for (int i = 0; i < KEY_KEY_CODES_COUNT; i++)
	{
		KeyDown[i] = false;
	}

}

CTeclado *CTeclado::getInstancia() {
	if (instancia == NULL)
		instancia = new CTeclado();

	return instancia;
}

bool CTeclado::OnEvent(const SEvent &event)
{
	switch (event.EventType)
	{
	case EET_KEY_INPUT_EVENT:
	{
		KeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}
	default:
		break;
	}
	return false;
}

bool CTeclado::isKeyDown(EKEY_CODE keyCode) const
{
	return KeyDown[keyCode];
}

bool CTeclado::isKeyup(EKEY_CODE keyCode) const
{
	return !KeyDown[keyCode];
}


