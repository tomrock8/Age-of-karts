#ifndef CORREDOR_H
#define CORREDOR_H

#include <iostream>
#include "IrrlichtLib.hpp"
#include "btBulletDynamicsCommon.h"

using namespace std;

class Corredor
{
  public:
<<<<<<< HEAD
	Corredor(stringw rutaObj, vector3df pos);
	void InicializarFisicas(list<btRigidBody *> &objetos, btDiscreteDynamicsWorld *mundo);
	std::string toString();

	//Update
	void update();

	//Metodos GET
	IMeshSceneNode *getNodo();
	float getVelocidad();
	btRaycastVehicle *getVehiculo();
	btRigidBody *getRigidBody();
	std::string getDireccion();
	int getDireccionGrados();

  protected:
	//orientacion
	vector3df orientacion;

	//Mascaras de colision

=======
	Corredor(ISceneManager *smgr, stringw rutaObj,btVector3 pos);
	//--movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	float movimiento(float pos, float vel, float accel, float delta);

	//auxiliares
	void escalar(float tam);
	void setAxis(ISceneManager *smgr);
	void moveCameraControl(IrrlichtDevice *device);
	//Update
	void updatePosicion();
	void updateVelocidadInicial();
	void updatePosicionInicial();
	void update();
	void resetGiro();
	//metodos SET

	//metodos GET
	vector3df getPosicion();
	vector3df getRotacion();
	IMeshSceneNode* getNodo();
	float getVelocidad();
	btRigidBody *getRigidBody();

  private:
	// aceleraciones
	float aceleracion, aceleracionInversa, aceleracionFrenado, tiempo;
	//velocidades
	float velocidadIni, velocidad, velocidadX, velocidadZ, velocidadMax;
	//posiciones
	float posXIni, posZIni, posYIni, posX, posZ, posY;
	bool adelante, atras;
	// rotaciones
	float Rotacionruedas,Rotacioncoche, rotRuedasY, rotCocheY,anguloMaxRuedas,anguloMaxCoche;
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.
	//objetos
	IMesh *coche;
	IMeshSceneNode *cuboNodo;
<<<<<<< HEAD
	ISceneNode *rueda1;
	ISceneNode *rueda2;
	ISceneNode *rueda3;
	ISceneNode *rueda4;

	//bullet
	btRaycastVehicle *vehiculo;
	btDefaultMotionState *motionStateCoche; //interpolacion
	btCollisionShape *FormaColision;		//contorno
	btRigidBody *CuerpoColisionChasis;
	btCompoundShape *CentroGravedad;

	btVector3 direccionRuedas;
	btVector3 rotacionRuedas;
	btScalar suspension;
	btScalar anchoRueda;
	btScalar radioRueda;
	btScalar alturaConexionChasis;
	btScalar Masa;
	btScalar Fuerza;
	btScalar FuerzaFrenado;
	btScalar FuerzaFrenoMano;
	btScalar FuerzaGiro;
	btScalar FuerzaFrenadoReposo;

	// Direccion
	bool norte, sur, este, oeste;
	float direccionGrados;

	void CrearRuedas(btRaycastVehicle *vehiculo, btRaycastVehicle::btVehicleTuning tuning);
	void BorrarFisicas();

	// Movimiento del corredor
	void acelerar();
	void frenar();
	void desacelerar();
	void girarDerecha();
	void girarIzquierda();
	void frenodemano();

	// UPDATES
	void actualizarRuedas();
	void updateDireccion();
	void Pedazodemierda();
	void updateDireccionGrados();

	//--movimiento del corredor
	virtual void movimiento(){}; // A implementar por derivadas
=======
	IMeshSceneNode *ruedasDelanteras;
	IMeshSceneNode *ruedasTraseras;
	//bullet
	btTransform transCoche;//posicion;
	btDefaultMotionState *motionStateCoche;//interpolacion
	btVector3 extensionCoche;  //contorno
	btCollisionShape *contorno;//contorno
	btVector3 localInertia;//masa
	btScalar masa;//masa
	btRigidBody *rigidBody;
>>>>>>> parent of af9063b... Se lee un .obj con las direcciones de un NurbsPath, se crean y se muestran los waypoints a traves del fichero .obj.
};

#endif /* JUGADOR_H */