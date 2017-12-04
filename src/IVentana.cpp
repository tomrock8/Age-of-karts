#include "IrrlichtLib.hpp"
#include "IVentana.hpp"

//-------------------------\*
//---CONSTRUCTOR VENTANA---\*
//-------------------------\*
IVentana::IVentana(CTeclado &teclado)
{

	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, true, &teclado);
	if (!device)
		std::cout << "algo falla con device" << std::endl;

	device->setWindowCaption(L"AGE OF KARTS");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
}

IrrlichtDevice *IVentana::getDevice()
{
	return device;
}

IVideoDriver *IVentana::getDriver()
{
	return driver;
}

ISceneManager *IVentana::getScene()
{
	return smgr;
}

IGUIEnvironment *IVentana::getGUI()
{
	return guienv;
};