#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H

#include "Escena.hpp"

#include "Motor3d.hpp"
#include "Timer.hpp"
#include "Graphics.hpp"


class EscenaLobby : public Escena {
public:
	EscenaLobby(Escena::tipo_escena tipo, std::string ipC);
	~EscenaLobby();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();
	void mostrarTipoPersonaje(int i);


	// METODOS GET
	std::string getIpConexion();

	// METODOS SET

private:
	bool lanzarServer;

	int nElementos; // Numeros para la IP (los ultimos 3)
	std::string ipConexion;
	Client *client;
	bool iniciar;
	bool iniciado;
	bool conectado;
	bool lanzado;
	int count;
	bool pressed;
	bool firstInit;
	IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)

	stringw texto; // Texto a mostrar en pantalla
	IGUIStaticText *textoUI; // Nodo de irrlicht
	ITexture* logoAOK; // Textura del logo del juego
};

#endif /* ESCENALOBBY_H */