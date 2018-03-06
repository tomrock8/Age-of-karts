#include "TGestorRecursos.hpp"

TGestorRecursos::TGestorRecursos() {}
TGestorRecursos::~TGestorRecursos() {}

TRecursoMalla *TGestorRecursos::getRecurso(const char * nombre) {
	TRecursoMalla *recMalla = NULL;
	for (GLuint i = 0; i < recursoMallas.size(); i++) {
		recMalla = (TRecursoMalla*)recursoMallas.at(i);
		if (nombre == recMalla->getNombre()) {							//Si encuentro un recurso se almacena
			recMalla = (TRecursoMalla*)recursoMallas.at(i);
		}
		else {
			recMalla = NULL;
		}
	}


	if (recMalla == NULL) {	//si no hay ningun recurso con ese nombre se carga
		recMalla = new TRecursoMalla(nombre);
		//recMalla->loadModel(nombre);
		recMalla->setNombre(nombre);
		recursoMallas.push_back(recMalla);
	}
	return recMalla;
}

TRecursoTextura *TGestorRecursos::getRecursoTextura(const char *text) {
	TRecursoTextura *recTextura = NULL;

	for (GLuint i = 0; i < recursoTexturas.size(); i++) {
		recTextura = (TRecursoTextura*)recursoTexturas.at(i);
		if (text == recTextura->getNombre()) {							//Si encuentro un recurso se almacena
			recTextura = (TRecursoTextura*)recursoTexturas.at(i);
		}
		else {
			recTextura = NULL;
		}
	}

	if (recTextura == NULL) {	//si no hay ningun recurso con ese nombre se carga
		recTextura = new TRecursoTextura(text);

		cout << "(TGestorRecursos::getRecursoTextura) Nombre :" << std::string(recTextura->getNombre()) << endl;
		recursoTexturas.push_back(recTextura);
	}
	return recTextura;

}

