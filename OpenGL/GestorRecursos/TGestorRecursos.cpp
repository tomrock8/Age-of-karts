#include "TGestorRecursos.hpp"

TGestorRecursos::TGestorRecursos() {}
TGestorRecursos::~TGestorRecursos() {}
//manejor del objeto 

TRecursoMalla *TGestorRecursos::getRecurso(const char * nombre)
{

	recursoMallas.size();

	TRecursoMalla *recMalla = NULL;
	if (recursoMallas.size() == 0) {


		for (int i = 0; i < recursoMallas.size(); i++) {
			recMalla = (TRecursoMalla*)recursoMallas.at(i);
			if (nombre == recMalla->getNombre()) {							//Si encuentro un recurso se almacena
				recMalla = (TRecursoMalla*)recursoMallas.at(i);
			}
		}
	}
	if (recMalla == NULL) {											//si no hay ningun recurso con ese nombre se carga
		recMalla->loadModel(nombre);
		recMalla->setNombre(nombre);
		recursoMallas.push_back(recMalla);
	}
	return recMalla;
}

