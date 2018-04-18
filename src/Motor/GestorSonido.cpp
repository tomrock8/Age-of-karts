#include "GestorSonido.hpp"

GestorSonido *GestorSonido::instancia = NULL;

GestorSonido * GestorSonido::getInstacia() {
	if (instancia == NULL)
		instancia = new GestorSonido();

	return instancia;
}

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

	//Cargamos todos los sonidos al inicializar el gestor
	int i;
	i = cargarSonido("assets/bounce.wav");
	i = cargarSonido("assets/sound.wav");
	i = cargarSonido("assets/menu.wav");
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
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);
}

bool GestorSonido::getSonidoCargado() {
	return cargado;
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