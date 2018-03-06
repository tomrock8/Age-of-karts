#include "TMotor.hpp"

TMotor &TMotor::instancia() {
	static TMotor _instancia;		//la primera vez llamara a el constructor de motor
	return _instancia;
}

TMotor::TMotor() {
	//Inicializar  libreria GLFW
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "(TMotor::TMotor()) Error inicializando la libreria GLFW" << std::endl;
	}
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Version maxima de opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Version minima de opengl


	//---------------------------------------------------------------//
	//----CREACION OBJETO VENTANA PARA USAR LS FUNCIONES DE GLFW-----//
	//---------------------------------------------------------------//
	ventana = glfwCreateWindow(WIDTH, HEIGHT, "Age of Karts - SocketWar 2017-2018", NULL, NULL);

	glfwGetFramebufferSize(ventana, &screenWIDTH, &screenHEIGHT);	//almacena las dimensiones de la ventana en las variables screen
	
	if (!ventana) {
		glfwTerminate();
		std::cout << "(TMotor::TMotor()) Error creando la ventana" << std::endl;

	}

	//------------------------------------------------------------------------//
	glfwMakeContextCurrent(ventana);					//hacer ventana = actual
	glViewport(0.0f, 0.0f, screenWIDTH, screenHEIGHT);	//especifica la parte de la pantalla donde openGL va a dibujar y convierte tams normalizados a px
	

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "(TMotor::TMotor()) ERROR EN GLEW\n";
		return;
	}


	gestorRecursos = new TGestorRecursos();

	//--------creacion del nodo escena por el cual colgara todo el arbol------//
	scene = new TNodo("escena_raiz");


}
GLFWwindow *TMotor::getVentana() {
	return ventana;
}

void TMotor::resizeScreen(int w, int h) {
	screenWIDTH = w;
	screenHEIGHT = h;
	GLint a = 0;
	glViewport(a, a, screenWIDTH, screenHEIGHT);	//especifica el tam de la ventana de opengl
}
GLfloat TMotor::getWidth() {
	return screenWIDTH;
}
GLfloat TMotor::getHeight() {
	return screenHEIGHT;
}
//------------------------------------------------------------//
//----------------------ESCENA--------------------------------//
//------------------------------------------------------------//
TNodo *TMotor::getSceneNode() {
	return scene;
}
//------------------------------------------------------------//
//----CREACION DE LOS NODOS QUE TIENE EL ARBOL DE ESCENA------//
//------------------------------------------------------------//
//-----------------------MALLAS-------------------------------//
//------------------------------------------------------------//
TMalla *TMotor::createMesh(const char *fich) {
	TRecursoMalla *auxMesh = gestorRecursos->getRecurso(fich);
	TRecursoTextura *auxText = gestorRecursos->getRecursoTextura(fich);
	return new TMalla(auxMesh, auxText);
}
TNodo  *TMotor::createMeshNode(TNodo *padre, TMalla *mesh, int id, const char* name) {
	TNodo *meshAux = new TNodo(name);
	meshAux->setPadre(padre);
	meshAux->setEntidad(mesh);
	padre->addHijo(meshAux);
	return meshAux;
}
//------------------------------------------------------------//
//------------------TRANSFORMACIONES--------------------------//
//------------------------------------------------------------//
TTransform *TMotor::createTransformation() {
	return new TTransform();
}
TNodo *TMotor::createTransformationNode(TNodo *padre, TTransform *transf, const char* name) {
	TNodo *transfAux = new TNodo(name);

	transfAux->setPadre(padre);
	transfAux->setEntidad(transf);
	padre->addHijo(transfAux);
	return transfAux;
}

