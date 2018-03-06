#include "TNodo.hpp"
//-------------------//
//-----CONSTRUCTOR---//
//-------------------//
TNodo::TNodo(const char* n) {
	name = n;
}
//-------------------//
//----DESTRUCTOR-----//
//-------------------//
TNodo::~TNodo() {
	delete entidad;
	delete padre;
	//Borrado de hijos
	for (GLuint i = 0; i < hijos.size(); i++) {
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
	for (GLuint i = 0; i < hijos.size(); i++) {
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

void TNodo::setID(int s) {
	id = s;
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

int TNodo::getID() {
	return id;
}

void TNodo::setName(const char*n) {
	name = n;
}

const char *TNodo::getName() {
	return name;
}

//---------------------------//
//---DIBUJADO DEL LOS NODOS--//
//------CON RECORRIDO DEL----//
//------ARBOL EN PREORDEN----//
//---------------------------//
void TNodo::draw(Shader *shader) {
	if (entidad != NULL) {
		entidad->beginDraw(shader);
	}
	for (GLuint i = 0; i < hijos.size(); i++) {
		std::cout << "(TNodo::draw) Entro a dibujar el hijo: " << this->getName() << std::endl;

		hijos.at(i)->draw(shader);
	}
	/*	for (std::list<TNodo *>::iterator Iterator = hijos.begin(); Iterator != hijos.end(); Iterator++) {
			TNodo *TNodoActual= *Iterator;
			TNodoActual->draw();
		}*/
	if (entidad != NULL) {
		entidad->endDraw();
	}
}