#ifndef GESTORSONIDO_H
#define GESTORSONIDO_H


#include <iostream>
#include <vector>
#include "AlSource.hpp"
#include "SoundEnum.hpp"
#include "alut.h"
#include "al.h"
#include "alc.h"

using namespace std;

class GestorSonido{
    public:
        GestorSonido();
        int cargarSonido(std::string file);
        void setPosition(float *pos);
        //int cargarFuente();
        //std::vector<*AlSource> getFuentes();
        void setListenerData();
        bool getSonidoCargado();
        float getVolGeneral();
        float getVolMusica();
        float getVolEfectos();
        void setVolGeneral(float vol);
        void setVolMusica(float vol);
        void setVolEfectos(float vol);
        ~GestorSonido();

    private:
        bool cargado;
        float volGeneral;
        float volMusica;
        float volEfectos;
        ALCcontext *context;
        ALCdevice *device;
        std::vector<int> buffers;
        std::vector<AlSource> sources;
        AlSource *sourceAux;
        unsigned int bufferAux;
        ALvoid*     alBuffer;             //data for the buffer
        ALenum      alFormatBuffer;    //buffer format
        ALsizei     alFreqBuffer;       //frequency     
        ALsizei     alBufferLen;        //bit depth 
        ALboolean   alLoop = AL_FALSE;        //loop
};


#endif /* GESTORSONIDO_H */