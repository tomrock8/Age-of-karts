#include "AlSource.hpp"

AlSource::AlSource(){
    alGenSources(1, &sourceAux);
    sourceId = sourceAux;
    //AlSourcef(sourceId, AL_GAIN, 1);
    //AlSourcef(sourceId, AL_PITCH, 1);
    //AlSource3f(sourceId, AL_POSITION, 0,0,0);
}

void AlSource::play(int buffer){
    alSourcei(sourceId, AL_BUFFER, buffer);
    alSourcePlay(sourceId);
}

void AlSource::stop(int buffer){
    alSourcei(sourceId, AL_BUFFER, buffer);
    alSourceStop(sourceId);
}

AlSource::~AlSource(){
    alDeleteSources(1, &sourceAux);
}