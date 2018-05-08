#include "TAnimacion.hpp"
#include "Timer.hpp"

TAnimacion::TAnimacion(std::vector<TMalla *> frames) {
	TAnimacion(frames, 0, frames.size());
	visible = true;
	isPlaying = false;
}



TAnimacion::TAnimacion(std::vector<TMalla *> frames, int firstFrame, int lastFrame) {
	this->frames = frames;
	totalFrames = frames.size();
	setFirstFrame(firstFrame);
	setLastFrame(lastFrame);

	actualFrame = this->firstFrame;
	framesPerSecond = 60; // Valor que se recoge de blender
	setVisible(true);
	
}

TAnimacion::~TAnimacion() {
	// Eliminacion de todos los frames del array
	for (int i = 0; i < frames.size(); i++) {
		TMalla *a = frames.at(i);
		delete a;
	}
	frames.clear();
}

void TAnimacion::beginDraw(Shader *shader) {
	if (visible) draw(shader);
}


void TAnimacion::draw(Shader *shader) {
	// Reproducir el frame actual
	if (isPlaying) {
		frames.at(actualFrame)->draw(shader);

		// Comprobar si hay que cambiar de frame
		//int framesInSeconds = framesPerSecond;
		//if (Timer::getInstancia()->getTimer() / framesInSeconds > 0) {
			// Aumentar el frame actual si no es el ultimo si le toca
		if (actualFrame < totalFrames - 1) {
			actualFrame++;
		}
		else {
			// Si el frame actual es el ultimo && loop => frame actual = primero
			//actualFrame = firstFrame;
			isPlaying = false;
		}
	}

	// Guardar el tiempo ultimo de reproduccion    
	//lastFramePlayed = Timer::getInstancia()->getTimer();
}


void TAnimacion::endDraw() {}

// METODOS SET
void TAnimacion::setFirstFrame(int firstFrame) {
	// Comprobacion de un valor que este entre el primer frame y el ultimo
	if (firstFrame > 0 && firstFrame < this->totalFrames)
		this->firstFrame = static_cast<unsigned short>(firstFrame);
	else
		this->firstFrame = 0;
}

void TAnimacion::setLastFrame(int lastFrame) {
	// Comprobacion de un valor que este entre el primer frame y el ultimo
	if (lastFrame > 0 && lastFrame < this->totalFrames)
		this->lastFrame = static_cast<unsigned short>(lastFrame);
	else
		this->lastFrame = static_cast<unsigned short>(frames.size());
}



void TAnimacion::setVisible(bool visible) {
	this->visible = visible;
}

// METODOS GET
bool TAnimacion::isVisible() {
	return visible;
}

int TAnimacion::getActualFrame() {
	return static_cast<int>(this->actualFrame);
}

int TAnimacion::getTotalFrames() {
	return static_cast<int>(this->totalFrames);
}
void TAnimacion::setPlaying(bool s) {
	isPlaying = s;
}
bool TAnimacion::setPlaying() {
	return isPlaying;
}
void TAnimacion::ResetAnimation() {
	actualFrame = firstFrame;
	isPlaying = true;
}