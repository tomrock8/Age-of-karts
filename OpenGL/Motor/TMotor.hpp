#pragma once
#include "libGL.hpp"
#include "TMalla.hpp"
#include "TGestorRecursos.hpp"
#include "TNodo.hpp"
#include <iostream>
#include <stack>

#define WIDTH 640
#define HEIGHT 480
class TMotor {

private:


public:
	static TMotor &instancia();
	TMotor();
	void resizeScreen(int width, int height);
	//----------------------------------------//
	//------------CREACION DEL NODOS----------//
	//----------------------------------------//
	TMalla *createMesh(const char *fich);
	GLFWwindow *getVentana();
protected:
	TNodo * escena;
	TGestorRecursos *gestorRecursos;
	GLFWwindow *ventana;
	int screenHEIGHT;
	int screenWIDTH;
};