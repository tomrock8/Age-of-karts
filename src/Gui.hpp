#ifndef GUI_H
#define GUI_H

#include "Motor3d.hpp"

#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\Irrlicht\Renderer.h>
#include <CEGUI\ImageManager.h>

using namespace CEGUI;

class Gui {

	public:
		Gui(IrrlichtDevice &device);
		~Gui();

	private:
		CEGUI::IrrlichtRenderer *renderer;
		//CEGUI::OpenGLRenderer *renderer;

};

#endif