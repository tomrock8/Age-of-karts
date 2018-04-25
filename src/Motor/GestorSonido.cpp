#include "GestorSonido.hpp"


GestorSonido::GestorSonido() {
	//inicializamos alut y crea el contexto
	device = alcOpenDevice(NULL);
	if (device == NULL)	{
		// Handle Exception
		cout << "DEVICE IS NULL\n";
	}
	//Create a context
	context = alcCreateContext(device, NULL);

	//Set active context
	alcMakeContextCurrent(context);

	// Clear Error Code
	alGetError();

	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);

	//Cargamos todos los sonidos al inicializar el gestor
	int i;
	i = cargarSonido("assets/Sounds/bounce.wav");
	i = cargarSonido("assets/Sounds/sound.wav");
	i = cargarSonido("assets/Sounds/menu.wav");
	i = cargarSonido("assets/Sounds/car_engine.wav");
	i = cargarSonido("assets/Sounds/throw.wav");
	i = cargarSonido("assets/Sounds/race1.wav");

	//Iniciamos los volumenes por defecto
	volGeneral = 1.0f;
	volMusica = 1.0f;
	volEfectos = 1.0f;
}

int GestorSonido::cargarSonido(std::string file) {
	const char * c = file.c_str();
	//cargamos el archivo de audio
	alutLoadWAVFile((ALbyte*)c, &alFormatBuffer, &alBuffer, &alBufferLen, &alFreqBuffer, &alLoop);
	alGenBuffers(1, &bufferAux);
	//cargamos el audio en el buffer
	alBufferData(bufferAux, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);

	//liberamos la  memoria que usamos para cargar el audio
	alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
	int buffer = bufferAux;
	buffers.push_back(buffer);
	if (alGetError() != AL_NO_ERROR) {
		std::cout << "ERROR AL LEER LOS SONIDOS\n";
		return 0;
	}

	return buffer;
}
/*
int GestorSonido::cargarFuente(){
	sourceAux = new AlSource();
	sources.push_back(sourceAux);

	return (int)sourceAux;
}

std::vector<*AlSource> GestorSonidogetFuentes(){
	return sources;
}*/

void GestorSonido::setListenerData() {
	alListener3f(AL_POSITION, 4, 4, 24);
	alListener3f(AL_VELOCITY, 0, 0, 0);
}

bool GestorSonido::getSonidoCargado() {
	return cargado;
}

float GestorSonido::getVolGeneral(){
	return volGeneral;
}

float GestorSonido::getVolMusica(){
	return volMusica;
}

float GestorSonido::getVolEfectos(){
	return volEfectos;
}

void GestorSonido::setVolGeneral(float vol){
	volGeneral = vol;
}

void GestorSonido::setVolMusica(float vol){
	volMusica = vol;
}

void GestorSonido::setVolEfectos(float vol){
	volEfectos = vol;
}

GestorSonido::~GestorSonido() {
	for (int i = 0; i < buffers.size(); i++) {
		alDeleteBuffers(buffers.at(i), &bufferAux);
	}

	context = alcGetCurrentContext();

	//Get device for active context
	device = alcGetContextsDevice(context);

	//Disable context
	alcMakeContextCurrent(NULL);
	//Release context(s)
	alcDestroyContext(context);

	//Close device
	alcCloseDevice(device);
}