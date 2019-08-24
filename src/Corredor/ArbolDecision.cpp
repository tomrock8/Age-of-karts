#include "ArbolDecision.hpp"
#include "NodoDecision.hpp"

ArbolDecision::ArbolDecision() {
	NodoRuta = nullptr;
}

ArbolDecision::~ArbolDecision() {
	//cout<<"NUMERO DE NODOS DEL ARBOL"<< arrayNodos.size()<<endl;
	if (arrayNodos.size() > 0) {
		delete arrayNodos.at(0);
		//cout<<"HE BORRADO EL ARBOL"<<endl;
		arrayNodos.clear();
		NodoRuta = nullptr;
	}
	else {
		NodoRuta = nullptr;
		//  cout<<"out of range"<<endl;
	}
}

void ArbolDecision::nuevoNodoDecision(int nodopadre, int idnodo, tipo_nodo tipodecision, tipo_parametro parametro, int valor, bool booleano) {
	if (!NodoRuta) {
		NodoDecision* nodo = new NodoDecision();
		nodo->decision(arrayNodos, nodopadre, idnodo, tipodecision, parametro, valor, booleano);
		NodoRuta = nodo;
	}
	else {
		NodoDecision* nodo = new NodoDecision();
		nodo->decision(arrayNodos, nodopadre, idnodo, tipodecision, parametro, valor, booleano);
	}
}

void ArbolDecision::nuevoNodoAccion(int nodopadre, int idnodo, int tipoaccion) {
	if (!NodoRuta) {
		NodoDecision* nodo = new NodoDecision();
		nodo->accion(arrayNodos, nodopadre, idnodo, tipoaccion);
		NodoRuta = nodo;
	}
	else {
		NodoDecision* nodo = new NodoDecision();
		nodo->accion(arrayNodos, nodopadre, idnodo, tipoaccion);
	}
}

void ArbolDecision::nuevoNodoSalto(int nodopadre, int idnodo, int idNodoSalto) {
	if (!NodoRuta) {
		NodoDecision* nodo = new NodoDecision();
		nodo->salto(arrayNodos, nodopadre, idnodo, idNodoSalto);
		NodoRuta = nodo;
	}
	else {
		NodoDecision* nodo = new NodoDecision();
		nodo->salto(arrayNodos, nodopadre, idnodo, idNodoSalto);
	}
}

int ArbolDecision::recorrerArbol(bool arrayboleanos[], int obj, int jugador) {
	if (NodoRuta->getDecision()) {
		bool check = false; //boleano de control para salir del bucle
		int cont = 0;   // variable para incrementar el array de hijos 

		while (!check && cont != NodoRuta->getHijos().size()) {
			if (NodoRuta->getHijos().at(cont)->getSalto()) {
				for (int i = 0; i < arrayNodos.size(); i++) {
					if (arrayNodos.at(i)->getid() == NodoRuta->getHijos().at(cont)->getidNodoSalto()) {
						NodoRuta = arrayNodos.at(i);

						return 0;
					}
				}
			}

			if (!NodoRuta->getHijos().at(cont)->getAccionB() && !NodoRuta->getHijos().at(cont)->getSalto()) {
				check = NodoRuta->getHijos().at(cont)->getConsulta(arrayboleanos, obj, jugador);
			}
			else {
				check = true;
			}

			if (!check) {
				cont++;
			}
		}

		if (cont == NodoRuta->getHijos().size()) {
			NodoRuta = arrayNodos.at(0);
			return 1; // por si acaso no actualiza del todo bien le decimos que acelere 
		}

		NodoRuta = NodoRuta->getHijos().at(cont);
		//llamamos al arbol recursivamente para que vaya navegando entre sus nodos 
		recorrerArbol(arrayboleanos, obj, jugador);
	}
	else {
		int auxiliar = NodoRuta->getAccion();
		NodoRuta = arrayNodos.at(0);
		return auxiliar;
		//en el caso de que sea un nodo de accion devolver la accion tal cual
	}
}