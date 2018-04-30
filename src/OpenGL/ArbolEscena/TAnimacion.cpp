#include "TAnimacion.hpp"
#include "Timer.hpp"

TAnimacion::TAnimacion(std::vector<TMalla *> frames) {
	TAnimacion(frames, 0, frames.size());
}



TAnimacion::TAnimacion(std::vector<TMalla *> frames, int firstFrame, int lastFrame) {
	this->frames = frames; 
	totalFrames = frames.size();
	setFirstFrame(firstFrame);
	setLastFrame(lastFrame);

	actualFrame = this->firstFrame;
	framesPerSecond = 24; // Valor que se recoge de blender
	setVisible(true);
	setLoopPlay(false);
}

TAnimacion::~TAnimacion(){
	
}

void TAnimacion::beginDraw(Shader *shader) {
	if(visible)
		draw(shader);
}


void TAnimacion::draw(Shader *shader) {
/*
	for (GLuint i = 0; i < malla.size(); i++) {
		//Pasamos la modelMatrix de la malla al shader para calcular la posicion final
		shader->setMat4("model", modelMatrix);
		//Activamos el buffer VAO donde hemos guardado los datos de la malla anteriormente
		malla.at(i)->getMesh()->activeVAO();
		if (malla.at(i)->getText()->getNombre() != NULL) {
			//Activamos la textura de la malla
			malla.at(i)->getText()->activeTexture(shader);
		}

		//Activamos el material
		malla.at(i)->getMat()->activeMaterial(shader);
		// Se llama al dibujado de la malla
		malla.at(i)->getMesh()->draw();

		if (malla.at(i)->getText()->getNombre() != NULL) {
			//Desactivamos las texturas usadas
			malla.at(i)->getText()->disableTexture();
		}

		//Desacivamos el buffer VAO antes del dibujado de la siguiente malla
		malla.at(i)->getMesh()->disableVAO();
	}
*/
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

void TAnimacion::setLoopPlay(bool loopPlay) {
	this->loopPlay = loopPlay;
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