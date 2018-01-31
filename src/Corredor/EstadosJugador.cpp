#include "EstadosJugador.hpp"

EstadosJugador::EstadosJugador(){
    estado=QUIETO;
    direccion=RECTO;
    objeto=NADA;
    estadoCoche=POR_DEFECTO;
}
void EstadosJugador::update(){
    TextoPantalla *texto =TextoPantalla::getInstancia();
	texto->agregar("----------------------");
    texto->agregar("ESTADO MOVIMIENTO: ");
    switch (estado){

        case 0:
	    texto->agregar("QUIETO\n");
		break;
		case 1:
        texto->agregar("AVANZA\n");
		break;
		case 2:
        texto->agregar("MARCHA_ATRAS\n");
		break;
		case 3:
		texto->agregar("FRENA\n");
		break;
		case 4:
        texto->agregar("DERRAPA\n");
		break;
		case 5:
        texto->agregar("DESACELERA\n");
		break;

	}
    texto->agregar("DIRECCION MOVIMIENTO: ");
    switch (direccion){

		case 0:
        texto->agregar("RECTO\n");
		break;
		case 1:
        texto->agregar("IZQUIERDA\n");
		break;
		case 2:
        texto->agregar("DERECHA\n");        
		break;
    }
    texto->agregar("OBJETO ACTUAL: ");
	
    switch (objeto){

		case 0:
        texto->agregar("NADA\n");
		break;
		case 1:
        texto->agregar("FLECHA\n");
		break;
		case 2:
        texto->agregar("CAJA_FALSA\n");
		break;
        case 3:
		texto->agregar("TURBO\n");
		break;
        case 4:
        texto->agregar("ACEITE\n");
		break;
        case 5:
        texto->agregar("ESCUDO\n");
		break;
		case 6:
		texto->agregar("FLECHA TRIPLE\n");
		break;
		case 8:
		texto->agregar("HABILIDAD\n");
		break;
		case 7:
		texto->agregar("ITEM_TELEDIRIGIDO\n");
		break;
		
    }

    texto->agregar("ESTADO COCHE: ");
    switch (estadoCoche){
		case 0:
        texto->agregar("POR_DEFECTO\n");
		break;
		case 1:
        texto->agregar("CON_TURBO\n");
		break;
		case 2:
        texto->agregar("CON_ESCUDO\n");
		break;
    }
   
}
void EstadosJugador::setEstadoMovimiento(estado_movimiento estado_mov){
    estado=estado_mov;
}
void EstadosJugador::setEstadoMovimiento(int estado_mov){
    switch (estado_mov){

        case 0:
        estado = QUIETO;
		break;
		case 1:
        estado = AVANZA;
		break;
		case 2:
        estado = MARCHA_ATRAS;
		break;
		case 3:
        estado = FRENA;
		break;
		case 4:
        estado = DERRAPA;
		break;
		case 5:
        estado = DESACELERA;
		break;

	}
}
estado_movimiento EstadosJugador::getEstadoMovimiento(){
    return estado;
}
void EstadosJugador::setDireccionMovimiento(direccion_movimiento direccion_mov){
    direccion=direccion_mov;
}
void EstadosJugador::setDireccionMovimiento(int direccion_mov){
    switch (direccion_mov){

        case 0:
        direccion = RECTO;
		break;
		case 1:
        direccion = IZQUIERDA;
		break;
		case 2:
        direccion = DERECHA;
		break;

	}
}
direccion_movimiento EstadosJugador::getDireccionMovimiento(){
    return direccion;
}
void EstadosJugador::setEstadoObjeto(estado_objeto estado_obj){
    objeto=estado_obj;
}
void EstadosJugador::setEstadoObjeto(int estado_obj){
    switch (estado_obj){

        case 0:
        objeto = NADA;
		break;
		case 1:
        objeto = FLECHA;
		break;
		case 2:
        objeto = CAJA_FALSA;
		break;
		case 3:
        objeto = TURBO;
		break;
		case 4:
        objeto = ACEITE;
		break;
		case 5:
        objeto = ESCUDO;
		break;
		case 6:
        objeto = FLECHA_TRIPLE;
		break;
		case 8:
		objeto = HABILIDAD;
		break;
		case 7:
		objeto = ITEM_TELEDIRIGIDO;
		break;

	}
}
estado_objeto EstadosJugador::getEstadoObjeto(){
    return objeto;
}
 void EstadosJugador::setEstadoCoche(estado_coche coche){
    estadoCoche=coche;
}
 void EstadosJugador::setEstadoCoche(int coche){
    switch (coche){

        case 0:
        estadoCoche = POR_DEFECTO;
		break;
		case 1:
        estadoCoche = CON_TURBO;
		break;
		case 2:
        estadoCoche = CON_ESCUDO;
		break;

	}
}
estado_coche EstadosJugador::getEstadoCoche(){
    return estadoCoche;
}