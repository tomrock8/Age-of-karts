#include "Corredor.hpp"
#include "irrlichtLib.hpp"

class CorredorRed : public Corredor
{
private:

public:
	CorredorRed(stringw rutaObj, vector3df pos);
	void acelerar();
	void frenar();
	void girarA();
	void girarB();

};