#include "GestorCarrera.hpp"

GestorCarrera::GestorCarrera(){
    vueltas=3;
    jugadores = GestorJugadores::getInstancia();
    pj1 = jugadores->getJugadores();
    jugadores->setJugadores(pj1);
    pj2= new Corredor*[6];
    
}
void GestorCarrera::update(){
    for (int j = 0; j < jugadores->getNumJugadores(); j++) {
        if (pj1[j] != NULL) {
            pj1[j]->setPosicionCarrera(pj1[j]->getNodo()->getID()+1);   //Asignamos para empezar la carrera las posiciones de los corredores en parrilla, en función de las ids.
        }
        pj2[j]=pj1[j];
    }
    int cont=0;
    pj_aux=NULL;
    //ordenamos el array de jugadores en pj2, en funcion de sus waypoints de mayor a menor
    for (int j = 0; j < jugadores->getNumJugadores()-1; j++) {
        for (int k = 0; k < jugadores->getNumJugadores()-1; k++) {
                if (pj2[k]->getWaypointActual()->getWaypoint()->getID()<pj2[k+1]->getWaypointActual()->getWaypoint()->getID()){
                    pj_aux=pj2[k];
                    pj2[k]=pj2[k+1];
                    pj2[k+1]=pj_aux;
                }
        }   
    }
    int aux=0;
    cont=0;
    int x=-1;
    int y=-1;
    for (int num=vueltas;num>=1;num--){
        pj_aux=NULL;
        x=-1;
        y=-1;
        
        //una vez ordenados por waypoints pasamos a clasificarlos, distinguiendo distancias entre waypoints en caso de ambiguedad (si estan el = waypoint)
        for (int i = 0; i < jugadores->getNumJugadores(); i++) {            
            y=getCorredorIndexOriginal(i);          //cogemos el mismo corredor correspondiente en el array original pj1 (asociar pj1<-->pj2)
            cont =jugadores->getNumJugadores();
            if (num==pj1[y]->getVueltas()){
                aux++;
                for (int j = 0; j < jugadores->getNumJugadores(); j++) {
                    
                    if (i!=j){      //si no es el mismo entra
                        x=getCorredorIndexOriginal(j);  //hacemos lo mismo para el bucle actual (asociar pj1<-->pj2)
                        if (num==pj1[x]->getVueltas()){
                            if (pj1[x]->getWaypointActual()->getWaypoint()->getID()==pj1[y]->getWaypointActual()->getWaypoint()->getID()){  //si estan en el mismo waypoint
                                if (pj1[x]->getdistanciaWaypoint()>pj1[y]->getdistanciaWaypoint()){     //si la distancia del waypoint pj1[y] es menor, su posicion en carrera aumenta (cont--)
                                    cont--;                 
                                }
                            }else{          //no estan en el mismo waypoint
                                if (i<j){   //si el waypoint de i es mayor (i mas pequenya) entra
                                    if (pj1[y]->getWaypointAnterior()->getWaypoint()->getID()==pj1[x]->getWaypointActual()->getWaypoint()->getID()){    //si un corredor esta un waypoint por detras que otro entra
                                        if (pj1[y]->getdistanciaWaypointAnterior()>pj1[x]->getdistanciaWaypointActual()){       //si ademas el corredor1 esta mas lejos del waypoint anterior que el corredor2 de su waypoint actual aumentamos su posicion en carrera (en caso contrario no)
                                            cont--;
                                        }
                                    }else{      //sino al i tener waypoint mayor aumentamos posicion de carrera
                                        cont--;
                                    }
                                    
                                }
                            }
                        }else if (num>pj1[x]->getVueltas()){
                            cont--;
                        }
                    }
                }
                
           
            pj1[y]->setPosicionCarrera(cont);       //modificamos posicion en pj1[y]
            }
        }
  
    }
}

int GestorCarrera::getCorredorIndexOriginal(int n){
        for (int i = 0; i < jugadores->getNumJugadores(); i++) {
            if (pj2[n]->getNodo()->getID()==pj1[i]->getNodo()->getID()){
                return i;
            }

        }
    
        return -1;
}
void GestorCarrera::setVueltas(int i){
    vueltas=i;
}
int GestorCarrera::getVueltas(){
    return vueltas;
}
bool GestorCarrera::comprobarItemTeledirigido(int num){ //aplicar el teledirigido al siguiente corredor
    for (int i = 0; i < jugadores->getNumJugadores(); i++) {
        if (num!=1){
             if (pj1[i]->getPosicionCarrera()==num-1){
				pj1[i]->setObjetivoTelederigido();
                return true;
            }
        }
    } 
    return false;
}
