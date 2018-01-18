#ifndef MOTOR3D_H
#define MOTOR3D_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
#include "Gui.hpp"


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
	int getTime();

	void cerrar();

  private:
	Motor3d(void);
	static Motor3d *instancia;
	IrrlichtDevice *device;
	IVideoDriver *driver;
	ISceneManager *smgr;
	IGUIEnvironment *guienv;
	ITimer *irrTimer;

	Gui *interfaz;

};

#endif /* MOTOR3D_H */