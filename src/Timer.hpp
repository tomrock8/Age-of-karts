#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <ctime>
#include <cstdlib>

class Timer
{
public:

    void setTimer(int valor);
    void restartTimer();
    int getTimer();
    static Timer *getInstancia();
    ~Timer();
private:
    Timer(void);
	std::clock_t tiempo_inicial;
    int tiempo_transcurrido;
    static Timer *instancia;

};

#endif /* Tiempo */