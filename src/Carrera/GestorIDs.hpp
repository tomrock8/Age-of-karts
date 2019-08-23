#ifndef GESTORIDS_H
#define GESTORIDS_H

class obj3D;

class GestorIDs {
public:
	void setIdentifier(obj3D* nodo, const char* nombre);
	int getIDLibre();
	void restartID();
	static GestorIDs& instancia();

private:
	GestorIDs(void);
	int id;
	//Deberia crearse un array de ints, para almacenar las ids de objetos borrados y asi aprovecharlas
	//const char *nombre;
	//ISceneNode *nodoA;
};

#endif

