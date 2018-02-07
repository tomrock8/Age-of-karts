
#include "TEntidad.hpp"
#include "TNodo.hpp"
#include "TTransform.hpp"
#include "TMalla.hpp"
#include "TLuz.hpp"
#include "TCamara.hpp"


int main() {

	//---COSNTRUCCION ARBOL DE NODOS-----//
	TNodo *Escena    = new TNodo();
	TNodo *RotaLuz   = new TNodo();
	TNodo *RotaCam   = new TNodo();
	TNodo *RotaCoche = new TNodo();
	Escena->addHijo(RotaLuz);
	RotaLuz->setPadre(Escena);
	Escena->addHijo(RotaCam);
	RotaCam->setPadre(Escena);
	Escena->addHijo(RotaCoche);
	RotaCoche->setPadre(Escena);
	//Siguiente nivel de nodos
	TNodo *TraslaLuz = new TNodo();
	TNodo *TraslaCam = new TNodo();
	TNodo *TraslaCoche = new TNodo();
	RotaLuz->addHijo(TraslaLuz);
	TraslaLuz->setPadre(RotaLuz);
	RotaCam->addHijo(TraslaCam);
	TraslaCam->setPadre(RotaCam);
	RotaCoche->addHijo(TraslaCoche);
	TraslaCoche->setPadre(RotaCoche);
	//SIGUIENTE NIVEL DE NODOS SE EXPANDIRA DEPENDIENDO DE LA CANTIDAD DE JUGADORES
	TNodo * Luz1     = new TNodo();
	TNodo * Cam1     = new TNodo();
	TNodo * Coche1  = new TNodo();
	TraslaLuz->addHijo(Luz1);
	Luz1->setPadre(TraslaLuz);
	TraslaCam->addHijo(Cam1);
	Cam1->setPadre(TraslaCam);
	TraslaCoche->addHijo(Coche1);
	Coche1->setPadre(TraslaCoche);


	// --Transformaciones rotacion
	TTransform *TransfRotaLuz = new TTransform();
	TTransform *TransfRotaCam = new TTransform();
	TTransform *TransfRotaCoche = new TTransform();
	RotaLuz->setEntidad(TransfRotaLuz);
	RotaCam->setEntidad(TransfRotaCam);
	RotaCoche->setEntidad(TransfRotaCoche);


	// --Transformaciones traslacion
	TTransform *TransfTraslaLuz = new TTransform();
	TTransform *TransfTraslaCam = new TTransform();
	TTransform *TransfTraslaCoche = new TTransform();
	TraslaLuz->setEntidad(TransfTraslaLuz);
	TraslaCam->setEntidad(TransfTraslaCam);
	TraslaCoche->setEntidad(TransfTraslaCoche);
	
	//--Entidades a los nodos luz camara y coche--//
	TLuz *entLuz = new TLuz();
	TCamara *entCam = new TCamara();
	TMalla *entMallaCoche = new TMalla();
	Luz1->setEntidad(entLuz);
	Cam1->setEntidad(entCam);
	Coche1->setEntidad(entMallaCoche);


	Escena->draw();

	std::cout << "Hola que haces" << std::endl;
	return 0;
}