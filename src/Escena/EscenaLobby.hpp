#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "Escena.hpp"
#include "Timer.hpp"

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
	void movimientoHorizontal(int k,bool dcha);
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
	float count;
	bool pressed;
	bool firstInit;
	Timer *time;
	int laps;

	bool checkfocus;
	bool selection_online;
	bool offline;
	bool offline_split;
	bool end;
	bool show_config;
	int index;
	int index_interno;
	bool index_selected;
	int index_mapa;
	std::string iphost;
	std::string connecting;
	std::string *infoLobby;
	int numPantallas;
};

#endif /* ESCENALOBBY_H */