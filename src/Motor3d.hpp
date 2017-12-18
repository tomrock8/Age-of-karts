#ifndef MOTOR3D_H
#define MOTOR3D_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
using namespace std;

class Motor3d
{
public:
	//Destructor
	~Motor3d(void);
	//metodos GET
	static Motor3d *getInstancia();

	IVideoDriver *getDriver();
	ISceneManager *getScene();
	IGUIEnvironment *getGUI();
	IrrlichtDevice *getDevice();
	void setTeclado(CTeclado *teclado);
	void dibujar();

	void cerrar();

private:
	Motor3d(void);
	static Motor3d *instancia;
	IrrlichtDevice *device;
	IVideoDriver *driver;
	ISceneManager *smgr;
	IGUIEnvironment *guienv;
};

#endif /* MOTOR3D_H */