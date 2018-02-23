#ifndef ESCENAJUEGO_H
#define ESCENAJUEGO_H

#include <ctime>

#include "GestorEscena.hpp"
#include "Escena.hpp"

#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "DebugFisicas.hpp"
#include "Camara3persona.hpp"

#include "Pista.hpp"
#include "GestorJugadores.hpp"
#include "GestorCarrera.hpp"
#include "GestorColisiones.hpp"
#include <vector>
#include "Graphics.hpp"


class EscenaJuego : public Escena {
public:
	EscenaJuego(tipo_escena tipo);
	EscenaJuego(tipo_escena tipo, std::string);
	~EscenaJuego();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();

	// METODOS GET


	// METODOS SET

private:
	
	std::string ipConexion;

	void UpdatePhysics(u32 TDeltaTime);
	void UpdateRender(btRigidBody *TObject);
	int debug;

	DebugDraw *debugDraw;

	int controlPlayer;

	Camara3persona *camara;
	int tipoCamara;
	bool cambioCamara;
	GestorColisiones *colisiones;

	int lastFPS;
	u32 TimeStamp;
	u32 DeltaTime;

	Client *client;
	vector3df pos;

	clock_t tiempoRefresco;
	GestorCarrera *gc;
	bool fin_carrera;
};

#endif /* ESCENAJUEGO_H */