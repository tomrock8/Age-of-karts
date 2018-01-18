#include "Gui.hpp"

Gui::Gui() {
	Motor3d *m = Motor3d::getInstancia();
	// Bootstrap CEGUI::System with an IrrlichtRenderer object, an Irrlicht based
	// ResourceProvider, and an Irrlicht based ImageCodec.
	CEGUI::IrrlichtRenderer &ren = CEGUI::IrrlichtRenderer::bootstrapSystem(*m->getDevice());
	renderer = &ren;
}

Gui::~Gui() {
	CEGUI::System::destroy();

	//CEGUI::OpenGL3Renderer::destroy(static_cast<CEGUI::OpenGL3Renderer&>(*d_renderer));
	CEGUI::IrrlichtRenderer::destroy(static_cast<CEGUI::IrrlichtRenderer&>(*renderer));
}