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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// CREACION OBJETO VENTANA PARA USAR LAS FUNCIONES DE GLFW
	ventana = glfwCreateWindow(WIDTH, HEIGHT, "Age of Karts - SocketWar 2017-2018", NULL, NULL);

	glfwGetFramebufferSize(ventana, &screenWIDTH, &screenHEIGHT);	//almacena las dimensiones de la ventana en las variables screen
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!ventana) {
		glfwTerminate();
		std::cout << "(TMotor::TMotor()) Error creando la ventana" << std::endl;
	}

	glfwMakeContextCurrent(ventana);					//hacer ventana = actual
	glViewport(0.0f, 0.0f, screenWIDTH, screenHEIGHT);	//especifica la parte de la pantalla donde openGL va a dibujar y convierte tams normalizados a px

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "(TMotor::TMotor()) ERROR EN GLEW\n";
		return;
	}

	contID = 0; //inicializacion de los id's de obj3D
	gestorRecursos = new TGestorRecursos(); // Inicializacion del gestor de recursos
	scene = new TNodo("escena_raiz"); // Creacion del nodo raiz (Escena) 


	//----------Declaracion IMGUI---------

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	ImGui_ImplGlfwGL3_Init(ventana, true);

	// Setup style
	ImGui::StyleColorsDark();

	shader = new Shader("assets/shaders/shaderLightingMap/vertexShader.txt", "assets/shaders/shaderLightingMap/fragmentShader.txt", nullptr);
	shaderHUD = new Shader("assets/shaders/shaderHUD/vertexShader.txt", "assets/shaders/shaderHUD/fragmentShader.txt", nullptr);
	shaderDebug = new Shader("assets/shaders/shaderDebug/vertexShader.txt", "assets/shaders/shaderDebug/fragmentShader.txt", nullptr);
	shaderDirectionalDepth = new Shader("assets/shaders/shaderDepth/shaderLuzDirigida/vertexShader.txt", "assets/shaders/shaderDepth/shaderLuzDirigida/fragmentShader.txt", nullptr);
	shaderPointDepth = new Shader("assets/shaders/shaderDepth/shaderLuzPuntual/vertexShader.txt", "assets/shaders/shaderDepth/shaderLuzPuntual/fragmentShader.txt"
		, "assets/shaders/shaderDepth/shaderLuzPuntual/geometryShader.txt");
	std::cout << "Version OPENGL: " << glGetString(GL_VERSION) << endl;
}

void TMotor::resizeScreen(int w, int h) {
	screenWIDTH = w;
	screenHEIGHT = h;
	GLint a = 0;
	glViewport(a, a, screenWIDTH, screenHEIGHT);	//especifica el tam de la ventana de opengl
}

void TMotor::close() {
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();									//Terminar GLFW, limpiando todos los recursos alocados por GLFW
	glfwDestroyWindow(TMotor::instancia().getVentana());
	delete gestorRecursos;
	for (int i = 0; i < HUDs.size(); i++) {
		delete HUDs[i];
	}
}

// METODOS GET
GLfloat TMotor::getWidth() { return screenWIDTH; }
GLfloat TMotor::getHeight() { return screenHEIGHT; }
GLFWwindow *TMotor::getVentana() { return ventana; }
TNodo *TMotor::getNode(const char* nombre) { return scene->getNode(nombre); }
TNodo *TMotor::getSceneNode() { return scene; }
TNodo *TMotor::getActiveCamera() { return activeCamera; }
std::vector <TNodo*> TMotor::getActiveLights() { return activeLights; }
Shader *TMotor::getShader() { return shader; }
Shader *TMotor::getShaderHUD() { return shaderHUD; }
Shader *TMotor::getShaderDebug() { return shaderDebug; }
Shader *TMotor::getShaderDirectionalDepth() { return shaderDirectionalDepth; }
Shader *TMotor::getShaderPointDepth() { return shaderPointDepth; }

// METODOS SET
void TMotor::setActiveCamera(TNodo *cam) { activeCamera = cam; }
void TMotor::setActiveLight(TNodo *light) { activeLights.push_back(light); }


