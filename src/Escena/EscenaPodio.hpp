#ifndef ESCENAPODIO_H
#define ESCENAPODIO_H

#include "Escena.hpp"

#include <iostream>

class EscenaPodio: public Escena {
public:

    EscenaPodio(Escena::tipo_escena tipo,std::vector<Corredor::tipo_jugador> jugadores);
	~EscenaPodio();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();

private:

    std::vector<obj3D*> personajes;
    //obj3D *Podio;
    cameraThird *camera;

};

#endif /* ESCENAPODIO_H */