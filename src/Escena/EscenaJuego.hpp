#ifndef ESCENAJUEGO_H
#define ESCENAJUEGO_H

#include "GestorEscena.hpp"
#include "Escena.hpp"

#include "Motor3d.hpp"
#include "MotorFisicas.hpp"
#include "DebugFisicas.hpp"
#include "Camara3persona.hpp"
#include "Client.hpp"

#include "Pista.hpp"
#include "GestorJugadores.hpp"
#include "GestorColisiones.hpp"




class EscenaJuego : public Escena {
public:
	EscenaJuego(tipo_escena tipo);
	~EscenaJuego();

	void init();
	void dibujar();
	void limpiar();
	void update();

	// METODOS GET


	// METODOS SET

private:
	void comprobarInputs();
	void UpdatePhysics(u32 TDeltaTime);
	void UpdateRender(btRigidBody *TObject);

	int argc;
	int debug;

	DebugDraw *debugDraw;

	Camara3persona *camara;
	GestorColisiones *colisiones;

	int lastFPS;
	u32 TimeStamp;
	u32 DeltaTime;

	Client *client;
	vector3df pos;
};

#endif /* ESCENAJUEGO_H */