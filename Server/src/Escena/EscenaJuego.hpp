#ifndef ESCENAJUEGO_H
#define ESCENAJUEGO_H

#include "Escena.hpp"
#include "DebugFisicas.hpp"
#include "GestorColisiones.hpp"
#include "GestorCarrera.hpp"
#include "Timer.hpp"
#include <vector>

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
	std::string getIpConexion();

	// METODOS SET

private:

	std::string ipConexion;

	void UpdatePhysics(unsigned int TDeltaTime);
	void UpdateRender(btRigidBody *TObject);
	void renderDebug();
	void debugRageIA(int k);
	void debugPlot(int j,float k,std::string str);
	void updateHUD();
	
	bool debug;

	DebugDraw *debugDraw;

	int controlPlayer;
	cameraThird *camera;
	int tipoCamara;
	bool cambioCamara;
	GestorColisiones *colisiones;

	int lastFPS;
	std::uint32_t TimeStamp;
	std::uint32_t DeltaTime;

	Client *client;
	btVector3 pos;
	

	float tiempoRefresco;
	GestorCarrera *gc;
	bool fin_carrera;
	Timer *t;

	float gravedad;

	// IMGUI
	bool muestraDebug;
	bool debug_Jugador;
	bool end;
	bool show_another_window;
	bool muestraDebugIA;
	std::string sr;
	
	//SONIDO 
	AlSource *fuenteCarrera;
	AlSource *fuenteCountDown;
	bool countDown3;
	bool countDown2;
	bool countDown1;
	bool countDownGo;

	int puesto;
	int vueltas;
	int vueltas_aux;
	float muestra_tiempo;
	
};

#endif /* ESCENAJUEGO_H */