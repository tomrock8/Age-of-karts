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

void AlSource::volume(float vol){
    alSourcef(sourceId, AL_GAIN, vol);;
}

bool AlSource::isPlaying()
{
    ALenum state;
    alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

int AlSource::getSourceId(){
    return sourceId;
}

AlSource::~AlSource(){
    alDeleteSources(1, &sourceAux);
}