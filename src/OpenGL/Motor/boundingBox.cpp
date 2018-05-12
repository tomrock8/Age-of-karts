#include "boundingBox.hpp"

// Constructor que recibe la posicion y tamanyo del bounding box
boundingBox::boundingBox(glm::vec3 p, glm::vec3 s){
    //Guardamos los datos de posicion y tamanyo
    positionBbox = p;
    sizeBbox = s;
}
