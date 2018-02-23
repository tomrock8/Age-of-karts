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
	delete entidad;
	delete padre;
	//Borrado de hijos
	for (int i = 0; i < hijos.size(); i++) {
		delete(hijos.at(i));
	}
	
}
void TNodo::inicializarMotor() {

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
	if (entidad != NULL) {
		entidad->beginDraw();
	}
	for (int i = 0; i < hijos.size(); i++) {
		std::cout << "Entro a dibujar el hijo" << std::endl;

		hijos.at(i)->draw();
	}
	/*	for (std::list<TNodo *>::iterator Iterator = hijos.begin(); Iterator != hijos.end(); Iterator++) {
			TNodo *TNodoActual= *Iterator;
			TNodoActual->draw();
		}*/
	if (entidad != NULL) {
		entidad->endDraw();
	}
	
}