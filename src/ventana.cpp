#include "irrlichtlib.hpp"
#include "ventana.hpp"


//-------------------------\*
//---CONSTRUCTOR VENTANA---\*
//-------------------------\*
ventana::ventana(CTeclado  teclado)
{

	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480),16,false,false,false,&teclado);
	if(!device) std::cout<<"algo falla con device"<<std::endl;;


	
    device->setWindowCaption(L"AGE OF KARTS");

	driver =  device->getVideoDriver();
	smgr   =  device->getSceneManager();
	guienv =  device->getGUIEnvironment();
	std::cout<<"----------------------------------------------------------------esto lo hago"<<endl;

	
}

IrrlichtDevice* ventana::getDevice(){
	return device;
}
IVideoDriver* ventana::getDriver(){
	return driver;
}
ISceneManager* ventana::getScene(){
	std::cout<<"mierda pa ti"<<endl;
	return smgr;
}
IGUIEnvironment* ventana::getGUI(){
	return guienv;
};