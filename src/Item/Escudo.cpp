#include "Escudo.hpp"
#include "Motor3d.hpp"

Escudo::Escudo(vector3df posicionItem, ISceneNode* parent) : Item(posicionItem)
{
	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	Motor3d *m = Motor3d::getInstancia();
	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();
	nombre = "Escudo";

//	EscudoNodo->setName("Escudo"+idItem);
	nodo = smgr->addMeshSceneNode(m->getScene()->getMesh("assets/Objetos/escudo.obj"));
	nodo->setScale(vector3df(5.0f, 5.0f, 5.0f));
	//nodo = smgr->addSphereSceneNode(5.0f);
	nodo->setParent(parent);
	//4smgr->getMeshManipulator()->setVertexColors(nodo->getMesh(), SColor(50));
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	nodo->setName(nombre);




}
Escudo::~Escudo() {
	delete nodo;
}

