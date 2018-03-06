#ifndef ARBOLDECISION_H
#define ARBOLDECISION_H

#include <iostream>
#include "NodoDecision.hpp"
#include <vector>
#include <string>


using namespace std;



class ArbolDecision
{
  public:
	
  ArbolDecision();
  ~ArbolDecision();

  void nuevoNodoDecision(int nodopadre,tipo_nodo tipodecision,tipo_parametro parametro,int valor,bool booleano);
  void nuevoNodoAccion(int nodopadre,int tipoaccion);

  int recorrerArbol(bool arrayboleanos[] , int obj);  

  private:

  NodoDecision *NodoRuta;
  vector<NodoDecision*> arrayNodos;

};

#endif /* ARBOLDECISION_H */