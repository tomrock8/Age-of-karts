#include "NodoDecision.hpp"



NodoDecision::NodoDecision(){

Decision=false;
Accion=false;
Salto = false;
idNodoSalto=0;
valor=0;
valorBoleano=false;
idNodo=0;
NodoPadre=nullptr;
parametro = NADA3;  
condiciones = NADA2;
tipoaccion=0;  

}

NodoDecision::~NodoDecision(){

}

void NodoDecision::decision(vector<NodoDecision*> &arrayNodos,int nodopadre,int idnodo,tipo_nodo tipodecision,tipo_parametro parametro,int valor,bool valorb){

    Decision=true;
    if(arrayNodos.size()==0){
    NodoPadre=nullptr;
    }else{
        for(int i =0; i< arrayNodos.size();i++){
            if(arrayNodos.at(i)->getid() == nodopadre){ 
            NodoPadre = arrayNodos.at(i);
            NodoPadre->setHijo(this);
            i=arrayNodos.size();
            }
        }
    
    }

    idNodo = idnodo;
    condiciones=tipodecision;
    this->parametro = parametro;
    this->valor=valor;
    valorBoleano = valorb;
    arrayNodos.push_back(this);

}

void NodoDecision::accion(vector<NodoDecision*> &arrayNodos,int nodopadre,int idnodo,int accionIA){

    Accion=true;
    if(arrayNodos.size()==0){
    NodoPadre=nullptr;
    }else{
            for(int i =0; i< arrayNodos.size();i++){
            if(arrayNodos.at(i)->getid() == nodopadre){ 
            NodoPadre = arrayNodos.at(i);
            NodoPadre->setHijo(this);
            i=arrayNodos.size();
            }
        }
    }
    
    idNodo = idnodo;
    tipoaccion=accionIA;
    arrayNodos.push_back(this);

}

void NodoDecision::salto(vector<NodoDecision*> &arrayNodos,int nodopadre,int idnodo,int idNodoSalto){

    Salto=true;
    if(arrayNodos.size()==0){
    NodoPadre=nullptr;
    }else{
            for(int i =0; i< arrayNodos.size();i++){
            if(arrayNodos.at(i)->getid() == nodopadre){ 
            NodoPadre = arrayNodos.at(i);
            NodoPadre->setHijo(this);
            i=arrayNodos.size();
            }
        }
    }
    
    idNodo = idnodo;
    this->idNodoSalto=idNodoSalto;
    arrayNodos.push_back(this);
}

bool NodoDecision::getConsulta(bool arrayboleanos[], int obj,int jugador){
    
    bool parametroaux = false;              
    bool resultado = false;
    int numero = 0;

    switch(parametro){

        case NADA3:
        break;

        case OBJNUMERO:
        numero=obj;
        break;

        case JUGADOR:
        numero=jugador;
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

        case ENEMIGOLADO:
        parametroaux=arrayboleanos[16];
        break;

        case HABILIDADD:
        parametroaux=arrayboleanos[17];
        break;

    }

    
    switch(condiciones){

        case NADA2:
        break;

        case IGUAL:
            
            if(parametro != OBJNUMERO && parametro != JUGADOR){
                
                if(parametroaux==valorBoleano)
                    resultado = true;
                     
            }else{

                if(numero == valor)
                    resultado = true;
                
            }

        break;

        case DISTINTO:

            if(parametro != OBJNUMERO){

                if(parametroaux!=valorBoleano)
                    resultado = true;
             

            }else{

                if(obj != valor)
                    resultado = true;
                   
            }

        break;

        case MAYOR:

            if(obj>valor)
                resultado = true;
           
        break;

        case MAYOROIGUAL:
            
            if(obj>=valor)
                resultado = true;

        break;

        case MENOR:
            
            if(obj<valor)
               resultado = true;
           

        break;

        case MENOROIGUAL:
            
            if(obj<=valor)
                resultado = true;
           

        break;
    }
   
    return resultado; 
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

int NodoDecision::getid(){
    return idNodo;
}

int NodoDecision::getidNodoSalto(){

    return idNodoSalto;
}

bool NodoDecision::getSalto() {
	return Salto;
}