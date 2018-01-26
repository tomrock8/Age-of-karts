#include "ItemTeledirigido.hpp"

ItemTeledirigido::ItemTeledirigido(vector3df posicionItem) : Item(posicionItem)
{
	Motor3d *m = Motor3d::getInstancia();
	GestorIDs *ids = GestorIDs::getInstancia();
	tamanyo = 1.0f;
	nodo = m->getScene()->addCubeSceneNode(tamanyo);
	ids->setIdentifier(nodo,"ItemTeledirigido");
	id=nodo->getID();

	escala.X = 3.f;
	escala.Y = 3.f;
	escala.Z = 3.f;
	nodo->setScale(escala);

	posicion = posicionItem;
	nodo->setPosition(posicion);

	masa = 10000;

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, m->getDriver()->getTexture("assets/textures/gust.png"));
}
void ItemTeledirigido::lanzarItemTeledirigido(int num)
{
}