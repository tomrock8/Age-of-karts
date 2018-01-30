#include "ItemTeledirigido.hpp"
#include "GestorCarrera.hpp"
#include "Corredor.hpp"
ItemTeledirigido::ItemTeledirigido(btVector3 posicionItem) : Item(posicionItem)
{
	Motor3d *m = Motor3d::getInstancia();
	GestorIDs *ids = GestorIDs::getInstancia();
	tamanyo = 1.0f;
	nodo = m->getScene()->addCubeSceneNode(tamanyo);
	ids->setIdentifier(nodo,"ItemTeledirigido");
	id=nodo->getID();


	//posicion = posicionItem;
	//nodo->setPosition(posicion);

	masa = 10000;

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	nodo->setMaterialTexture(0, m->getDriver()->getTexture("assets/textures/gust.png"));
}

void ItemTeledirigido::lanzarItemTeledirigido(int num)
{
	GestorCarrera *gc=new GestorCarrera();
	Corredor *pj=gc->comprobarItemTeledirigido(num);

	

	if (pj!=NULL){
		tiempo = Timer::getInstancia();
		t = tiempo->getTimer();
		pj->setObjetivoTelederigido();
		/*posicion=pj->getNodo()->getPosition();
		nodo->setPosition(posicion);
		nodo->setParent(pj->getNodo());
		inicializarFisicas();
		rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);*/
	}
	
}
int ItemTeledirigido::getTimer(){
	return t;
}