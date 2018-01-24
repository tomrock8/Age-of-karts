#ifndef HABILIDAD_H
#define HABILIDAD_H

#include "Item.hpp"
#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "Timer.hpp"

using namespace std;

class Habilidad : public Item
{
public:
  
  Habilidad(int tipo, ISceneNode* n);
  void HabilidadGladiador();
  void HabilidadPirata();
  void HabilidadVikingo();
  void HabilidadChino();
  void lanzarHabilidad();
  //get
  int getInicioHabilidad();
  bool getHabilidadActiva();
  int getTipo();
  //SET
  void setHabilidadActiva(bool s);
 // void setOrientacion(btVector3 o);
 // void setPosicion(btVector3 po);
  void setOrientacion(vector3df o);
  void setPosicion(vector3df po);
  void setPadre(ISceneNode* n);
  btRigidBody* inicializarFisicas(int tipo);
  void eliminarFisicas();

private:
	int tipoHabilidad;
	btCompoundShape *CentroGravedad;
	//objetos
	IMesh * objetoHabilidad;
	Timer *tiempo;
	int inicioHabilidad;
	int finalHabilidad;
	bool habilidadActiva;
	


	//nodo padre
	ISceneNode* NodoVehiculo;

	//variables
	//btVector3 posicion;
	//btVector3 orientacion;
	vector3df orientacion;
};

#endif