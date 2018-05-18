#ifndef ARBOLDECISION_H
#define ARBOLDECISION_H

#include <iostream>
#include "NodoDecision.hpp"
#include <vector>
#include <string>






class ArbolDecision
{
  public:
	
  ArbolDecision();
  ~ArbolDecision();

  void nuevoNodoDecision(int nodopadre,int idnodo,tipo_nodo tipodecision,tipo_parametro parametro,int valor,bool booleano);
  void nuevoNodoAccion(int nodopadre,int idnodo,int tipoaccion);
  void nuevoNodoSalto(int nodopadre,int idnodo,int idNodoSalto);
  void deleteNodosArbol();  
  int recorrerArbol(bool arrayboleanos[] , int obj,int jugador);  

  private:

  NodoDecision *NodoRuta;
  std::vector<NodoDecision*> arrayNodos;

};

#endif /* ARBOLDECISION_H */