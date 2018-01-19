#include "Gui.hpp"

Gui::Gui(irr::IrrlichtDevice *device) {
	//Motor3d *m = Motor3d::getInstancia();
	// Bootstrap CEGUI::System with an IrrlichtRenderer object, an Irrlicht based
	// ResourceProvider, and an Irrlicht based ImageCodec.

	// Guardamos la direccion del objeto que crea CEGUI (POR ESO EL &)
	//render = &CEGUI::OpenGLRenderer::bootstrapSystem(OPEN_GL_DEVICE);
	render = &CEGUI::IrrlichtRenderer::bootstrapSystem(*device); // Inicializa el renderer de irrlicht
	contexto = &CEGUI::System::getSingletonPtr()->createGUIContext(render->getDefaultRenderTarget()); // Contenedor de todos los widgets
	CEGUI::WindowManager *winMan = CEGUI::WindowManager::getSingletonPtr();
	ventana = winMan->createWindow("DefaultWindow", "root");
	CEGUI::DefaultResourceProvider *recursos = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingletonPtr()->getResourceProvider());
	
	recursos->setResourceGroupDirectory("imagesets", "lib/cegui/assets/imagesets/");
	recursos->setResourceGroupDirectory("schemes", "lib/cegui/assets/schemes/");
	recursos->setResourceGroupDirectory("layouts", "lib/cegui/assets/layouts/");
	recursos->setResourceGroupDirectory("fonts", "lib/cegui/assets/fonts/");
	recursos->setResourceGroupDirectory("looknfeel", "lib/cegui/assets/looknfeel/");
	recursos->setResourceGroupDirectory("lua_scripts", "lib/cegui/assets/lua_scripts/");
	
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");


	contexto->setRootWindow(ventana);

	std::cout << "\n FIN DE CONSTRUCTOR DE GUI\n\n";
}

Gui::~Gui() {
	CEGUI::System::getSingletonPtr()->destroyGUIContext(*contexto); // Con * para referencia
	CEGUI::System::destroy(); // Destruye el CEGUI::System y sus variables

	//CEGUI::OpenGL3Renderer::destroy(static_cast<CEGUI::OpenGL3Renderer&>(*d_renderer));
	CEGUI::IrrlichtRenderer::destroy(static_cast<CEGUI::IrrlichtRenderer&>(*render));
}

CEGUI::Window *Gui::crearWidget(const std::string &tipo, float tam[], float escala[], const std::string &nombre) {
	CEGUI::Window *vent = CEGUI::WindowManager::getSingletonPtr()->createWindow(tipo, nombre);
	ventana->addChild(vent);
	setWidgetTamEscala(vent, tam, escala);
	return vent;
}

void Gui::cargarScheme(std::string archivoScheme){
	// Esquema de elementos para el gui
	CEGUI::SchemeManager::getSingletonPtr()->createFromFile(archivoScheme);
}

void Gui::dibujar() {
	render->beginRendering();

	contexto->draw();

	render->endRendering();


	// TODO: BUG
}


// -----------------------------
//    METODOS GET
// -----------------------------

CEGUI::GUIContext *Gui::getContexto() {
	return contexto;
}

CEGUI::IrrlichtRenderer *Gui::getRender() {
	return render;
}


// -----------------------------
//    METODOS SET
// -----------------------------

void Gui::setWidgetTamEscala(CEGUI::Window * vent, float escala[], float tam[]) {
	ventana->setPosition(CEGUI::UVector2(CEGUI::UDim(escala[0], tam[0]), CEGUI::UDim(escala[1], tam[1])));
	ventana->setSize(CEGUI::USize(CEGUI::UDim(escala[2], tam[2]), CEGUI::UDim(escala[3], tam[3])));
}

void Gui::setFuente(std::string archivoFuente) {
	// Fuente a mostrar en el gui
	CEGUI::FontManager::getSingletonPtr()->createFromFile(archivoFuente + ".font");
	contexto->setDefaultFont(archivoFuente);
}