#include "TNodo.hpp"
#include "TMotor.hpp"

TNodo::TNodo(const char* n) {
	name = new std::string(n);
	entidad = nullptr;
	padre = nullptr;
	hijos.clear();
}

TNodo::~TNodo() {
	//std::cout << "Destructor de TNodo: " << *name << "\n";
	padre = nullptr; // Eliminamos el enlace con el padre
	//Borrado de hijos
	if (hijos.size() > 0) {
		//cout<<"Numero de HIJOS::::"<<hijos.size()<<endl;
		for (GLuint i = 0; i < hijos.size(); i++) {
		//	cout<<"Soy el hijo::"<< i << "ME voy a intentar borrar BORRO JUEPUTAS"<<endl;
			delete hijos.at(i);
			
		}
		hijos.clear();
	}else{
	//cout<<"NO TENGO HIJOS::::"<<endl;
	if (entidad){	
	//cout<<"VOY A BORRAR SU ENTIDAD"<<endl;
	delete entidad;
	//cout<<"ENTIDAD BORRADA"<<endl;
	}		
	delete name;
	//cout<<"ME BORRO"<<endl;
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
void TNodo::setEntidad(TEntidad *n) {
	if (n != NULL) {
		entidad = n;
	}
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


std::vector<TNodo *>TNodo::getHijos() {

	return hijos;
	
}


//----------------------------
// DIBUJADO DEL LOS NODOS
//	RECORRIDO EN PREORDEN
//----------------------------
void TNodo::draw(Shader *shader) {
	string n = name->c_str();
	if (entidad != NULL) {
		//if (n.find("escena") == std::string::npos)
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
		//if (n.find("escena") == std::string::npos) {
			entidad->endDraw();
		//}
	}
}

void TNodo::deleteNode(){


	TNodo *padreaux = padre;
	TNodo *padreaux2 = nullptr;
	std::vector<TNodo*> miel;
	if(padre){
	while(padreaux && padreaux->getPadre()->getHijos().size() == 1){

		padreaux = padreaux->getPadre();		
	}
	
	padreaux2 = padreaux->getPadre();
	miel = padreaux2->getHijos();
	for(int i = 0; i<miel.size();i++){
		
		if(miel[i] == padreaux){
			std::cout << "Destructor de TNodo: " << padreaux->getName() << "\n";
			miel.erase(miel.begin() + i);	
		}

	}
	padreaux2->setArrayHijos(miel);
	delete padreaux;

	padreaux = nullptr;
	padreaux2=nullptr;
	miel.clear();
	}
}


void TNodo::setArrayHijos(std::vector<TNodo*> miel){

	hijos.clear();
	hijos=miel;

}