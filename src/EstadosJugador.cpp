#include "EstadosJugador.hpp"

EstadosJugador::EstadosJugador(){
    estado=QUIETO;
    direccion=RECTO;
    objeto=NADA;
    estadoCoche=POR_DEFECTO;
}
void EstadosJugador::update(){
    TextoPantalla *texto =TextoPantalla::getInstancia();
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
estado_movimiento EstadosJugador::getEstadoMovimiento(){
    return estado;
}
void EstadosJugador::setDireccionMovimiento(direccion_movimiento direccion_mov){
    direccion=direccion_mov;
}
direccion_movimiento EstadosJugador::getDireccionMovimiento(){
    return direccion;
}
void EstadosJugador::setEstadoObjeto(estado_objeto estado_obj){
    objeto=estado_obj;
}
estado_objeto EstadosJugador::getEstadoObjeto(){
    return objeto;
}
 void EstadosJugador::setEstadoCoche(estado_coche coche){
    estadoCoche=coche;
}
