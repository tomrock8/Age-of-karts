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

	initDebugWindow();

	//Inicializamos el skybox que usara el motor
	skybox = new Skybox();

	//Creamos los distintos shaders que se van a usar para algun aspecto del motor
	shader = new Shader("assets/shaders/shaderLightingMap/vertexShader.txt", "assets/shaders/shaderLightingMap/fragmentShader.txt", nullptr);
	shaderHUD = new Shader("assets/shaders/shaderHUD/vertexShader.txt", "assets/shaders/shaderHUD/fragmentShader.txt", nullptr);
	shaderProjectedShadows = new Shader("assets/shaders/shaderProjectedShadows/vertexShader.txt", "assets/shaders/shaderProjectedShadows/fragmentShader.txt", nullptr);
	shaderDirectionalDepth = new Shader("assets/shaders/shaderDepth/shaderLuzDirigida/vertexShader.txt", "assets/shaders/shaderDepth/shaderLuzDirigida/fragmentShader.txt", nullptr);
	shaderPointDepth = new Shader("assets/shaders/shaderDepth/shaderLuzPuntual/vertexShader.txt", "assets/shaders/shaderDepth/shaderLuzPuntual/fragmentShader.txt"
		, "assets/shaders/shaderDepth/shaderLuzPuntual/geometryShader.txt");
	shaderSkybox = new Shader("assets/shaders/shaderSkybox/vertexShader.txt", "assets/shaders/shaderSkybox/fragmentShader.txt", nullptr);
	shaderDebug = new Shader("assets/shaders/shaderDebug/vertexShader.txt", "assets/shaders/shaderDebug/fragmentShader.txt", nullptr);
	std::cout << "Version OPENGL: " << glGetString(GL_VERSION) << endl;
}

void TMotor::initDebugWindow(){
	//----------Declaracion IMGUI---------
	renderDebug = true;
	
	if (ImGui::GetCurrentContext() == NULL) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
		ImGui_ImplGlfwGL3_Init(ventana, true);

		// Setup style
		ImGui::StyleColorsDark();
	}
}

void TMotor::closeDebugWindow(){
	if (ImGui::GetCurrentContext() != NULL) {
		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
	}
	setRenderDebug(false);
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
Shader *TMotor::getShaderProjectedShadows() { return shaderProjectedShadows; }
Shader *TMotor::getShaderDirectionalDepth() { return shaderDirectionalDepth; }
Shader *TMotor::getShaderPointDepth() { return shaderPointDepth; }
Shader *TMotor::getShaderSkybox() { return shaderSkybox; }
Shader *TMotor::getShaderDebug() { return shaderDebug; }
obj3D *TMotor::getObjeto(const char* name){
	obj3D *objeto = NULL;
	for (int i = 0; i < objetos.size(); i++){
		if (strcmp(objetos.at(i)->getName(), name) == 0){
			objeto = objetos.at(i);
		}
	}
	return objeto;
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

// METODOS SET
void TMotor::setActiveCamera(TNodo *cam) { activeCamera = cam; }
void TMotor::setActiveLight(TNodo *light) { activeLights.push_back(light); }
void TMotor::setRenderDebug(bool renderDebug) { this->renderDebug = renderDebug; }

//Funcion para activar un hud, que sera el que se utilice, solo puede haber un hud activo por iteracion
void TMotor::setActiveHud(const char* n) {
	hud* h = getHud(n); //Obtenemos el hud a partir del nombre
	activeHud = h; //Lo activamos
}

obj3D* TMotor::getObjActiveCamera() {
	if (cameras.size() > 0)
		return new obj3D(activeCamera, activeCamera->getName(), 99);

	return NULL;
}


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
	obj3D *o = new obj3D(nodo, name, contID);
	objetos.push_back(o);
	return o;
}

/*
cameraThird *TMotor::newCamera3ThPerson(const char *name, const char *parentName) {
	//La camara no tendra escalado, no es necesario
	//rotacion
	string *nameRot = new string("rotacion_" + (string)name);
	TTransform *rotacion = createTransformation();
	//de primeras lo colocamos en el mismo sitio que su padre
	TNodo *rotacionNodo = createTransformationNode(getNode("escena_raiz"), rotacion, nameRot->c_str());
	// T R A S L A C I O N
	string *nameTras = new string("traslacion_" + (string)name);
	TTransform *traslacion = createTransformation();
	TNodo *traslacionNodo = createTransformationNode(rotacionNodo, traslacion, nameTras->c_str());
	TCamara *camara = createCam();
	camara->setPerspective(screenHEIGHT, screenWIDTH, 0.01f, 10000.0f, 70.0f);//OJO CON EL VALOR 1000 ES CLAVE PARA MOSTRAR PARTE DEL MAPA O TODO
	TNodo *node = createCamNode(traslacionNodo, camara, name);

	contID++;
	return new cameraThird(node, getNode(parentName), name, contID++);
}
*/

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
	contID++;
	obj3D *o = new obj3D(nodo, name, contID);
	objetos.push_back(o);
	return o;
}

