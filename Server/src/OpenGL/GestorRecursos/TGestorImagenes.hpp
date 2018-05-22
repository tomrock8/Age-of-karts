#ifndef TGESTORIMAGENES_H
#define TGESTORIMAGENES_H

#include "TRecursoImagen.hpp"
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

/*Clase para gestionar los recursos imagen utilizados en la creacion del hud*/

class TGestorImagenes
{
    public:
        TGestorImagenes(); //Constructor del gestor de imagenes

        static TGestorImagenes *getInstancia(); //Para recoger la instancia del gestor de imagenes desde cualquier punto del programa
        TRecursoImagen *cargarImagen(const char* path); //Cargar las distintas imagenes

    private:
        static TGestorImagenes *instancia; //Un unico gestor de imagenes por cada ejecucion del programa

        std::vector<TRecursoImagen *> images; //Vector con todas las imagenes que se usan por los distintos hud
};
#endif
