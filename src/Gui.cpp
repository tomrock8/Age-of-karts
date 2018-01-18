#include "Gui.hpp"

Gui::Gui(IrrlichtDevice &device) {
	Motor3d *m = Motor3d::getInstancia();
	IrrlichtDevice &dev = *m->getDevice();
	// Bootstrap CEGUI::System with an IrrlichtRenderer object, an Irrlicht based
	// ResourceProvider, and an Irrlicht based ImageCodec.
	//CEGUI::OpenGLRenderer &ren = CEGUI::OpenGLRenderer::bootstrapSystem(OPEN_GL_DEVICE);
	IrrlichtRenderer &ren = CEGUI::IrrlichtRenderer::bootstrapSystem(device); // Inicializa el renderer de irrlicht
	renderer = &ren;

	System::create(*renderer); // Inicializa el CEGUI::System y sus variables


	// Recoger el ResourceProvider para los elementos
	ImageManager::getSingleton().loadImageset("./lib/cegui/assets/imagesets/WindowsLook.imageset");
	
	// Al inicializar, se configura el grupo de recursos
	DefaultResourceProvider *rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("imagesets", "./lib/cegui/assets/imagesets/");

	// Para cargar un imageset:
	ImageManager::getSingleton().loadImageset("WindowsLook.imageset", "imagesets");
}

Gui::~Gui() {
	CEGUI::System::destroy(); // Destruye el CEGUI::System y sus variables

	//CEGUI::OpenGL3Renderer::destroy(static_cast<CEGUI::OpenGL3Renderer&>(*d_renderer));
	CEGUI::IrrlichtRenderer::destroy(static_cast<CEGUI::IrrlichtRenderer&>(*renderer));
}