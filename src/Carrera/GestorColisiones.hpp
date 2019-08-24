#ifndef GESTORCOLISIONES_H
#define GESTORCOLISIONES_H

#include <vector>

class Corredor;
class obj3D;
class Caja;

class GestorColisiones
{
public:
	GestorColisiones();
	~GestorColisiones();
	void ComprobarColisiones();
	bool JugadorCaja(std::vector<Caja*> cajas);
	bool JugadorTurbo();
	bool JugadorEstatico();
	bool JugadorWaypoint();
	bool JugadorProyectil();
	bool ItemTeledirigidoWaypoint();
	bool Escudoitems();
	bool HabilidadesItems();
	bool HabilidadesJugadores();
	bool habilidadVShabilidad();
	void IniciarTimer();

	//bool JugadorItemTeledirigido();

private:
	std::vector<Corredor*> pj1;
	obj3D* nodoA;
	obj3D* nodoB;
	float tiempoLanzado;
	bool control;
	float tiempoInicio;
};

#endif