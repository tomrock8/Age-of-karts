#ifndef ALSOURCE_H
#define ALSOURCE_H

#include "alut.h"
#include "al.h"
#include "alc.h"
#include <iostream>

class AlSource{
    public:
        AlSource();
        void play(int buffer);
        void volume(float vol);
        void setLoop(bool loop);
        void setPitch(float change);
        void setPosition(float *pos);
        bool isPlaying();
        void stop(int buffer);
        int getSourceId();
        ~AlSource();

    private:
        int sourceId;
        ALuint sourceAux;
};


#endif /* ALSOURCE */