//------------------------------------------------------------//
//----------------------CAMARA--------------------------------//
//------------------------------------------------------------//
TCamara *TMotor::createCam() {
	return new TCamara();
}
TNodo *TMotor::createCamNode(TNodo *padre, TCamara *cam, int id, const char* name) {//crear y registrar camara en la aplicacion
	TNodo *camAux = new TNodo(name);
	camAux->setPadre(padre);		//Se asigna el nodo padre
	camAux->setEntidad(cam);
	camAux->setID(id);
	padre->addHijo(camAux);
	cameras.push_back(camAux);		//se guarda en el vector de camaras con su identificador;
	//cuando se crea se activa automaticamente, desactivando las otras posibles camaras
	setActiveCamera(camAux);
	return camAux;
}
void TMotor::setActiveCamera(TNodo *cam) {
	activeCamera = cam;
}
TNodo *TMotor::getActiveCamera() {
	return activeCamera;
}

//------------------------------------------------------------//
//--------------------------LUCES-----------------------------//
//------------------------------------------------------------//
TLuz *TMotor::createLight() {
	return new TLuz();
}
TNodo *TMotor::createLightNode(TNodo *padre, TLuz *luz, int id, const char* name) {
	TNodo *lightAux = new TNodo(name);
	lightAux->setPadre(padre);		//Se asigna el nodo padre
	lightAux->setEntidad(luz);
	lightAux->setID(id);
	padre->addHijo(lightAux);
	lights.push_back(lightAux);		//se guarda en el vector de camaras con su identificador;
	//cuando se crea se activa automaticamente, desactivando las otras posibles camaras
	setActiveLight(lightAux);
	return lightAux;
}
void TMotor::setActiveLight(TNodo *light) {
	activeLight = light;
}
TNodo *TMotor::getActiveLight() {
	return activeLight;
}
//---------------------------------------------------------------------//
//---------------------------DRAW--------------------------------------//
//---------------------------------------------------------------------//
void TMotor::drawCamera(TNodo *camera, Shader *shader) {
	//recorrer el nodo a la inversa hasta la raiz y guardar las matrices que se obtienen de sus entidades
	//para luego multiplicarlas en orden inverso
	std::vector<glm::mat4> matrixAux;
	TNodo *aux = camera->getPadre();

	while (aux != scene) {
		//recorrer y guardar en un vector de matrices las transformaciones
		matrixAux.push_back(static_cast<TTransform *>(aux->getEntidad())->getMatriz());
		aux = aux->getPadre();
	}
	glm::mat4 viewMatrix;
	//recorrido a la inversa
	for (int i = matrixAux.size() - 1; i >= 0; i--) {
		viewMatrix = viewMatrix * matrixAux.at(i);
	}
	
	//por ultimo cargar la matriz en la viewMatrix
	shader->use();
	shader->setMat4("view", glm::inverse(viewMatrix));
	shader->setMat4("projection", camera->getEntidad()->getProjectionMatrix());

}void TMotor::drawLight(TNodo *luz) {	//recorrer el nodo a la inversa hasta la raiz y guardar las matrices que se obtienen de sus entidades
	//para luego multiplicarlas en orden inverso
	std::vector<glm::mat4> matrixAux;
	glm::mat4 lightMatrix;
	glm::vec4 defaultVector(0, 0, 0, 1);
	TNodo *aux = luz->getPadre();

	while (aux != scene) {
		//recorrer y guardar en un vector de matrices las transformaciones
		matrixAux.push_back(static_cast<TTransform *>(aux->getEntidad())->getMatriz());
		aux = aux->getPadre();
	}

	//recorrido a la inversa
	for (int i = matrixAux.size() - 1; i >= 0; i--) {
		lightMatrix = lightMatrix * matrixAux.at(i);
	}
	//por ultimo cargar la posicion que ocupa la luz en su matriz de luz
	static_cast<TLuz*>(luz->getEntidad())->setLightMatrix(glm::vec4(defaultVector * lightMatrix));
}void TMotor::draw(Shader *shader) {
	drawCamera(getActiveCamera(), shader);
	drawLight(getActiveLight());
	scene->draw(shader);

}
