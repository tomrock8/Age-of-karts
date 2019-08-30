#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <ctime>
#include <cstdlib>

class Timer {
public:
	~Timer();

	void restartTimer();

	// METODOS SET
	void setTimer(int valor);

	// METODOS GET
	int getTimer();
	static Timer* getInstancia();

private:
	Timer();

	std::clock_t tiempo_inicial;
	int tiempo_transcurrido;
	static Timer* instancia;

};

#endif /* TIMER_H */