#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "Escena.hpp"
#include "Timer.hpp"

class elementoHud;

class EscenaLobby : public Escena {
public:
	EscenaLobby(Escena::tipo_escena tipo, std::string ipC);
	~EscenaLobby();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	void updateLobbyOnline();
	void debugClientInfo();
	void crearFicheroIP();
	void mostrarInfoLobby(int indice);
	void mostrarInfoLobbyPropia();
	Escena::tipo_escena comprobarInputs();
	void mostrarTipoPersonaje(int i);
	int character_callback(GLFWwindow *window, unsigned int codepoint);
	void movimientoVertical(bool interno);
	void movimientoHorizontal(int k,bool dcha,int id);
	void actualizarSelector();
	void iniciarCarrera();
	void crearHUD();
	void addControllers();
	// METODOS GET
	std::string getIpConexion();

	// METODOS SET

private:
	bool lanzarServer;

	int nElementos; // Numeros para la IP (los ultimos 3)
	int nElementos2;
	std::string ipConexion;
	Client *client;
	bool iniciar;
	bool empiezaCarrera;
	bool iniciado;
	bool conectado;
	bool lanzado;
	int count;
	bool pressed[6];
	bool firstInit;
	Timer *time;
	int laps;

	bool selection_online;
	bool offline;
	bool offline_split;
	bool end;
    bool checkReady;
	bool checkCPU;
	bool mando;
	bool pantallaCarga;
	bool checkfocus;
	bool show_config;
	int index;
	int index_interno;
	bool index_selected;
	int index_mapa;
	std::string iphost;
	std::string connecting;
	std::string *infoLobby;
	int numPantallas;
	//elementos del hud
	elementoHud *fondo;
	elementoHud *fondo_final;
	elementoHud *personajeOff;
	elementoHud *flechaIzq;
	elementoHud *flechaDer;
	elementoHud *seleccion;
	//elementos del hud:: seleccion de CPU

	elementoHud *CPU;
	elementoHud *checkLAPS;
	//elementos HUD comunes
	elementoHud *mapa;

	//elemtentos HUD Botones
	elementoHud *start;
	elementoHud *returnB;
	elementoHud *selectB;
	//elementos HUD multiplayer
	std::vector<elementoHud*> players;
	std::vector<elementoHud*> playersReady;

};

#endif /* ESCENALOBBY_H */
