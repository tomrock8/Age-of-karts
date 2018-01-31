#include "ItemTeledirigido.hpp"
#include "GestorCarrera.hpp"
#include "Corredor.hpp"
ItemTeledirigido::ItemTeledirigido(btVector3 posicionItem) : Item(posicionItem) {
	tamanyo = 1.0f;
	nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyo);
	GestorIDs::instancia().setIdentifier(nodo, "ItemTeledirigido");
	id = nodo->getID();


	//posicion = posicionItem;
	//nodo->setPosition(posicion);

	masa = 10000;

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/gust.png"));
}

void ItemTeledirigido::lanzarItemTeledirigido(int num)
{
	GestorCarrera *gc=new GestorCarrera();
	gc->comprobarItemTeledirigido(num);

	

	/*if (b){
		tiempo = Timer::getInstancia();
		t = tiempo->getTimer();
		
		/*posicion=pj->getNodo()->getPosition();
		nodo->setPosition(posicion);
		nodo->setParent(pj->getNodo());
		inicializarFisicas();
		rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}*/
}

int ItemTeledirigido::getTimer() {
	return t;
}