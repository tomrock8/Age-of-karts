#include "CajaFalsa.hpp"

CajaFalsa::CajaFalsa(btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo) : Item(posicion, escala, masa, tiempoDesctruccion, fcolision, tamanyoNodo, radio, alturaLanzamiento, idNodo) {

	nodo = TMotor::instancia().newMeshNode("Estatico", "assets/caja/caja.obj", "escena_raiz");
	cout << "(CajaFalsa::CajaFalsa) Hay que comprobar la posicion que sea respecto del corredor al crear\n";
	nodo->setPosition(posicion.getX(), posicion.getY(), posicion.getZ());
	//nodo->setScale(vector3df(tamanyoNodo.getX(), tamanyoNodo.getY(), tamanyoNodo.getZ()));

	GestorIDs::instancia().setIdentifier(nodo, "Estatico");
	id = nodo->getID();
	nombre = "CajaFalsa";
	inicializarFisicas();
}

void CajaFalsa::lanzarItem(int direccion, btVector3 orientacion) {}
void CajaFalsa::updateHijos() {}
void CajaFalsa::deleteHijos() {}