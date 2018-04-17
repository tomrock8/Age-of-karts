#include "alut.h"
#include "al.h"
#include "alc.h"
#include <iostream>

class AlSource{
    public:
        AlSource();
        void play(int buffer);
        void stop(int buffer);
        ~AlSource();

    private:
        int sourceId;
        ALuint sourceAux;
};
