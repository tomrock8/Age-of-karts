#ifndef GESTORSONIDO_H
#define GESTORSONIDO_H


#include <iostream>
#include <vector>
#include <string>

#include "AlSource.hpp"
#include "SoundEnum.hpp"
#include "alut.h"
#include "al.h"
#include "alc.h"

class GestorSonido {
public:
	GestorSonido();
	~GestorSonido();

	int cargarSonido(std::string file);
	void ajustarVolumen(float general, float musica, float efectos);

	// METODOS GET
	bool getSonidoCargado();
	float getVolGeneral();
	float getVolMusica();
	float getVolEfectos();
	void getParametros(float* general, float* musica, float* efectos);

	// METODOS SET
	void setPosition(float* pos);
	void setListenerData();
	void setVolGeneral(float vol);
	void setVolMusica(float vol);
	void setVolEfectos(float vol);

private:
	bool cargado;
	float volGeneral;
	float volMusica;
	float volEfectos;

	ALCcontext* context;
	ALCdevice* device;
	std::vector<int> buffers;
	AlSource* sourceAux;
	unsigned int bufferAux;
	ALvoid* alBuffer;             //data for the buffer
	ALenum alFormatBuffer;    //buffer format
	ALsizei alFreqBuffer;       //frequency     
	ALsizei alBufferLen;        //bit depth 
	ALboolean alLoop = AL_FALSE;        //loop
};


#endif /* GESTORSONIDO_H */