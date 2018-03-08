#include "CajaFalsa.hpp"

CajaFalsa::CajaFalsa(btVector3 posicion,btVector3 escala,btScalar masa,float tiempoDesctruccion,forma_Colision fcolision,float tamanyoNodo,btScalar radio,
float alturaLanzamiento) : Item(posicion,escala,masa,tiempoDesctruccion,fcolision,tamanyoNodo,radio,alturaLanzamiento)
{

    nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyoNodo);
	nodo->setPosition(vector3df(posicion.getX(), posicion.getY(), posicion.getZ()));
	nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
	

	nodo->setMaterialFlag(EMF_LIGHTING, false);
	nodo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	GestorIDs::instancia().setIdentifier(nodo,"Estatico");
	id=nodo->getID();
    
    nombre = "CajaFalsa";
	//nodo->setName("CajaFalsa");
    nodo->setMaterialTexture(0, Motor3d::instancia().getDriver()->getTexture("assets/textures/gust.png"));

    inicializarFisicas();

}

void CajaFalsa::lanzarItem(int direccion,btVector3 orientacion){

}

void CajaFalsa::updateHijos(){
	
}