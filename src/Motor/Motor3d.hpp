#ifndef MOTOR3D_H
#define MOTOR3D_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"

using namespace std;

class Motor3d
{
public:

	void iniciarDibujado();
	void dibujar();
	void terminarDibujado();

	void cerrar();

	// Metodos GET
	static Motor3d &instancia();
	IVideoDriver *getDriver();
	ISceneManager *getScene();
	IGUIEnvironment *getGUI();
	IrrlichtDevice *getDevice();

	u16 getAnchoPantalla();
	u16 getAltoPantalla();

	// Metodos SET
	void setTeclado(CTeclado *teclado);

private:
	Motor3d();

	u16 anchoPantalla;
	u16 altoPantalla;

	IrrlichtDevice *device;
	IVideoDriver *driver;
	ISceneManager *smgr;
	IGUIEnvironment *guienv;
};

#endif /* MOTOR3D_H */