// ----------------------------------
//  CREACION DE NODOS PUBLICOS
// ----------------------------------
obj3D *TMotor::newCameraNode(const char *name, const char* parentNode) {
	// R O T A C I O N
	string *nameRot = new string("rotacion_" + (string)name);
	TTransform *rotacion = createTransformation();
	if (strcmp(parentNode, "escena_raiz") != 0)	rotacion->setMatriz(static_cast<TTransform*>(getNode(parentNode)->getPadre()->getPadre()->getEntidad())->getMatriz());
	TNodo *rotacionNodo = createTransformationNode(getNode(parentNode), rotacion, nameRot->c_str());

	// T R A S L A C I O N

	string *nameTras = new string("traslacion_" + (string)name);
	TTransform *traslacion = createTransformation();
	if (strcmp(parentNode, "escena_raiz") != 0)	traslacion->setMatriz(static_cast<TTransform*>(getNode(parentNode)->getPadre()->getEntidad())->getMatriz());
	//traslacion->trasladar(traslation.x, traslation.y, traslation.z);
	TNodo *traslacionNodo = createTransformationNode(rotacionNodo, traslacion, nameTras->c_str());


	TCamara *camara = createCam();
	camara->setPerspective(screenHEIGHT, screenWIDTH, 0.01f, 10000.0f, 70.0f);//OJO CON EL VALOR 1000 ES CLAVE PARA MOSTRAR PARTE DEL MAPA O TODO
	TNodo *nodo = createCamNode(traslacionNodo, camara, name);

	contID++;
	return new obj3D(nodo, name, contID);
}


obj3D *TMotor::newLightNode(const char *name, glm::vec4 dir, float att, float corte, bool shadow, bool active, const char* parentNode) {
	// R O T A C I O N
	string *nameRot = new string("rotacion_" + (string)name);
	TTransform *rotacion = createTransformation();
	//rotateLight->rotar(0, 0, 1, 45);

	TNodo *rotacionNodo = createTransformationNode(getNode(parentNode), rotacion, nameRot->c_str());

	// T R A S L A C I O N
	string *nameTras = new string("traslacion_" + (string)name);
	TTransform *traslacion = createTransformation();
	//traslacion->trasladar(traslation.x, traslation.y, traslation.z);
	TNodo *traslacionNodo = createTransformationNode(rotacionNodo, traslacion, nameTras->c_str());

	// N O D O
	//TLuz  *luz = new TLuz(glm::vec3(.2f), glm::vec3(.5f), glm::vec3(.8f), dir, att, corte);
	TLuz  *luz = new TLuz(glm::vec3(.2f), glm::vec3(.5f), glm::vec3(0.8f), dir, att, corte, shadow, active);
	TNodo *nodo = createLightNode(traslacionNodo, luz, name);
	if (static_cast<TLuz *>(nodo->getEntidad())->getLightType() < 0.1) {
		nPointLights++;
	}
	else {
		nSpotLights++;
	}
	contID++;
	return new obj3D(nodo, name, contID);
}

obj3D *TMotor::newMeshNode(const char *name, const char *path, const char* parentNode) {
	// E S C A L A D O
	string *nameEsc = new string("escalado_" + (string)name);
	TTransform *scaleMesh = createTransformation();
	TNodo *scaleNodeMesh = createTransformationNode(getNode(parentNode), scaleMesh, nameEsc->c_str());

	// R O T A C I O N
	string *nameRot = new string("rotacion_" + (string)name);
	TTransform *rotateMesh = createTransformation();
	if (strcmp(parentNode, "escena_raiz") != 0)	rotateMesh->setMatriz(static_cast<TTransform*>(getNode(parentNode)->getPadre()->getPadre()->getEntidad())->getMatriz());
	TNodo *rotationNodeMesh = createTransformationNode(scaleNodeMesh, rotateMesh, nameRot->c_str());

	// T R A S L A C I O N
	string *nameTras = new string("traslacion_" + (string)name);
	TTransform *traslationMesh = createTransformation();
	if (strcmp(parentNode, "escena_raiz") != 0)	traslationMesh->setMatriz(static_cast<TTransform*>(getNode(parentNode)->getPadre()->getEntidad())->getMatriz());
	TNodo *traslationNodeMesh = createTransformationNode(rotationNodeMesh, traslationMesh, nameTras->c_str());


	// N O D O
	/*cout << "\nContenido de mallas en TMotor\n";
	for (int i = 0; i < gestorRecursos->getRecursoMallas().size(); i++) {
		cout << "MALLA MOTOR: " << gestorRecursos->getRecursoMallas().at(i)->getNombre() << "\n";
	}*/


	TMalla *malla = TMotor::instancia().createMesh(path);
	TNodo  *nodo = TMotor::instancia().createMeshNode(traslationNodeMesh, malla, name);
	contID++;
	return  new obj3D(nodo, name, contID);
}