obj3D *TMotor::newMeshNode(const char *name, const char *path, const char* parentNode,bool sta) {
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

	TMalla *malla = TMotor::instancia().createMesh(path,sta);
	TRecursoMalla *nameMalla = static_cast<TRecursoMalla*>(gestorRecursos->getRecursoMallas().at(gestorRecursos->getRecursoMallas().size()-1));
	TNodo  *nodo = TMotor::instancia().createMeshNode(traslationNodeMesh, malla, nameMalla->getNombre());
	contID++;
	obj3D *o = new obj3D(nodo, name, contID);
	objetos.push_back(o);
	return o;
}


void TMotor::precarga(const char* modelo) {
	//gestorRecursos->loadMesh(modelo);
}

// -------------------------------
// M A L L A S
// -------------------------------
TMalla *TMotor::createMesh(const char *fich,bool sta) {
	//	TRecursoMalla *auxMesh = gestorRecursos->getRecurso(fich);
	//	TRecursoTextura *auxText = gestorRecursos->getRecursoTextura(fich);
	//	return new TMalla(auxMesh, auxText);
	return new TMalla(gestorRecursos->loadMesh(fich,sta));
	//return NULL;
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

	glClearColor(0.16f, 0.533f, 0.698f, 0.0f);//Establecer el nuevo color de fondo

	//Llamamos al render de las luces para calcular el depth map que se usara para calcular las sombras
	if (lights.size() > 0) {
		for (int i = 0; i < lights.size(); i++) {
			if (static_cast<TLuz *>(lights[i]->getEntidad())->getActive() == true){
				static_cast<TLuz *>(lights[i]->getEntidad())->renderMap();
				glEnable(GL_CULL_FACE); //Activamos el face culling
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

				glDisable(GL_CULL_FACE);//Desactivamos el face culling
				static_cast<TLuz *>(lights[i]->getEntidad())->unbindDepthBuffer();//Desactivamos el framebuffer usado para el mapa de profundidad
			}
		}
	}


	clean();//Se llama a la funcion para limpiar los buffers de OpenGL
	if (tipo == 1 || tipo == 2) {
		//DIBUJADO DEL SKYBOX
		//-------------------
		//Activamos el shader especifico del skybox
		shaderSkybox->use(); 
		//Establecemos las matrices view y projection a partir del dibujado de la camara
		drawCamera(shaderSkybox); 
		//Llamamos al metodo de dibujado del skybox
		skybox->drawSkyBox();

		//DIBUJADO DEL ARBOL
		//------------------
		//Se activa el shader para el renderizado 3D
		shader->use();
		//Establecemos las matrices view y projection a partir del dibujado de la camara
		drawCamera(shader);
		//Establecemos los datos de las distintas luces
		drawLight();
		//Configuramos los shaders para producir las sombras de cada luz
		if (lights.size() > 0) {
			for (int i = 0; i < lights.size(); i++) {
				static_cast<TLuz *>(lights[i]->getEntidad())->configureShadow();
			}
		}
		//Dibujamos los distintos nodos del arbol
		scene->draw(shader);

		//==========================================================
		// NO TOQUEIS ESTE FRAGMENTO DE CODIGO
		//==========================================================
		/*
		//DIBUJADO DE LAS SOMBRAS PROYECTADAS
		//-----------------------------------
		//No dibujamos el mapa (suelo) ya que no queremos que produzca ninguna tipo de sombra
		getObjeto("mapa")->setVisible(false);
		//Activamos el shader especifico para dibujar las sombras proyectadas
		shaderProjectedShadows->use();
		
		if (lights.size() > 0) {
			for (int i = 0; i < lights.size(); i++) {
				if (static_cast<TLuz *>(lights[i]->getEntidad())->getActive() == true){
					//Establecemos la matrix view llamando al dibujado de la camara
					drawCamera(shaderProjectedShadows);
					//Le pasamos la matriz proyeccion de la luz (perspectiva)
					glm::mat4 projectionLight = glm::perspective(glm::radians(70.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
					shaderProjectedShadows->setMat4("projectionLight", projectionLight);
					//Le pasamos la posicion de la luz al shader
					shaderProjectedShadows->setVec3("lightPosition", static_cast<TLuz *>(lights[i]->getEntidad())->getPosition());
					shaderProjectedShadows->setVec4("lightDirection", static_cast<TLuz *>(lights[i]->getEntidad())->getDirection());
					//Dibujamos la escena con el shader de sombras proyectadas
					scene->draw(shaderProjectedShadows);
				}
			}
		}
		//Activamos el dibujado del mapa
		getObjeto("mapa")->setVisible(true);*/
		//====================================================
		//====================================================
	}

	//Se activa el shader para el dibujado del HUD
	shaderHUD->use();

	//Dibujamos el hud activo
	activeHud->drawHud(shaderHUD);//}
	
	int display_w, display_h;
	glfwGetFramebufferSize(TMotor::instancia().getVentana(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	if (renderDebug) {
		if (ImGui::GetFrameCount() > 0) {
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	//swap los bufers de pantalla (trasero y delantero)
	glfwSwapBuffers(TMotor::instancia().getVentana());
}


void TMotor::drawCamera(Shader *s) {
	//recorrer el nodo a la inversa hasta la raiz y guardar las matrices que se obtienen de sus entidades
	//para luego multiplicarlas en orden inverso
	std::vector<glm::mat4> matrixAux;
	glm::vec4 defaultVector(0, 0, 0, 1);
	TNodo *aux = activeCamera->getPadre();
	int cont = 0;
	while (aux != scene) {
		matrixAux.push_back(static_cast<TTransform *>(aux->getEntidad())->getMatriz());
		cont++;
		aux = aux->getPadre();
	}
	glm::mat4 viewMatrix;
	//recorrido a la inversa
	for (int i = matrixAux.size() - 1; i >= 0; i--) {
		viewMatrix = matrixAux.at(i) * viewMatrix;
	}

	//calcular posicion de la camara y pasarsela al fragment shader
	glm::vec4 posC = viewMatrix * defaultVector;
	s->setVec3("posCamera", glm::vec3(posC[0], posC[1], posC[2]));

	//por ultimo pasar al shader la view y la projection matrix
	s->setMat4("view", glm::inverse(viewMatrix));
	s->setMat4("projection", activeCamera->getEntidad()->getProjectionMatrix());
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
		static_cast<TLuz *> (activeLights.at(i)->getEntidad())->setLuz(shader, activeLights.at(i)->getName());
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

//Funcion para devolver el hud activo
hud* TMotor::getActiveHud() {
  return activeHud;
}

TGestorRecursos *TMotor::getGR() {
  return gestorRecursos;
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