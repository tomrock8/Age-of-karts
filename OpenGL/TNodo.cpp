#include "TNodo.hpp"
//-------------------//
//-----CONSTRUCTOR---//
//-------------------//
TNodo::TNodo() {
}
//-------------------//
//----DESTRUCTOR-----//
//-------------------//
TNodo::~TNodo() {
	
}
//----------------------//
//----MANEJO DE HIJOS---//
//----------------------//
int TNodo::addHijo(TNodo *n) {//1 == ha agregado hijos
	hijos.push_back(n);
	return 1;
}
int TNodo::remHijo(TNodo*n) {//1 == ha borrado hijos 
	
	for (int i = 0; i < hijos.size(); i++) {
		if (hijos.at(i) == n) {
			delete(hijos.at(i));
			return 1;
		}
	}
	
	return 0;
}
//---------------------------//
//-------METODOS SET---------//
//---------------------------//
bool TNodo::setEntidad(TEntidad *n) {
	if (n != NULL) {
		entidad = n;
		return true;
	}
	else return false;
}
void TNodo::setPadre(TNodo *p) {
	padre = p;
}
//---------------------------//
//-------METODOS GET---------//
//---------------------------//
TEntidad *TNodo::getEntidad() {
	return entidad;
}
TNodo *TNodo::getPadre() {
	return padre;
}
//---------------------------//
//---DIBUJADO DEL LOS NODOS--//
//------CON RECORRIDO DEL----//
//------ARBOL EN PREORDEN----//
//---------------------------//
void TNodo::draw() {

	entidad->beginDraw();
	for (int i = 0; i < hijos.size(); i++) {
		hijos.at(i)->draw();
	}
	/*	for (std::list<TNodo *>::iterator Iterator = hijos.begin(); Iterator != hijos.end(); Iterator++) {
			TNodo *TNodoActual= *Iterator;
			TNodoActual->draw();
		}*/
	entidad->endDraw();
}