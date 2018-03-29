#include "hud.hpp"
 
//Constructor que crea el hud asignandole un nombre
hud::hud(const char* n){
    nombre = n;
}
 
//Funcion para añadir un elemento al hud - se le pasa el ancho, alto, nombre y ruta de imagen del elementoHud
void hud::addElement(float w, float h, const char* n, const char* p){
    //Creamos un elemento con las caracteristicas deseadas
    elementoHud *e = new elementoHud(w,h,n,p);
 
    //Lo añadimos al array de elementos
    elems.push_back(e);
}
 
//Funcion para dibujar el hud
void hud::drawHud(Shader *shader){
    //Recorremos el array de elementos, dibujando cada uno de ellos
    for (int i = 0; i < elems.size(); i++){
        //Llamamos al dibujado de cada elementoHud
        elems.at(i)->draw(shader);
    }
}
 
//Funcion para trasladar un elemento a partir de su nombre y el punto de traslacion
void hud::traslateElement(const char* n, float x, float y){
    //Trasladamos el elemento
    getElement(n)->traslacion(x,y);
}
 
//Funcion para escalar un elemento a partir de su nombre y los indices de escalado
void hud::scaleElement(const char* n, float x, float y){
    //Escalamos el elemento
    getElement(n)->escalado(x,y);
}
 
//Funcion para rotar un elemento a partir de su nombre y el angulo de rotacion
void hud::rotateElement(const char* n, float angle){
    //Rotamos el elemento
    getElement(n)->rotacion(angle);
}
 
//Funcion que devuelve un elemento a partir de su nombre
elementoHud* hud::getElement(const char* n){
    elementoHud* e = NULL;
    //Recorremos el array de elementos, buscando el elemento con el nombre especificado
    for (int i = 0; i < elems.size(); i++){
       if (strcmp(elems.at(i)->getName(),n) == 0){
           e = elems.at(i);
       }
    }
    return e;
}
 
//Funcion para retornar el nombre
const char* hud::getName(){
    return nombre;
}