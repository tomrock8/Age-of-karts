
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
        static GestorSonido *getInstacia();
        GestorSonido();
        int cargarSonido(std::string file);
        void setListenerData();
        bool getSonidoCargado();
        ~GestorSonido();

    private:
        static GestorSonido *instancia;
        bool cargado;
        ALCcontext *context;
        ALCdevice *device;
        std::vector<int> buffers;
        unsigned int bufferAux;
        ALvoid*     alBuffer;             //data for the buffer
        ALenum      alFormatBuffer;    //buffer format
        ALsizei     alFreqBuffer;       //frequency     
        ALsizei     alBufferLen;        //bit depth 
        ALboolean   alLoop = AL_FALSE;        //loop
};
