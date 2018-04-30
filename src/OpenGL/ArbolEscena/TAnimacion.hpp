#ifndef TANIMACION_H
#define TANIMACION_H

#include <vector>
#include "TEntidad.hpp"
#include "TMalla.hpp"

class TAnimacion : public TEntidad
{
public:
	TAnimacion(std::vector<TMalla *> frames, int firstFrame, int lastFrame);
	~TAnimacion();

	// METODOS GET
	int getActualFrame();
	int getTotalFrames();
	bool isVisible();

	//METODOS SET
	void setFirstFrame(int firstFrame);
	void setLastFrame(int lastFrame);
	void setVisible(bool visible);

	// DIBUJADO
	virtual void beginDraw(Shader *shader) override;
	virtual void endDraw() override;
	void draw(Shader *shader);

private:
	std::vector<TMalla *> frames; // Todos los frames de la animacion
	
	int lastFramePlayed; // Tiempo del ultimo frame reproducido
	unsigned short framesPerSecond; // Valor temporal para cambio de animaciones
	unsigned short actualFrame; // Valor actual del frame que esta leyendo
	unsigned short totalFrames; // Numero total de frames de la animacion
	unsigned short firstFrame; // Primer fotograma a reproducir
	unsigned short lastFrame; // Ultimo frame a reproducir
	
	bool visible; // Indicador para dibujar
	bool loopPlay; // Indicador para reiniciar la animacion


};
#endif