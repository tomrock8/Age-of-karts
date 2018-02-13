#include "Motor3d.hpp"

//-------------------------\*
//---CONSTRUCTOR Motor3d---\*
//-------------------------\*

Motor3d::Motor3d() {
	anchoPantalla = 1280;
	altoPantalla = 720;
	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(anchoPantalla, altoPantalla), 16, false, false, true, 0);
	device->getCursorControl()->setVisible(false);

	if (!device) 
		std::cout << "ERROR AL CREAR EL DEVICE" << std::endl;

	driver = device->getVideoDriver();

	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	device->setWindowCaption(L"AGE OF KARTS");
}

void Motor3d::iniciarDibujado() {
	driver->beginScene(true, true, SColor(255, 200, 200, 200));
}

void Motor3d::dibujar() {
	smgr->drawAll();
	guienv->drawAll();
}

void Motor3d::terminarDibujado() {
	driver->endScene();
}

void Motor3d::cerrar() {
	device->closeDevice();
	device->drop();
}


// METODOS GET
Motor3d &Motor3d::instancia() {
	static Motor3d _instancia;
	return _instancia;
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

u16 Motor3d::getAnchoPantalla() {
	return anchoPantalla;
}

u16 Motor3d::getAltoPantalla() {
	return altoPantalla;
}

IVideoDriver *Motor3d::getDriver() {
	return driver;
}
