#include "CorredorRed.hpp"


CorredorRed::CorredorRed(stringw rutaObj, vector3df pos) : Corredor(rutaObj, pos)
{

}

void CorredorRed::update(){
    
}

void CorredorRed::movimiento(){
    EstadosJugador *estados = this->getEstados();
    frenodemano(false);
    switch (estados->getEstadoMovimiento()){

        case 0:
		break;
		case 1:
        acelerar();
		break;
		case 2:
        frenar();
		break;
		case 3:
        frenar();
		break;
		case 4:
		frenodemano(true);
		break;
		case 5:
		desacelerar();
		break;
    }

    switch (estados->getDireccionMovimiento()){

		case 0:
		break;
		case 1:
        girarIzquierda();
		break;
		case 2:
        girarDerecha();   
		break;
    }
    
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