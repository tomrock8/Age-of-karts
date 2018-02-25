#include "CorredorRed.hpp"


CorredorRed::CorredorRed(stringw rutaObj, btVector3 pos,Corredor::tipo_jugador tipo) : Corredor(rutaObj, pos, tipo)
{
	nombre="JugadorRed";
	cuboNodo->setName(nombre);
}


void CorredorRed::updateHijos(){
	
}

void CorredorRed::movimiento(){
    //std::cout << "Entro\n";
    EstadosJugador *estados = this->getEstados();
	int estadoMovimiento = estados->getEstadoMovimiento();
	int direccionMovimiento = estados->getDireccionMovimiento();
	//cout <<"Estado del jugador: "<<estadoMovimiento<<endl;
	//cout <<"Direccion del jugador: "<<direccionMovimiento<<endl;
    frenodemano(false, false);
    switch (estadoMovimiento){

        case 0:
        //std::cout << "Esta quieto\n";
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
		frenodemano(true, true);
		break;
		case 5:
		desacelerar();
		break;
    }

	
    switch (direccionMovimiento){

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

void CorredorRed::actualizarItem(){
	
}