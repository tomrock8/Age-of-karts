#include "alut.h"
#include "al.h"
#include "alc.h"
#include <iostream>

class AlSource{
    public:
        AlSource();
        void play(int buffer);
        void volume(float vol);
        bool isPlaying();
        void stop(int buffer);
        int getSourceId();
        ~AlSource();

    private:
        int sourceId;
        ALuint sourceAux;
};
