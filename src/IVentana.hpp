#ifndef IVENTANA_H
#define IVENTANA_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "CTeclado.hpp"
using namespace std;

class IVentana
{
  public:
<<<<<<< HEAD:src/Motor3d.hpp
	//Destructor
	~Motor3d(void);
=======
	IVentana(CTeclado &teclado);
	
	//metodos SET

>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.:src/IVentana.hpp
	//metodos GET

<<<<<<< HEAD:src/Motor3d.hpp
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
=======
	IVideoDriver* getDriver();
	ISceneManager* getScene();
	IGUIEnvironment* getGUI();
	IrrlichtDevice* getDevice();

  private:
	IrrlichtDevice *device;
	IVideoDriver* driver;
	ISceneManager* smgr; 
	IGUIEnvironment* guienv;
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.:src/IVentana.hpp
};

#endif /* IVENTANA_H */