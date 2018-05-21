#ifndef HUD_H
#define HUD_H
 
#include "elementoHud.hpp"
#include <vector>
#include <stdio.h>
#include <string.h>
 
using namespace std; 
 
class hud  
{ 
public: 
    hud(const char* n);
    ~hud();
 
    const char* getName();
 
    void addElement(float w, float h, const char* n, const char* p); //Añade un elemento con textura al hud
	elementoHud* addElementHud(float w, float h, const char* n, const char* p); //Añade un elemento con textura al hud
    void addElement(float w, float h, const char* n, glm::vec4 c); //Añade un elemento con color al hud
	void deleteElement(const char* nombre); // Elimina el elemento del array


    //Tratamiento de los elementosHud
    void rotateElement(const char* n, float angle);
    void traslateElement(const char* n, float x, float y);
    void scaleElement(const char* n, float x, float y);
    void changeTextureElement(const char* n, const char* p);
    void changeColorElement(const char* n, glm::vec4 c);
    void changeTransparencyElement(const char* n, bool t, float l);
 
    void drawHud(Shader *shader); //Dibuja todos los elementos del hud
 
private:
    const char* nombre; //Nombre del hud, por ejemplo "Opciones"
 
    std::vector <elementoHud*> elems; //Array con los elementos que forman el hud
 
    elementoHud* getElement(const char* n); //Funcion que devuelve un elemento a partir de su nombre
	int getElementPosition(const char * n); // Funcion que devuelve la posicion del elemento en el array
};
#endif