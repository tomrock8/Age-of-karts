#include "Aceite.hpp"

Aceite::Aceite(btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo) : Item(posicion, escala, masa, tiempoDesctruccion, fcolision, tamanyoNodo, radio, alturaLanzamiento, idNodo) {

	//nodo = Motor3d::instancia().getScene()->addCubeSceneNode(tamanyoNodo.getX());
	nodo = TMotor::instancia().newMeshNode("Estatico", "assets/aceite/aceite.obj", "escena_raiz",false);
	
	
	nodo->setScale(escala.getX(),escala.getY(),escala.getZ());

	GestorIDs::instancia().setIdentifier(nodo, "Estatico");
	id = nodo->getID();
	nombre = "Aceite";

	inicializarFisicas();
}


void Aceite::lanzarItem(int direccion, btVector3 orientacion,btTransform objeto) {
	
	btTransform posObj= rigidBody->getCenterOfMassTransform();
	posObj.setOrigin(btVector3(posicion.getX(),posicion.getY(),posicion.getZ()));
	posObj.setRotation(objeto.getRotation());
	rigidBody->setCenterOfMassTransform(posObj);

}
void Aceite::updateHijos() {}
void Aceite::deleteHijos() {}