// -------------------------------
// M A L L A S
// -------------------------------
TMalla *TMotor::createMesh(const char *fich) {
	//	TRecursoMalla *auxMesh = gestorRecursos->getRecurso(fich);
	//	TRecursoTextura *auxText = gestorRecursos->getRecursoTextura(fich);
	//	return new TMalla(auxMesh, auxText);
	return new TMalla(gestorRecursos->loadMesh(fich));
	//return NULL;
}


void TMotor::precarga(const char* modelo) {
	gestorRecursos->loadMesh(modelo);
}

TNodo  *TMotor::createMeshNode(TNodo *padre, TMalla *mesh, const char* name) {
	TNodo *nodo = new TNodo(name);
	nodo->setPadre(padre);
	nodo->setEntidad(mesh);
	padre->addHijo(nodo);
	return nodo;
}

//---------------------------------------------
// T R A N S F O R M A C I O N E S
//---------------------------------------------
TTransform *TMotor::createTransformation() { return new TTransform(); }

TNodo *TMotor::createTransformationNode(TNodo *padre, TTransform *transf, const char* name) {
	TNodo *nodo = new TNodo(name);
	nodo->setPadre(padre);
	nodo->setEntidad(transf);
	padre->addHijo(nodo);
	return nodo;
}

//-----------------------
// C A M A R A 
//-----------------------
TCamara *TMotor::createCam() { return new TCamara(); }

TNodo *TMotor::createCamNode(TNodo *padre, TCamara *cam, const char* name) {//crear y registrar camara en la aplicacion
	TNodo *nodo = new TNodo(name);
	nodo->setPadre(padre);		//Se asigna el nodo padre
	nodo->setEntidad(cam);
	//nodo->setID(id);
	padre->addHijo(nodo);
	cameras.push_back(nodo);		//se guarda en el vector de camaras con su identificador;
	//cuando se crea se activa automaticamente, desactivando las otras posibles camaras
	setActiveCamera(nodo);
	return nodo;
}

//--------------------
// L U C E S
//--------------------
TLuz *TMotor::createLight() { return new TLuz(); }

TNodo *TMotor::createLightNode(TNodo *padre, TLuz *luz, const char* name) {
	TNodo *nodo = new TNodo(name);
	nodo->setPadre(padre);		//Se asigna el nodo padre
	nodo->setEntidad(luz);
	//nodo->setID(id);
	padre->addHijo(nodo);
	lights.push_back(nodo);		//se guarda en el vector de luces con su identificador;
	//cuando se crea se activa automaticamente
	setActiveLight(nodo);
	return nodo;
}

