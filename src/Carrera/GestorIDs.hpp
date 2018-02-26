#ifndef GESTORIDS_H
#define GESTORIDS_H

#include "IrrlichtLib.hpp"

using namespace std;

class GestorIDs {
public:
	void setIdentifier(ISceneNode *nodo, const char *nombre);
	int getIDLibre();
        void restartID();
	static GestorIDs &instancia();

private:
	GestorIDs(void);
	int id;
	//Deberia crearse un array de ints, para almacenar las ids de objetos borrados y asi aprovecharlas
	//const char *nombre;
	//ISceneNode *nodoA;
};

#endif

