#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Escena.hpp"
#include "Motor3d.hpp"
#include "Timer.hpp"
#include "Graphics.hpp"
#include "Timer.hpp"

class EscenaLobby : public Escena {
public:
	EscenaLobby(Escena::tipo_escena tipo, std::string ipC);
	~EscenaLobby();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	void mostrarInfoLobby(int indice);
	Escena::tipo_escena comprobarInputs();
	void mostrarTipoPersonaje(int i);


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
	bool iniciado;
	bool conectado;
	bool lanzado;
	float count;
	bool pressed;
	bool firstInit;
	IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)
	Timer *time;
	stringw texto; // Texto a mostrar en pantalla
	stringw texto2;
	IGUIStaticText *textoUI; // Nodo de irrlicht
	ITexture* logoAOK; // Textura del logo del juego
	bool selection;
	bool offline;
	std::string iphost;
};

#endif /* ESCENALOBBY_H */