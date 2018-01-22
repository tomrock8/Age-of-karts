#include "Timer.hpp"

Timer *Timer::instancia = new Timer();
Timer::Timer()
{
    tiempo_inicial = time(0);
    tiempo_transcurrido = (time(0) - tiempo_inicial);
    
}
void Timer::setTimer(int valor){
    tiempo_transcurrido=valor;
}
int Timer::getTimer(){
     tiempo_transcurrido = (time(0) - tiempo_inicial);
    return tiempo_transcurrido;
}
void Timer::restartTimer(){
    tiempo_inicial = time(0);
}   
Timer *Timer::getInstancia()
{
	if (instancia == NULL)
		instancia = new Timer();

	return instancia;
}
Timer::~Timer(){
    delete instancia;
}