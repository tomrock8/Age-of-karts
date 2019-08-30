#ifndef TMALLA_H
#define TMALLA_H

#include <vector>
#include "TEntidad.hpp"

class Shader;
class mesh;
class boundingBox;

class TMalla : public TEntidad {
public:
	TMalla(std::vector<mesh*> m);
	~TMalla() override;

	// VISIBILIDAD DEL OBJETO
	bool isVisible();
	void setVisible(bool visible);

	// DIBUJADO
	virtual void beginDraw(Shader* shader) override;
	virtual void endDraw() override;
	void draw(Shader* shader);

private:
	// --- VARIABLES ---
	std::vector<mesh*> malla; //Vector de mallas
	bool visible; //Visibilidad de la malla
	std::vector<boundingBox*> bBoxes; //Vector de bounding boxes de la malla
};
#endif