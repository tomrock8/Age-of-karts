#include "TMotor.hpp"

TMotor &TMotor::instancia() {
	static TMotor _instancia;		//la primera vez llamara a el constructor de motor
	return _instancia;
}

 TMotor::TMotor() {
	//Inicializar  libreria GLFW
	if (!glfwInit()) {
		std::cout << "Error inicializando la libreria GLFW" << std::endl;
	}
	//---------------------------------------------------------------//
	//----CREACION OBJETO VENTANA PARA USAR LS FUNCIONES DE GLFW-----//
	//---------------------------------------------------------------//
	ventana = glfwCreateWindow(WIDTH, HEIGHT, "Age of Karts - SocketWar 2017-2018", NULL, NULL);

	
	glfwGetFramebufferSize(ventana, &screenWIDTH, &screenHEIGHT);	//almacena las dimensiones de la ventana en las variables screen
	std::cout << "Width: " << screenWIDTH << std::endl;
	std::cout << "Height: " << screenHEIGHT << std::endl;

	if (!ventana) {
		glfwTerminate();
		std::cout << "Error creando la ventana" << std::endl;
		
	}
	//------------------------------------------------------------------------//
	glfwMakeContextCurrent(ventana);					//hacer ventana = actual
	glViewport(0.0f, 0.0f, screenWIDTH, screenHEIGHT);	//especifica la parte de la pantalla donde openGL va a dibujar y convierte tams normalizados a px
	glMatrixMode(GL_PROJECTION);						//define las propiedades de la camara que mostrara los objetos en la escena
	glLoadIdentity();									//Reemplaza la matriz actual con la matriz de identidad, que suele ser (0,0,0)
	glOrtho(0, WIDTH, 0, HEIGHT, 0, 1);					//pone las coordenadas del sistema
	glMatrixMode(GL_MODELVIEW);							//por defecto siempre es modewlview. rotar escalar mover
	glLoadIdentity();
	string  f= "caja.obj";
	const char *g = f.c_str();

	gestorRecursos = new TGestorRecursos();
	TMalla *caja = createMesh(g);
}
 GLFWwindow *TMotor::getVentana() {
	 return ventana;
 }

void TMotor::resizeScreen(int w, int h) {
	screenWIDTH = w;
	screenHEIGHT = h;
	glViewport(0.0f, 0.0f, screenWIDTH, screenHEIGHT);	//especifica el tam de la ventana de opengl
}

TMalla *TMotor::createMesh(const char *fich) {
		TRecursoMalla *aux = gestorRecursos->getRecurso(fich);
		return new TMalla(aux);
}