#ifndef MOTOR3D_H
#define MOTOR3D_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"

using namespace std;

class Motor3d
{
  public:
	~Motor3d();

	void dibujar();
	int getTime();
	void cerrar();

	// Metodos GET
	static Motor3d *getInstancia();
	IVideoDriver *getDriver();
	ISceneManager *getScene();
	IGUIEnvironment *getGUI();
	IrrlichtDevice *getDevice();

	// Metodos SET
	void setTeclado(CTeclado *teclado);


  private:
	Motor3d();

	static Motor3d *instancia;
	IrrlichtDevice *device;
	IVideoDriver *driver;
	ISceneManager *smgr;
	IGUIEnvironment *guienv;
};

#endif /* MOTOR3D_H */