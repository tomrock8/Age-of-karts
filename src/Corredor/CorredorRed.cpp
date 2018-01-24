#include "CorredorRed.hpp"


CorredorRed::CorredorRed(stringw rutaObj, vector3df pos) : Corredor(rutaObj, pos)
{

}

void CorredorRed::update(){
    
}

void CorredorRed::movimiento(){
    EstadosJugador *estados = this->getEstados();
    bool comprobadorMovimiento = false;
    frenodemano(false);
    switch (estados->getEstadoMovimiento()){

        case 0:
		break;
		case 1:
        acelerar();
        comprobadorMovimiento = true;
		break;
		case 2:
        frenar();
        comprobadorMovimiento = true;
		break;
		case 3:
        frenar();
        comprobadorMovimiento = true;
		break;
		case 4:
		frenodemano(true);
		comprobadorMovimiento = true;
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