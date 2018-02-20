#include "NodoDecision.hpp"



NodoDecision::NodoDecision(){

Decision=false;
Accion=false;
valor=0;
valorBoleano=false;
idNodo=0;
NodoPadre=nullptr;
parametro = NADA3;  
condiciones = NADA2;
tipoaccion=0;  

}

void NodoDecision::decision(vector<NodoDecision*> &arrayNodos,int nodopadre,tipo_nodo tipodecision,tipo_parametro parametro,int valor,bool booleano){

    Decision=true;
    if(arrayNodos.size()==0){
    NodoPadre=nullptr;
    }else{
    NodoPadre = arrayNodos.at(nodopadre);
    NodoPadre->setHijo(this);
    }

    idNodo = arrayNodos.size();
    condiciones=tipodecision;
    this->parametro = parametro;
    this->valor=valor;
    valorBoleano = booleano;
    arrayNodos.push_back(this);

}

void NodoDecision::accion(vector<NodoDecision*> &arrayNodos,int nodopadre,int accionIA){

    Accion=true;
    if(arrayNodos.size()==0){
    NodoPadre=nullptr;
    }else{
    NodoPadre = arrayNodos.at(nodopadre);
    NodoPadre->setHijo(this);
    }
    
    idNodo = arrayNodos.size();
    tipoaccion=accionIA;
    arrayNodos.push_back(this);

}


bool NodoDecision::getConsulta(bool arrayboleanos[], int obj){
    
    bool parametroaux = false;              


    switch(parametro){

        case NADA3:
        break;

        case OBJNUMERO:
        break;

        case OBJBOOLEANO:
        parametroaux= arrayboleanos[15];
        break;

        case DISTANCIACERCA:
        parametroaux= arrayboleanos[0];
        break;

        case DISTANCIAMEDIA:
        parametroaux= arrayboleanos[1];
        break;

        case DISTANCIALEJOS:
        parametroaux= arrayboleanos[2];
        break;

        case VELOCIDADBAJA:
        parametroaux= arrayboleanos[3];
        break;

        case VELOCIDADMEDIA:
        parametroaux= arrayboleanos[4];
        break;

        case VELOCIDADALTA:
        parametroaux= arrayboleanos[5];
        break;

        case NOGIRO:
        parametroaux= arrayboleanos[6];
        break;

        case GIROMEDIODERECHA:
        parametroaux= arrayboleanos[7];
        break;

        case GIROMEDIOIZQUIERDA:
        parametroaux= arrayboleanos[8];
        break;

        case GIROFUERTEDERECHA:
        parametroaux= arrayboleanos[9];
        break;

        case GIROFUERTEIZQUIERDA:
        parametroaux= arrayboleanos[10];
        break;

        case CAJA:
        parametroaux= arrayboleanos[11];
        break;

        case TURBOO:
        parametroaux= arrayboleanos[12];
        break;

        case ENEMIGO:
        parametroaux= arrayboleanos[13];
        break;

        case VISION:
        parametroaux= arrayboleanos[14];
        break;

    }

    
    switch(condiciones){

        case NADA2:
        break;

        case IGUAL:

            if(parametro != OBJNUMERO){

                if(parametroaux==valorBoleano){

                    return true;
                }

            }else{

                if(obj == valor){

                    return true;
                }
            }


        break;

        case DISTINTO:

            if(parametro != OBJNUMERO){

                if(parametroaux!=valorBoleano){

                    return true;
                }

            }else{

                if(obj != valor){

                    return true;
                }
            }

        break;

        case MAYOR:

            if(obj>valor){
                return true;
            }

        break;

        case MAYOROIGUAL:
            
            if(obj>=valor){
                return true;
            }

        break;

        case MENOR:
            
            if(obj<valor){
                return true;
            }

        break;

        case MENOROIGUAL:
            
            if(obj<=valor){
                return true;
            }

        break;

           
    }

    return false;
}

void NodoDecision::setHijo(NodoDecision* nodo){

    NodosHijos.push_back(nodo);

}

vector<NodoDecision*> NodoDecision::getHijos(){

    return NodosHijos;

}  


bool NodoDecision::getDecision(){
    
    return Decision;

}


int NodoDecision::getAccion(){
    
    return tipoaccion;

}

bool NodoDecision::getAccionB() {
	return Accion;
}