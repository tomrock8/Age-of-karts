#include "AlSource.hpp"

AlSource::AlSource() {
	alGenSources(1, &sourceAux);
	sourceId = sourceAux;
	alSourcef(sourceId, AL_GAIN, 1);
	alSourcef(sourceId, AL_PITCH, 1);
	alSource3f(sourceId, AL_POSITION, 0,0,0);
}

void AlSource::play(int buffer) {
	alSourcei(sourceId, AL_BUFFER, buffer);
	alSourcePlay(sourceId);
}

void AlSource::stop(int buffer) {
	alSourcei(sourceId, AL_BUFFER, buffer);
	alSourceStop(sourceId);
}

void AlSource::volume(float vol) {
	alSourcef(sourceId, AL_GAIN, vol);;
}

void AlSource::setLoop(bool loop){
	if(loop){
    	alSourcei(sourceId,AL_LOOPING,AL_TRUE);
	} else {
    	alSourcei(sourceId,AL_LOOPING,AL_FALSE);
	}
}

void AlSource::setPitch(float change){
	alSourcef(sourceId, AL_PITCH, change);
}

bool AlSource::isPlaying()
{
	ALenum state;
	alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

int AlSource::getSourceId() {
	return sourceId;
}

AlSource::~AlSource() {
	alDeleteSources(1, &sourceAux);
}