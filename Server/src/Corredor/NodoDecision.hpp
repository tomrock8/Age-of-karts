#ifndef NODODECISION_H
#define NODODECISION_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


typedef enum {NADA2,IGUAL,DISTINTO,MAYOR,MAYOROIGUAL,MENOR,MENOROIGUAL} tipo_nodo;

typedef enum {NADA3,OBJNUMERO,OBJBOOLEANO,DISTANCIACERCA,DISTANCIAMEDIA,DISTANCIALEJOS,
              VELOCIDADBAJA,VELOCIDADMEDIA,VELOCIDADALTA,
              NOGIRO,GIROMEDIODERECHA,GIROMEDIOIZQUIERDA,GIROFUERTEDERECHA,GIROFUERTEIZQUIERDA,
              CAJA,TURBOO,ENEMIGO,VISION,ENEMIGOLADO,HABILIDADD,JUGADOR} tipo_parametro;

class NodoDecision
{
  public:
	
    NodoDecision();
    ~NodoDecision();
   
  void decision(std::vector<NodoDecision*> &arrayNodos,int nodopadre,int idnodo,tipo_nodo tipoNodo,tipo_parametro parametro,int valor,bool valorb);
  void accion(std::vector<NodoDecision*> &arrayNodos,int nodopadre,int idnodo,int accionIA);
  void salto(std::vector<NodoDecision*> &arrayNodos,int nodopadre,int idnodo,int idNodoSalto);
  void setHijo(NodoDecision * nodo);
  std::vector<NodoDecision*> getHijos();
  bool getDecision();
  int getAccion();
  bool getConsulta(bool arrayboleanos[], int obj,int jugador);   
  bool getAccionB();
  int getid();
  int getidNodoSalto();
  bool getSalto();

  
  private:
    
    int idNodo;
    NodoDecision *NodoPadre; 
    std::vector<NodoDecision*> NodosHijos;
    
    tipo_nodo condiciones;
    int tipoaccion;
    tipo_parametro parametro;

    int  valor; // VALOR NUMERICO PARA COMPROBACIONES
    bool valorBoleano;  // VALOR BOOLEANO PARA COMPROBACIONES
    bool Decision;
    bool Accion;
    bool Salto;
    int idNodoSalto;
};

#endif /* NODODECISION_H */