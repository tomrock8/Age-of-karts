#include "GestorCarrera.hpp"

GestorCarrera::GestorCarrera() {
	vueltas = 1;
	jugadores = GestorJugadores::getInstancia();
	pj1 = jugadores->getJugadores();
	jugadores->setJugadores(pj1);
	for (int j = 0; j < jugadores->getNumJugadores(); j++)
	pj1.at(j)->setMaxVueltas(vueltas);
	acum=1;
}
bool GestorCarrera::update() {

	
	bool b=true;	//creamos un booleano para devolver si se ha terminado la partida

	//Primero copiamos pj1 en pj2 (vector de jugadores)
	pj2.resize(pj1.size());
	for (int j = 0; j < jugadores->getNumJugadores(); j++) {
		pj1.at(j)->setPosicionCarrera(pj1.at(j)->getNodo()->getID() + 1);   //Asignamos para empezar la carrera las posiciones de los corredores en parrilla, en función de las ids.
		pj2.at(j) = pj1.at(j);

		if (pj1.at(j)->getVueltas()<=vueltas){	//comprobamos si todos los corredores han terminado
			b=false;	
		}
	}
	if (b==true){	
		return b;  //Todos los corredores han terminado, no es necesario seguir haciendo update. Devolvemos al menu.
	}

	int cont = 0;
	pj_aux = NULL;
	//ordenamos el array de jugadores en pj2, en funcion de sus waypoints de mayor a menor
	for (int j = 0; j < jugadores->getNumJugadores() - 1; j++) {
		for (int k = 0; k < jugadores->getNumJugadores() - 1; k++) {
			if (pj2.at(k)->getWaypointActual()->getWaypoint()->getID() < pj2.at(k + 1)->getWaypointActual()->getWaypoint()->getID()) {
				pj_aux = pj2.at(k);
				pj2.at(k) = pj2.at(k + 1);
				pj2.at(k + 1) = pj_aux;
			}else if (pj2.at(k)->getWaypointActual()->getWaypoint()->getID() == pj2.at(k+1)->getWaypointActual()->getWaypoint()->getID()) {  //si estan en el mismo waypoint
				if (pj2.at(k)->getdistanciaWaypoint() > pj2.at(k+1)->getdistanciaWaypoint()) {     //si la distancia del waypoint pj1[y] es menor, su posicion en carrera aumenta (cont--)
					pj_aux = pj2.at(k);
					pj2.at(k) = pj2.at(k + 1);
					pj2.at(k + 1) = pj_aux;
				}
			}
		}
	}
	cont = acum;
	int y = -1;
	acum=1;
	for (int num = vueltas; num >= 1; num--) {
		y = -1;

		//una vez ordenados por waypoints pasamos a clasificarlos, distinguiendo distancias entre waypoints en caso de ambiguedad (si estan el = waypoint)
		for (int i = 0; i < jugadores->getNumJugadores(); i++) {
			y = getCorredorIndexOriginal(i);					//cogemos el mismo corredor correspondiente en el array original pj1 (asociar pj1<-->pj2)
			if (pj1.at(y)->getVueltas()>vueltas && acum<jugadores->getNumJugadores()){		//controlamos el numero de jugadores 
				acum++;
			}
			if (pj1.at(y)->getVueltas()==num){				//para esa vuelta comprobamos los corredores									
				pj1.at(y)->setPosicionCarrera(cont);       //modificamos posicion en pj1[y]
				cont++;
			}
			
			
		}
	}
}

int GestorCarrera::getCorredorIndexOriginal(int n) {
	for (int i = 0; i < jugadores->getNumJugadores(); i++) {
		if (pj2.at(n)->getNodo()->getID() == pj1.at(i)->getNodo()->getID()) {
			return i;
		}
	}

	return -1;
}

void GestorCarrera::setVueltas(int i) {
	vueltas = i;
}

int GestorCarrera::getVueltas() {
	return vueltas;
}
void GestorCarrera::comprobarItemTeledirigido(int num){ //aplicar el teledirigido al siguiente corredor
    for (int i = 0; i < jugadores->getNumJugadores(); i++) {
        if (num!=1){
             if (pj1.at(i)->getPosicionCarrera()==num-1){
				pj1.at(i)->setObjetivoTelederigido();
                //return true;
            }
        }
    } 
}
