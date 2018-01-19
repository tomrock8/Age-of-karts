#include "Motor3d.hpp"

//-------------------------\*
//---CONSTRUCTOR Motor3d---\*
//-------------------------\*

Motor3d *Motor3d::instancia = NULL;

Motor3d::Motor3d() {
	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(1280, 720), 16, false, false, true, 0);
	if (!device) 
		std::cout << "ERROR AL CREAR EL DEVICE" << std::endl;

	driver = device->getVideoDriver();

	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	CTeclado *teclado = CTeclado::getInstancia();
	setTeclado(teclado);

	device->setWindowCaption(L"AGE OF KARTS");
}

Motor3d::~Motor3d() {
	device->drop();
	delete instancia;
}

void Motor3d::dibujar() {
	driver->beginScene(true, true, SColor(255, 200, 200, 200));
	smgr->drawAll();
}

void Motor3d::cerrar() {
	device->closeDevice();
}


// METODOS GET
Motor3d *Motor3d::getInstancia() {
	if (instancia == NULL)
		instancia = new Motor3d();

	return instancia;
}

IGUIEnvironment *Motor3d::getGUI() {
	return guienv;
}

ISceneManager *Motor3d::getScene() {
	return smgr;
}

IrrlichtDevice *Motor3d::getDevice() {
	return device;
}

IVideoDriver *Motor3d::getDriver() {
	return driver;
}


// METODOS SET

void Motor3d::setTeclado(CTeclado *teclado) {
	device->setEventReceiver(teclado);
}