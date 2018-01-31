#include "Proyectil.hpp"

Proyectil::Proyectil(btVector3 posicionItem) : Item(posicionItem) {
	tamanyo = 1.0f;
	nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyo);
	nombre = "Proyectil";
	GestorIDs::instancia().setIdentifier(nodo, nombre);
	id = nodo->getID();

	escala.setX(3);
	escala.setY(3);
	escala.setZ(3);
	nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));

	posicion = posicionItem;
	nodo->setPosition(vector3df(posicion.getX(), posicion.getY(), posicion.getZ()));

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/gust.png"));
}

void Proyectil::lanzarItem() {
	//Se ha mudado al corredor :(
}