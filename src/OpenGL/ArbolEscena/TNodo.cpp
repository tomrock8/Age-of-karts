#include "TNodo.hpp"
#include "TMotor.hpp"
TNodo::TNodo(const char* n) {
	name = new std::string(n);
	
}

TNodo::~TNodo() {
	//Borrado de hijos

	if(hijos.size()>0){

		//cout<<"tengo hijos::::"<<hijos.size() <<endl;
	for (GLuint i = 0; i < hijos.size(); i++) {
		//cout<<"intentando borrar:::"<<i<<endl;
		delete hijos.at(i);
		
	}
		if(this != TMotor::instancia().getSceneNode()){
			hijos.clear();
			delete entidad;
			delete name;
			//delete this;
			//cout<<"Borro nodo"<<endl;
		}

	}else{
		if(this != TMotor::instancia().getSceneNode()){
			//cout<<"no tengo ningun hijo"<<endl;	
			delete entidad;
			delete name;	
			//delete this;
			//cout<<"Borro nodo"<<endl;	
		}
			
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
void TNodo::setPadre(TNodo *p) { padre = p; }
void TNodo::setName(const char*n) { name = new std::string(n); }

bool TNodo::setEntidad(TEntidad *n) {
	if (n != NULL) {
		entidad = n;
		return true;
	}
	else return false;
}

//-----------------
// METODOS GET
//-----------------
TEntidad *TNodo::getEntidad() { return entidad; }
TNodo *TNodo::getPadre() { return padre; }
const char *TNodo::getName() { return name->c_str(); }

TNodo *TNodo::getNode(const char* nombre) {
	TNodo *aux = nullptr;

	//Si su nombre es el que se busca, devolverse a si mismo
	if (strcmp(name->c_str(), nombre) == 0)
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


std::vector<TNodo *>TNodo::getHijos(){

	return hijos;
}


//----------------------------
// DIBUJADO DEL LOS NODOS
//	RECORRIDO EN PREORDEN
//----------------------------
void TNodo::draw(Shader *shader) {
	string n = name->c_str();
	if (entidad != NULL) {
		if (n.find("escena") == std::string::npos)
			getEntidad()->beginDraw(shader);
	}
	for (GLuint i = 0; i < hijos.size(); i++) {
		hijos.at(i)->draw(shader);
	}
	// for (std::list<TNodo *>::iterator Iterator = hijos.begin(); Iterator != hijos.end(); Iterator++) {
	//   TNodo *TNodoActual = *Iterator;
	// 	 TNodoActual->draw();
	// }
	if (entidad != NULL) {
		if (n.find("escena") == std::string::npos) {
			entidad->endDraw();
		}
	}
}


