#include "Aceite.hpp"

Aceite::Aceite(btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo) : Item(posicion, escala, masa, tiempoDesctruccion, fcolision, tamanyoNodo, radio, alturaLanzamiento, idNodo) {

	//nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyoNodo.getX());
	nodo = TMotor::instancia().newMeshNode("Estatico", "assets/aceite/aceite.obj", "escena_raiz",false);
	cout << "(Aceite::Aceite) Hay que comprobar la posicion que sea respecto del corredor al crear\n";
	nodo->setPosition(posicion.getX(), posicion.getY(), posicion.getZ());
	nodo->setScale(escala.getX(),escala.getY(),escala.getZ());

	GestorIDs::instancia().setIdentifier(nodo, "Estatico");
	id = nodo->getID();
	nombre = "Aceite";

	inicializarFisicas();
}


void Aceite::lanzarItem(int direccion, btVector3 orientacion) {}
void Aceite::updateHijos() {}
void Aceite::deleteHijos() {}