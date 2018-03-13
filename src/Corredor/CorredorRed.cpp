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
		/*
		Estado_movimiento
			0. Quieto
			1. Avanza
			2. Marcha atras
			3. Frena
			4. Derrapa
			5. Desacelera
		*/
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
	/*
	Direccion_Movimiento
		0. Recto
		1. Izquierda
		2. Derecha
	*/
	
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
	
	/*
	Estado_Objeto
		0. Nada
		1. Flecha
		2. CajaFalsa
		3. Turbo
		4. Aceite
		5. Escudo
		6. Flecha_Triple
		7. Item Teledirigido
		8. Habilidad?
	*/
	int estado_obj = estados->getEstadoObjeto();
	if (getTipoObj() != estado_obj) {
		setTipoObj(estado_obj);
	}

	/*
	Estado Coche
		0. Por_defecto
		1. Con_turbo
		2. Con_escudo
	*/

	/*
	Estado_carreda
		0. Parrilla
		1. Carrera
		2. Fin
	*/

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