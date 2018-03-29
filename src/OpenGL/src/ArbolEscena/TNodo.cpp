#include "TNodo.hpp"

TNodo::TNodo(const char* n) {
	name = n;
}

TNodo::~TNodo() {
	delete entidad;
	delete padre;
	//Borrado de hijos
	for (GLuint i = 0; i < hijos.size(); i++) {
		delete(hijos.at(i));
	}
}

// MANEJO DE HIJOS
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

//-----------------
// METODOS SET
//-----------------
bool TNodo::setEntidad(TEntidad *n) {
	if (n != NULL) {
		entidad = n;
		return true;
	}
	else return false;
}

void TNodo::setPadre(TNodo *p) { padre = p; }
//void TNodo::setID(int s) { id = s; }
void TNodo::setName(const char*n) { name = n; }

//-----------------
// METODOS GET
//-----------------
TEntidad *TNodo::getEntidad() { return entidad; }
TNodo *TNodo::getPadre() { return padre; }
//int TNodo::getID() { return id; }
const char *TNodo::getName() { return name; }

TNodo *TNodo::getNode(const char* nombre) {
	TNodo *aux = nullptr;

	//Si su nombre es el que se busca, devolverse a si mismo
	if (strcmp(name, nombre) == 0)
		return this;

	//Sino buscar en los hijos
	if (hijos.size() > 0) {
		for (int i = 0; i < hijos.size(); i++) {
			aux = hijos.at(i)->getNode(nombre);
			if (aux) return aux;
		}
	}

	// No se encuentra en los hijos
	return nullptr;
}

//----------------------------
// DIBUJADO DEL LOS NODOS
//	RECORRIDO EN PREORDEN
//----------------------------
void TNodo::draw(Shader *shader) {
	string n = name;
	if (entidad != NULL) {
		if (n.find("escena") == std::string::npos)
			getEntidad()->beginDraw(shader);
	}
	for (GLuint i = 0; i < hijos.size(); i++) {
		hijos.at(i)->draw(shader);
	}
	/*	for (std::list<TNodo *>::iterator Iterator = hijos.begin(); Iterator != hijos.end(); Iterator++) {
			TNodo *TNodoActual= *Iterator;
			TNodoActual->draw();
		}*/
	if (entidad != NULL) {
		if (n.find("escena") == std::string::npos) {
			entidad->endDraw();
		}
	}
}