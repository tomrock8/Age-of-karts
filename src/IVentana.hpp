#ifndef IVENTANA_H
#define IVENTANA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
using namespace std;

class IVentana
{
  public:
	IVentana(CTeclado &teclado);
	
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

#endif /* IVENTANA_H */