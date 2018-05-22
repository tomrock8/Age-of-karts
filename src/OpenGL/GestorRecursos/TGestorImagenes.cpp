#include "TGestorImagenes.hpp"

TGestorImagenes *TGestorImagenes::instancia = NULL;

//Funcion que devuelve el gestor de imagenes. Si es la primera vez que de accede, lo inicializa y lo devuelve
TGestorImagenes *TGestorImagenes::getInstancia() {
	if (instancia == NULL)
		instancia = new TGestorImagenes();

	return instancia;
}

TGestorImagenes::TGestorImagenes(){}


TGestorImagenes::~TGestorImagenes(){
       
        if(images.size()>0){

            for(int i = 0; i< images.size();i++){
              
                delete images[i];
            }
            images.clear();
        }
       instancia=nullptr; //Un unico gestor de imagenes por cada ejecucion del programa

}

TRecursoImagen *TGestorImagenes::cargarImagen(const char* path){
    TRecursoImagen *image = NULL; //Recurso imagen a devolver
    bool isHere = false; //Booleano para comprobar si la imagen ya esta creada en el gestor

    if (images.size() > 0){ //Si el array de recursos imagen no esta vacio...
        //Bucle para recorrer todas la imagenes del array de TRecursoImagen
        for ( int i = 0; i < images.size(); i++){
            //Si la ruta coincide con una de las imagenes ya guardadas...
            if (strcmp(images.at(i)->getPath(), path) == 0){
                //Recuperamos la imagen para devolverla
                image = images.at(i);
                isHere = true; //true == imagen encontrada
            }
        }
    }

    //Si la imagen no ha sido encontrada...
    if (isHere == false){
        //Creamos la imagen
        image = new TRecursoImagen(path);
        //Guardamos la nueva imagen en el vector 
        images.push_back(image);
    }

    return image;
}