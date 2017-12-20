#include "Motor3d.hpp"

//-------------------------\*
//---CONSTRUCTOR Motor3d---\*
//-------------------------\*

Motor3d *Motor3d::instancia = NULL;

Motor3d::Motor3d()
{

	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, true, 0);
	if (!device)
		std::cout << "algo falla con device" << std::endl;

	device->setWindowCaption(L"AGE OF KARTS");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	CTeclado *teclado = CTeclado::getInstancia();
	setTeclado(teclado);
}

Motor3d::~Motor3d() {}

Motor3d *Motor3d::getInstancia()
{
	if (instancia == NULL)
		instancia = new Motor3d();

	return instancia;
}

void Motor3d::setTeclado(CTeclado *teclado)
{

	device->setEventReceiver(teclado);
}

IrrlichtDevice *Motor3d::getDevice()
{
	return device;
}

IVideoDriver *Motor3d::getDriver()
{
	return driver;
}

ISceneManager *Motor3d::getScene()
{
	return smgr;
}

IGUIEnvironment *Motor3d::getGUI()
{
	return guienv;
}

void Motor3d::dibujar()
{
	driver->beginScene(true, true, SColor(255, 200, 200, 200));
	smgr->drawAll();
}

void Motor3d::cerrar()
{
	device->closeDevice();
}