
#ifndef GESTORJUGADORES_H
#define GESTORJUGADORES_H

#include <iostream>
#include <string>
//#include "IrrlichtLib.hpp"
//#include "Corredor.hpp"
#include "CorredorRed.hpp"
#include "CorredorJugador.hpp"

using namespace std;

class GestorJugadores
{
    public:

        ~GestorJugadores(void);

        static GestorJugadores *getInstancia();
        Corredor **getJugadores();
        int getNumJugadores();       

        void setJugadores(Corredor **pj1);

        void aumentarJugadores();


    private:
        GestorJugadores(void);
        static GestorJugadores *instancia;
        Corredor **pj;
        int numJugadores;


};

#endif