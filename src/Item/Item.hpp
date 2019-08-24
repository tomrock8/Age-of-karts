#ifndef ITEM_H
#define ITEM_H


#include "MotorFisicas.hpp"

#include <iostream>
#include <vector>
#include <ctime>


class btVector3;
class btTransform;
class btRigidBody;
class obj3D;

typedef enum { CUBO, ESFERA, CILINDRO } forma_Colision;

class Item
{
public:
	Item(btVector3 posicion, btVector3 escala, btScalar masa,
		float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo,
		btScalar radio, float alturaLanzamiento, int idNodo);
	virtual ~Item();

	void inicializarFisicas();
	virtual void lanzarItem(int direccion, btVector3 orientacion, btTransform objeto) = 0;
	virtual void updateHijos() = 0;
	void ajustarAltura();
	void comprobarAltura(float altura);
	bool update();


	obj3D* getNodo();
	btRigidBody* getRigidBody();
	int getID();
	int getIDPadre();
	void setIDPadre(int id);
	void setLanzado(bool b);
	void setColision(int id);


	bool getLanzado();
	void setNombre(const char* nombre);
	const char* getNombre();
	void setMasa(btScalar masa);
	bool comprobarDestructor();



protected:

	//NODO
	obj3D* nodo;
	btRigidBody* rigidBody;

	btScalar masa;
	btVector3 escala;
	btVector3 posicion;
	btVector3 orientacionItem;

	int idNodoPadre;
	int idwaypoint;
	float alturaLanzamiento;
	forma_Colision fcolision;
	btScalar radio;
	btVector3 tamanyoNodo;
	float tiempoDesctruccion;
	const char* nombre;
	int id;
	bool lanzado;
	bool colision;
	clock_t tiempoLanzado;
	float diferencia;

	bool subir;
	bool bajar;
	float indiceAltura;
	float alturaItem;
};

#endif