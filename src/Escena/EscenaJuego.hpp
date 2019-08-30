#ifndef ESCENAJUEGO_H
#define ESCENAJUEGO_H

#ifdef _WIN32
#define _USE_MATH_DEFINES // for C++  4
#endif

#include "Escena.hpp"
#include "Corredor.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "btBulletDynamicsCommon.h"

class elementoHud;
class cameraThird;
class AlSource;
class DebugDraw;
class GestorColisiones;
class GestorCarrera;
class Client;
class Timer;
class obj3D;

class EscenaJuego : public Escena {
public:
	EscenaJuego(tipo_escena tipo);
	EscenaJuego(tipo_escena tipo, std::string);
	~EscenaJuego();

	virtual void init() override;
	virtual void dibujar() override;

	virtual void limpiar() override;
	virtual void update() override;
	tipo_escena comprobarInputs();
	std::vector<Corredor::tipo_jugador> getJugadores();

	// METODOS GET
	std::string getIpConexion();

	// METODOS SET

private:

	std::string ipConexion;
	std::string* chr = new std::string("OnGameHUD");
	void UpdatePhysics(unsigned int TDeltaTime);
	void UpdateRender(btRigidBody* TObject);
	void renderDebug(int i);
	void debugRageIA(int k);
	void debugPlot(int j, float k, std::string str);
	void updateHUD(int i);
	void crearHUD(int i);

	int numPantallas; //Numero de mandos conectados

	bool debug;

	DebugDraw* debugDraw;

	int controlPlayer;
	std::vector<cameraThird*> camera;
	int tipoCamara;
	bool cambioCamara;
	GestorColisiones* colisiones;

	int lastFPS;
	std::uint32_t TimeStamp;
	std::uint32_t DeltaTime;

	Client* client;
	btVector3 pos;

	float tiempoRefresco;
	GestorCarrera* gc;
	bool fin_carrera;
	Timer* t;

	float gravedad;

	// IMGUI
	bool muestraDebug;
	bool debug_Jugador;
	bool end;
	bool show_another_window[6];
	bool muestraDebugIA;
	std::string sr;

	bool waitingPlayers;

	//SONIDO 
	AlSource* fuenteCarrera;
	AlSource* fuenteCountDown;
	bool countDown3;
	bool countDown2;
	bool countDown1;
	bool countDownGo;

	int puesto;
	int objeto;
	std::vector<int> vueltas;
	std::vector<int> vueltas_aux;
	std::vector<int> habilidad;
	std::vector<float> inc_habilidad;
	std::vector<float> muestra_tiempo;

	//LUCES
	obj3D* luzPuntual_0;
	obj3D* luzPuntual_1;
	obj3D* luzPuntual_2;
	obj3D* luzPuntual_3;

	//corredores
	std::vector<Corredor*> pj;
	Corredor* jugador;
	elementoHud* cuentaAtras;

};

#endif /* ESCENAJUEGO_H */