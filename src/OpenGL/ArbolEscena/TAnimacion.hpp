#ifndef TANIMACION_H
#define TANIMACION_H

#include <vector>
#include "TEntidad.hpp"

class TMalla;
class Shader;

class TAnimacion : public TEntidad
{
public:
	TAnimacion(std::vector<TMalla*> frames);
	TAnimacion(std::vector<TMalla*> frames, int firstFrame, int lastFrame);
	~TAnimacion() override;
	void ResetAnimation();

	// METODOS GET
	int getActualFrame();
	int getTotalFrames();
	bool isVisible();

	//METODOS SET
	void setPlaying(bool s);
	bool setPlaying();
	void setFirstFrame(int firstFrame);
	void setLastFrame(int lastFrame);
	void setVisible(bool visible);


	// DIBUJADO
	virtual void beginDraw(Shader* shader) override;
	virtual void endDraw() override;
	void draw(Shader* shader);

private:
	std::vector<TMalla*> frames; // Todos los frames de la animacion

	int lastFramePlayed; // Tiempo del ultimo frame reproducido
	unsigned short framesPerSecond; // Valor temporal para cambio de animaciones
	unsigned short actualFrame; // Valor actual del frame que esta reproduciendo
	unsigned short totalFrames; // Numero total de frames de la animacion
	unsigned short firstFrame; // Primer fotograma a reproducir
	unsigned short lastFrame; // Ultimo frame a reproducir

	bool visible; // Indicador para dibujar
	bool isPlaying; //Indicador para empezar a dibujar
	bool endAnimation; //controlar que no entre en bucle
};
#endif