//------------------------------
// D I B U J A D O
//------------------------------
void TMotor::clean() {
	glViewport(0, 0, screenWIDTH, screenHEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				//Limpia el buffer del color
}

void TMotor::draw(int tipo) {
	//Establecer el nuevo color de fondo
	glClearColor(0.16f, 0.533f, 0.698f, 0.0f);

	//Llamamos al render de las luces para calcular el depth map que se usara para calcular las sombras
	if (lights.size() > 0) {
		for (int i = 0; i < lights.size(); i++) {
			static_cast<TLuz *>(lights[i]->getEntidad())->renderMap();
			//Activamos el face culling
			glEnable(GL_CULL_FACE);
			//Para evitar el efecto del peter panning. Debido a la utilizacion de un bias (rango), se pueden producir fallos 
			//en las sombras al quedar en algunos casos separadas de los objetos que las producen
			glCullFace(GL_FRONT);
			//Segun el tipo de luz (puntual o dirigida), usamos un shader diferente para calcular el mapa de profundidad
			if (static_cast<TLuz *>(lights[i]->getEntidad())->getLightType() < 0.1) {
				scene->draw(shaderPointDepth);
			}
			else {
				scene->draw(shaderDirectionalDepth);
			}
			//Desactivamos el face culling
			glDisable(GL_CULL_FACE);
			//Desactivamos el framebuffer usado para el mapa de profundidad
			static_cast<TLuz *>(lights[i]->getEntidad())->unbindDepthBuffer();
		}
	}

	//Se llama a la funcion para limpiar los buffers de OpenGL
	clean();
	if (tipo == 1 || tipo == 2) {
		//Se activa el shader para el renderizado 3D
		shader->use();

		//Mandamos el numero de luces de cada tipo al shader
		shader->setInt("numPointLights", nPointLights);
		shader->setInt("numSpotLights", nSpotLights);
		//Se llama al dibujado de los distintos nodos del arbol
		drawCamera();
		drawLight();
		if (lights.size() > 0) {
			for (int i = 0; i < lights.size(); i++) {
				static_cast<TLuz *>(lights[i]->getEntidad())->configureShadow();
			}
		}
		scene->draw(shader);
	}

	/*
	unsigned int VBO,VAO;
		GLfloat points[12];

		points[0] = 100.5f;
		points[1] = 100.5f;
		points[2] = 100.0f;
		points[3] = 0.8f;
		points[4] = 0.5f;
		points[5] = 0.3f;

		points[6] = 200.0f;
		points[7] = 200.0f;
		points[8] = 200.0f;
		points[9] = 0.8f;
		points[10] = 0.5f;
		points[11] = 0.3f;


		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		TMotor::instancia().getShaderDebug()->use();

		glm::mat4 proje = glm::ortho(
		glm::radians(70.0f), 			// El campo de vision vertical:zoom 90 (extra ancho) y 30 (zoom aumentado)
		800.0f / 600.0f,					// Proporcion.ventana 4/3 == 800/600
		0.01f,					// Plano de corte cercano. Tan grande como sea posible o tendras problemas de precision.
		1000.0f						// Plano de corte lejano. Tan enano como se pueda.
	);
		glm::mat4 view = TMotor::instancia().getActiveCamera()->getEntidad()->getViewMatrix();

		TMotor::instancia().getShaderDebug()->setMat4("projection", proje);
		TMotor::instancia().getShaderDebug()->setMat4("view", view);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	*/

	//if(tipo != 1 && tipo != 2){
	//Se activa el shader para el dibujado del HUD
	shaderHUD->use();

	//Dibujamos el hud activo
	activeHud->drawHud(shaderHUD);//}


	/*shaderDebug->use();

	if (lights.size()> 0){
		static_cast<TLuz *>(lights[0]->getEntidad())->configureShadow();
	}

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);*/

	
	int display_w, display_h;
	glfwGetFramebufferSize(TMotor::instancia().getVentana(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	
	if (ImGui::GetFrameCount() > 0) {
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	}

	//swap los bufers de pantalla (trasero y delantero)
	glfwSwapBuffers(TMotor::instancia().getVentana());
}


void TMotor::drawCamera() {
	//recorrer el nodo a la inversa hasta la raiz y guardar las matrices que se obtienen de sus entidades
	//para luego multiplicarlas en orden inverso
	std::vector<glm::mat4> matrixAux;
	glm::vec4 defaultVector(0, 0, 0, 1);
	TNodo *aux = activeCamera->getPadre();
	int cont = 0;
	while (aux != scene) {
		//recorrer y guardar en un vector de matrices las transformaciones
		//if (cont < 3)
		matrixAux.push_back(static_cast<TTransform *>(aux->getEntidad())->getMatriz());
		cont++;
		aux = aux->getPadre();
	}
	//cout << cont << endl;
	glm::mat4 viewMatrix;
	//recorrido a la inversa
	for (int i = matrixAux.size() - 1; i >= 0; i--) {
		/*cout << matrixAux.at(i)[0][0] << " - " << matrixAux.at(i)[0][1] << " - " <<  matrixAux.at(i)[0][2] << " - " <<  matrixAux.at(i)[0][3] << endl;
		cout << matrixAux.at(i)[1][0] << " - " << matrixAux.at(i)[1][1] << " - " <<  matrixAux.at(i)[1][2] << " - " <<  matrixAux.at(i)[1][3] << endl;
		cout << matrixAux.at(i)[2][0] << " - " << matrixAux.at(i)[2][1] << " - " <<  matrixAux.at(i)[2][2] << " - " <<  matrixAux.at(i)[2][3] << endl;
		cout << matrixAux.at(i)[3][0] << " - " << matrixAux.at(i)[3][1] << " - " <<  matrixAux.at(i)[3][2] << " - " <<  matrixAux.at(i)[3][3] << endl;
		cout << "*********************" << endl;
		cout << viewMatrix[0][0] << " - " << viewMatrix[0][1] << " - " <<  viewMatrix[0][2] << " - " <<  viewMatrix[0][3] << endl;
		cout << viewMatrix[1][0] << " - " << viewMatrix[1][1] << " - " <<  viewMatrix[1][2] << " - " <<  viewMatrix[1][3] << endl;
		cout << viewMatrix[2][0] << " - " << viewMatrix[2][1] << " - " <<  viewMatrix[2][2] << " - " <<  viewMatrix[2][3] << endl;
		cout << viewMatrix[3][0] << " - " << viewMatrix[3][1] << " - " <<  viewMatrix[3][2] << " - " <<  viewMatrix[3][3] << endl;
		cout << "=====================" << endl;*/
		viewMatrix = matrixAux.at(i) * viewMatrix;
	}

	//calcular posicion de la camara y pasarsela al fragment shader
	glm::vec4 posC = viewMatrix * defaultVector;
	shader->setVec3("posCamera", glm::vec3(posC[0], posC[1], posC[2]));
	//cout << "POSICION DE LA CAMARA: " << posC[0] << " - " << posC[1] << " - " << posC[2]<< endl;
	//cout << "------------------" << endl;

	//por ultimo pasar al shader la view y la projection matrix
	shader->setMat4("view", glm::inverse(viewMatrix));
	shader->setMat4("projection", activeCamera->getEntidad()->getProjectionMatrix());
}

void TMotor::drawLight() {
	//recorrer el nodo a la inversa hasta la raiz y guardar las matrices que se obtienen de sus entidades
	//para luego multiplicarlas en orden inverso
	glm::vec4 defaultVector(0, 0, 0, 1);

	for (int i = 0; i < activeLights.size(); i++) {
		std::vector<glm::mat4> matrixAux;
		glm::mat4 lightMatrix;
		TNodo *aux = activeLights.at(i)->getPadre();

		while (aux != scene) {
			//recorrer y guardar en un vector de matrices las transformaciones
			matrixAux.push_back(static_cast<TTransform *>(aux->getEntidad())->getMatriz());
			aux = aux->getPadre();
		}

		//recorrido a la inversa
		for (int i = matrixAux.size() - 1; i >= 0; i--) {
			lightMatrix = lightMatrix * matrixAux.at(i);
		}

		glm::vec4 lPos = lightMatrix * defaultVector;
		glm::vec3 pos(lPos.x, lPos.y, lPos.z);
		static_cast<TLuz *> (activeLights.at(i)->getEntidad())->setLightPosition(pos);
		static_cast<TLuz *> (activeLights.at(i)->getEntidad())->setLuzPuntual(shader, activeLights.at(i)->getName());
	}
	//por ultimo cargar la posicion que ocupa la luz en su matriz de luz
}

//----- HUD -----

//Funcion para crear un nuevo hud dentro del motor
void TMotor::newHud(const char* n) {
	//creamos el nuevo hud con su nombre
	hud* h = new hud(n);
	//Lo añadimos al array de hud
	HUDs.push_back(h);
	//Lo activamos por defecto
	setActiveHud(n);
}

//Funcion para activar un hud, que sera el que se utilice, solo puede haber un hud activo por iteracion
void TMotor::setActiveHud(const char* n) {
	//Obtenemos el hud a partir del nombre
	hud* h = getHud(n);
	//Lo activamos
	activeHud = h;
}

//Funcion que devuelve un hud a partir del nombre
hud* TMotor::getHud(const char* n) {
	hud* h = NULL;
	//Recorremos el array de HUDs
	for (int i = 0; i < HUDs.size(); i++) {
		//Comparamos los nombres y nos guardamos el correcto
		if (strcmp(HUDs.at(i)->getName(), n) == 0) {
			h = HUDs.at(i);
		}
	}
	return h;
}

//Funcion para devolver el hud activo
hud* TMotor::getActiveHud() {
	return activeHud;
}
obj3D* TMotor::getObjActiveCamera() {
	if (cameras.size() > 0) {
		return new obj3D(activeCamera, activeCamera->getName(), 99);

	}
	else return NULL;

}
void TMotor::toEulerAngle(float x, float y, float z, float w, float& roll, float& pitch, float& yaw)
{

	// roll (x-axis rotation)
	double sinr = +2.0 * (w * x + y * z);
	double cosr = +1.0 - 2.0 * (x * x + y * y);
	roll = atan2(sinr, cosr);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (w * y - z * x);
	if (fabs(sinp) >= 1)
		pitch = copysign(PI / 2, sinp); // use 90 degrees if out of range
	else
		pitch = asin(sinp);

	pitch = -pitch;

	// yaw (z-axis rotation)
	double siny = +2.0 * (w * z + x * y);
	double cosy = +1.0 - 2.0 * (y * y + z * z);
	yaw = atan2(siny, cosy);

	/*
		glm::quat q(x,y,z,w);
		glm::vec3 euler = glm::eulerAngles(q) * 3.14159f / 180.f;
		roll = euler.x;
		pitch = euler.y;
		yaw = euler.z;*/
}