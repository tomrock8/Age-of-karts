#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\Irrlicht\Renderer.h>
#include <CEGUI\ImageManager.h>
#include "btBulletDynamicsCommon.h"

using namespace std;

class Gui {

	public:
		Gui(irr::IrrlichtDevice *device);
		~Gui();
		
		void dibujar();
		bool cargarScheme(std::string archivoScheme);

		CEGUI::Window *crearWidget(const std::string &tipo, float tam[], float escala[], const std::string &nombre = "");

		// METODOS SET
		void setFuente(std::string archivoFuente);
		void setWidgetTamEscala(CEGUI::Window *vent, float tam[], float escala[]);

		//METODOS GET
		CEGUI::IrrlichtRenderer *getRender();
		CEGUI::GUIContext *getContexto();

	private:
		CEGUI::IrrlichtRenderer *render; // Encargado del render de la interface
		//CEGUI::OpenGLRenderer *render;

		CEGUI::GUIContext *contexto; // Contexto de CEGUI
		CEGUI::Window *ventana; // Ventana CEGUI que sera el padre de todos los elementos
};

#endif