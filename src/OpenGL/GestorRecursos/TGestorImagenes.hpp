#ifndef TGESTORIMAGENES_H
#define TGESTORIMAGENES_H

#include <vector>

class TRecursoImagen;

/* Clase para gestionar los recursos imagen utilizados en la creacion del hud */
class TGestorImagenes
{
public:
	TGestorImagenes(); //Constructor del gestor de imagenes
	~TGestorImagenes();
	static TGestorImagenes* getInstancia(); //Para recoger la instancia del gestor de imagenes desde cualquier punto del programa
	TRecursoImagen* cargarImagen(const char* path); //Cargar las distintas imagenes

private:
	static TGestorImagenes* instancia; //Un unico gestor de imagenes por cada ejecucion del programa
	std::vector<TRecursoImagen*> images; //Vector con todas las imagenes que se usan por los distintos hud
};
#endif
