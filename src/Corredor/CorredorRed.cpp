#include "CorredorRed.hpp"


CorredorRed::CorredorRed(stringw rutaObj, vector3df pos) : Corredor(rutaObj, pos)
{

}

void CorredorRed::update(){
    
}

void CorredorRed::acelerarRed(){
	acelerar();
}

void CorredorRed::frenarRed(){
    frenar();
}
void CorredorRed::girarDRed(){
    girarDerecha();
}

void CorredorRed::girarIRed(){
    girarIzquierda();
}