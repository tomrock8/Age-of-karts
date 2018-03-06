#include "ArbolDecision.hpp"


ArbolDecision::ArbolDecision(){

    
    NodoRuta = nullptr;

}



 void ArbolDecision::nuevoNodoDecision(int nodopadre,tipo_nodo tipodecision,tipo_parametro parametro,int valor,bool booleano){

    if(!NodoRuta){
    NodoDecision *nodo = new NodoDecision();
    nodo->decision(arrayNodos,nodopadre,tipodecision,parametro,valor,booleano);
    NodoRuta = nodo;
    }else{
    NodoDecision *nodo = new NodoDecision();
    nodo->decision(arrayNodos,nodopadre,tipodecision,parametro,valor,booleano);   
    }

 }

void ArbolDecision::nuevoNodoAccion(int nodopadre,int tipoaccion){

    if(!NodoRuta){
    NodoDecision *nodo = new NodoDecision();
    nodo->accion(arrayNodos,nodopadre,tipoaccion);
    NodoRuta = nodo;
    }else{
    NodoDecision *nodo = new NodoDecision();
    nodo->accion(arrayNodos,nodopadre,tipoaccion);   
    }

 }

int ArbolDecision::recorrerArbol(bool arrayboleanos[], int obj){

  

    if(NodoRuta->getDecision()){
    bool check = false; //boleano de control para salir del bucle
    int cont = 0;   // variable para incrementar el array de hijos 
   
    while(!check && cont!=NodoRuta->getHijos().size()){
        //cout<<"HOLA"<<endl;
		if (!NodoRuta->getHijos().at(cont)->getAccionB()){
			check = NodoRuta->getHijos().at(cont)->getConsulta(arrayboleanos, obj);
            
        }else{
            check = true;
        }

    if(!check){    
    cont++;
        }
    
    }    

    if (cont==NodoRuta->getHijos().size()){
         NodoRuta=arrayNodos.at(0);
         return 1; // por si acaso no actualiza del todo bien le decimos que acelere 
    }

    NodoRuta = NodoRuta->getHijos().at(cont);
    //llamamos al arbol recursivamente para que vaya navegando entre sus nodos 
    recorrerArbol(arrayboleanos,obj);   

    }else{

    int auxiliar = NodoRuta->getAccion();
    NodoRuta=arrayNodos.at(0);
    return auxiliar;    
    //en el caso de que sea un nodo de accion devolver la accion tal cual
    }


}


