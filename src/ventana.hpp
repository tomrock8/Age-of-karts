#ifndef VENTANA_H
#define VENTANA_H

#include <iostream>
#include "irrlichtlib.hpp"
#include "CTeclado.hpp"
using namespace std;

class ventana
{
  public:
	ventana(CTeclado  teclado);
	
	//metodos SET

	//metodos GET

	IVideoDriver* getDriver();
	ISceneManager* getScene();
	IGUIEnvironment* getGUI();
	IrrlichtDevice* getDevice();

  private:
	IrrlichtDevice *device;
	IVideoDriver* driver;
	ISceneManager* smgr; 
	IGUIEnvironment* guienv;
};

#endif /* VENTANA_H */