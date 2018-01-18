#ifndef GUI_H
#define GUI_H

#include "Motor3d.hpp"

#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\Irrlicht\Renderer.h>

class Gui {

	public:
		Gui();
		~Gui();

	private:
		CEGUI::IrrlichtRenderer *renderer;

};

#endif