#include "Proyectil.hpp"

Proyectil::Proyectil(vector3df posicionItem) : Item(posicionItem)
{
	Motor3d *m = Motor3d::getInstancia();
	GestorIDs *ids = GestorIDs::getInstancia();
	tamanyo = 1.0f;
	nodo = m->getScene()->addCubeSceneNode(tamanyo);
	nombre = "Proyectil";
	ids->setIdentifier(nodo,nombre);
	id=nodo->getID();

	escala.X = 3.f;
	escala.Y = 3.f;
	escala.Z = 3.f;
	nodo->setScale(escala);

	posicion = posicionItem;
	nodo->setPosition(posicion);

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, m->getDriver()->getTexture("assets/textures/gust.png"));
}

void Proyectil::lanzarItem()
{
	//Se ha mudado al corredor :(
}