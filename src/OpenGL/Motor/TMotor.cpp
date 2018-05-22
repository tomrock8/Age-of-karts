#include "TMotor.hpp"

// ---------------------------------------
//  I N S T A N C I A   D E L   M O T O R
// ---------------------------------------

TMotor &TMotor::instancia() {
	static TMotor _instancia; //la primera vez llamara a el constructor de motor
	return _instancia;
}

// ------------------------
//  C O N S T R U C T O R
// ------------------------

TMotor::TMotor() {

	//Inicializar libreria GLFW
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "(TMotor::TMotor()) Error inicializando la libreria GLFW" << std::endl;
	}

	//Parametros de GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Version minima de opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version maxima de opengl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Se usa la version Core de OpenGL

	// CREACION OBJETO VENTANA PARA USAR LAS FUNCIONES DE GLFW
	ventana = glfwCreateWindow(screenWIDTH, screenHEIGHT, "Age of Karts - SocketWar 2017-2018", NULL, NULL);

	//Almacena las dimensiones de la ventana en las variables screen
	glfwGetFramebufferSize(ventana, &screenWIDTH, &screenHEIGHT);

	//Si la ventana no se ha creado correctamente, se cierra GLFW y se muestra un error por terminal
	if (!ventana) {
		glfwTerminate();
		std::cout << "(TMotor::TMotor()) Error creando la ventana" << std::endl;
	}

	//Hacer la ventana creada como actual
	glfwMakeContextCurrent(ventana);
	//Especificar el viewport usado por OpenGL			
	glViewport(0.0f, 0.0f, screenWIDTH, screenHEIGHT);

	//Se inicializa GLEW con los indices a las distintas funciones de OpenGL
	glewExperimental = true;
	//Si no se inicia bien, se muestra un error y se termina la ejecucion
	if (glewInit() != GLEW_OK) {
		cout << "(TMotor::TMotor()) ERROR EN GLEW\n";
		return;
	}

	//Se inicializan algunos aspectos necesarios del motor
	contID = 0; //inicializacion de los id's de obj3D
	gestorRecursos = new TGestorRecursos(); // Inicializacion del gestor de recursos
	nuevaEscenaRaiz();

	//Se inicializa el gestor de sonidos
	gestorSonido = new GestorSonido();
	gestorSonido->setListenerData();

	//Se inicializa el gestor de imagenes
	TGestorImagenes::getInstancia();

	//Se inicializa IMGUI
	initDebugWindow();

	//Creamos los distintos shaders que se van a usar para algun aspecto del motor
	shader = new Shader("shader", "assets/shaders/shaderLightingMap/vertexShader.txt", "assets/shaders/shaderLightingMap/fragmentShader.txt", nullptr);
	shaderHUD = new Shader("shaderHUD", "assets/shaders/shaderHUD/vertexShader.txt", "assets/shaders/shaderHUD/fragmentShader.txt", nullptr);
	shaderProjectedShadows = new Shader("shaderProjectedShadows", "assets/shaders/shaderProjectedShadows/vertexShader.txt", "assets/shaders/shaderProjectedShadows/fragmentShader.txt", nullptr);
	shaderDirectionalDepth = new Shader("shaderDirectionalDepth", "assets/shaders/shaderDepth/shaderLuzDirigida/vertexShader.txt", "assets/shaders/shaderDepth/shaderLuzDirigida/fragmentShader.txt", nullptr);
	shaderPointDepth = new Shader("shaderPointDepth", "assets/shaders/shaderDepth/shaderLuzPuntual/vertexShader.txt", "assets/shaders/shaderDepth/shaderLuzPuntual/fragmentShader.txt"
		, "assets/shaders/shaderDepth/shaderLuzPuntual/geometryShader.txt");
	shaderSkybox = new Shader("shaderSkybox", "assets/shaders/shaderSkybox/vertexShader.txt", "assets/shaders/shaderSkybox/fragmentShader.txt", nullptr);
	shaderDebugBbox = new Shader("shaderDebugBbox", "assets/shaders/shaderDebugAndBbox/vertexShader.txt", "assets/shaders/shaderDebugAndBbox/fragmentShader.txt", nullptr);
	shaderCartoon = new Shader("shaderCartoon", "assets/shaders/shaderCartoon/vertexShader.txt", "assets/shaders/shaderCartoon/fragmentShader.txt", nullptr);
	shaderSilhouette = new Shader("shaderSilhouette", "assets/shaders/shaderSilhouette/vertexShader.txt", "assets/shaders/shaderSilhouette/fragmentShader.txt", nullptr);
	shaderBillboard = new Shader("shaderBillboard", "assets/shaders/shaderBillboard/vertexShader.txt", "assets/shaders/shaderBillboard/fragmentShader.txt", nullptr);
	shaderParticles = new Shader("shaderParticles", "assets/shaders/shaderParticles/vertexShader.txt", "assets/shaders/shaderParticles/fragmentShader.txt", nullptr);
	shaderClouds = new Shader("shaderClouds", "assets/shaders/shaderClouds/vertexShader.txt", "assets/shaders/shaderClouds/fragmentShader.txt", nullptr);
	shaderGbuffer = new Shader("shaderGbuffer", "assets/shaders/shaderDeferred/gBufferVertexShader.txt", "assets/shaders/shaderDeferred/gBufferFragmentShader.txt", nullptr);
	shaderDeferred = new Shader("shaderDeferred", "assets/shaders/shaderDeferred/deferredVertexShader.txt", "assets/shaders/shaderDeferred/deferredFragmentShader.txt", nullptr);
	std::cout << "Version OPENGL: " << glGetString(GL_VERSION) << endl;

	//Inicializamos los buffers para el debug de Bullet
	initializeBuffersDebugBullet();

	//Seteamos el buffer y las texturas que guardaran los datos de posicion, normales y color para el deferred shading
	setDeferredBuffers();

	//Seteamos el buffer para el dibujado del cubo del level of detail
	setBuffersCube();
}

// -------------------------------------------------
//  M E T O D O S   D E   U S O   D E L   M O T O R
// -------------------------------------------------

//Funcion para cambiar los buffers de dibujado de GLFW
void TMotor::swapBuffers() {
	//Se intercambian los dos buffers (delantero y trasero)
	glfwSwapBuffers(TMotor::instancia().getVentana());
}

//Funcion para el control de inputs del motor mediante GLFW
void TMotor::getInputs() {
	glfwPollEvents();
}

//Funcion para crear el nodo raiz del arbol de la escena
void TMotor::nuevaEscenaRaiz() {
	scene = new TNodo("escena_raiz"); // Creacion del nodo raiz (Escena)
}
void TMotor::newGestorRecursos() {
	gestorRecursos = new TGestorRecursos();
}

//Funcion que cierra correctamente el motor
void TMotor::close() {


	//Se eliminan distintos elementos usados por el motor

	delete gestorSonido; //Gestor de sonidos

	//Hud y Menus
	for (int i = 0; i < HUDs.size(); i++) {
		delete HUDs[i];
	}
	//Billboards
	for (int i = 0; i < billboards.size(); i++) {
		delete billboards[i];
	}
	//Sistemas de particulas
	for (int i = 0; i < particleSystems.size(); i++) {
		delete particleSystems[i];
	}

	cleanScene(); // Limpiado de diferentes elementos de la escena
	if (skybox) delete skybox;
	if (nubes) delete nubes;

	delete shader; // Shader que se usa en el motor en la parte 3D
	delete shaderHUD; //Shader que usa el motor para el dibujado del HUD
	delete shaderProjectedShadows; //Shader para el calculo de de las sombras proyectadas
	delete shaderDirectionalDepth; //Shader para el calculo de las sombras de las luces dirigidas
	delete shaderPointDepth; //Shader para el calculo de las sombras de las luces puntuales
	delete shaderSkybox; //Shader para el dibujado del skybox
	delete shaderDebugBbox; //Shader para el modo debug de Bullet
	delete shaderCartoon; //Shader para el efecto cartoon
	delete shaderSilhouette; //Shader para crear el contorno de los objetos en el efecto cartoon
	delete shaderBillboard; //Shader para dibujar los diferentes billboards 
	delete shaderParticles; //Shader para el dibujado de las particulas
	delete shaderClouds; //Shader para el dibujado de las nubes
	delete shaderGbuffer; //Shader para renderizar la escena en el buffer que despues se usara en el deferred shading
	delete shaderDeferred;
}

//Funcion para cambiar la resolucion de la pantalla
void TMotor::changeResolution(int w, int h) {
	//Guardamos el ancho y alto nuevos
	screenWIDTH = w;
	screenHEIGHT = h;
	//Funcion de GLFW para redimensionar la pantalla
	glfwSetWindowSize(ventana, screenWIDTH, screenHEIGHT);
}

//Funcion que inicializa la libreria IMGUI y crea nuevas ventanas
void TMotor::initDebugWindow() {
	renderDebug = true;

	if (ImGui::GetCurrentContext() == NULL) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  //Control mediante teclado
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   //Control mediante mando
		ImGui_ImplGlfwGL3_Init(ventana, true);

		//Establecer estilo de la ventana, en este caso, oscuro
		ImGui::StyleColorsDark();
	}
}

//Funcion que destruye una ventana creada mediante IMGUI
void TMotor::closeDebugWindow() {
	if (ImGui::GetCurrentContext() != NULL) {
		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
	}
	setRenderDebug(false);
}

//Funcion que limpia el array de HUDs del motor
void TMotor::cleanHUD() {
	HUDs.clear();
}

//Funcion para limpiar todos los objetos en cualquier momento
void TMotor::cleanScene() {

	if (scene)
		delete scene;

	//Billboards 
	if (billboards.size() > 0) {
		for (int i = 0; i < billboards.size(); i++) {
			delete billboards[i];
		}
		billboards.clear();
	}

	//Sistemas de particulas
	if (particleSystems.size() > 0) {
		for (int i = 0; i < particleSystems.size(); i++) {
			delete particleSystems[i];
		}
		particleSystems.clear();
	}

	//Hud y Menus
	if (HUDs.size() > 0) {
		for (int i = 0; i < HUDs.size(); i++) {
			delete HUDs[i];
		}
		HUDs.clear();
	}


	cameras.clear();
	lights.clear();
	activeLights.clear();
	notShadowObjects.clear();

	if(TGestorImagenes::getInstancia());
	delete TGestorImagenes::getInstancia();
	
	if (gestorRecursos)
		delete gestorRecursos;

	
}

void TMotor::resizeScreen(int w, int h) {
	screenWIDTH = w;
	screenHEIGHT = h;
}

// ---------------------------------------
//  C R E A C I O N   D E   O B J E T O S
// ---------------------------------------

// ---- CAMARAS ----
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
	TNodo *traslacionNodo = createTransformationNode(rotacionNodo, traslacion, nameTras->c_str());

	// N O D O
	TCamara *camara = createCam();
	camara->setPerspective(screenHEIGHT, screenWIDTH, 0.01f, 10000.0f, 70.0f);
	TNodo *nodo = createCamNode(traslacionNodo, camara, name);
	contID++;
	return new obj3D(nodo, name, contID);

}

// ---- LUCES ----
obj3D *TMotor::newLightNode(const char *name, glm::vec4 dir, float att, float corte, bool shadow, bool active, const char* parentNode) {
	// R O T A C I O N
	string *nameRot = new string("rotacion_" + (string)name);
	TTransform *rotacion = createTransformation();
	TNodo *rotacionNodo = createTransformationNode(getNode(parentNode), rotacion, nameRot->c_str());

	// T R A S L A C I O N
	string *nameTras = new string("traslacion_" + (string)name);
	TTransform *traslacion = createTransformation();
	TNodo *traslacionNodo = createTransformationNode(rotacionNodo, traslacion, nameTras->c_str());

	// N O D O
	TLuz  *luz = new TLuz(glm::vec3(.2f), glm::vec3(.7f), glm::vec3(0.8f), dir, att, corte, shadow, active);
	TNodo *nodo = createLightNode(traslacionNodo, luz, name);
	contID++;
	return new obj3D(nodo, name, contID);
}

//---- ANIMACIONES ----
obj3D *TMotor::newAnimation(const char *name, const char *path, const char* parentNode, int framesIni, int framesFin) {
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
	TAnimacion *anim = TMotor::instancia().createAnimation(path, framesIni, framesFin);
	TNodo  *nodo = TMotor::instancia().createAnimationNode(traslationNodeMesh, anim, name);
	contID++;
	return new obj3D(nodo, name, contID);

}

// ---- OBJETOS 3D ----
obj3D *TMotor::newMeshNode(const char *name, const char *path, const char* parentNode, bool sta) {
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
	TMalla *malla = TMotor::instancia().createMesh(path, sta);
	TNodo  *nodo = TMotor::instancia().createMeshNode(traslationNodeMesh, malla, name);

	if (strcmp(name, "mapa") == 0 || strcmp(name, "elementos") == 0 || strcmp(name, "rueda1") == 0
		|| strcmp(name, "rueda2") == 0 || strcmp(name, "rueda3") == 0 || strcmp(name, "rueda4") == 0) {
		obj3D *obj = new obj3D(nodo, name, contID);
		notShadowObjects.push_back(obj);
		return obj;
	}

	contID++;
	return new obj3D(nodo, name, contID);
}

//----- HUDs Y MENUS -----
void TMotor::newHud(const char* n) {
	//Creamos el nuevo hud con su nombre
	hud* h = new hud(n);
	//Lo añadimos al array de hud
	HUDs.push_back(h);
	//Lo activamos por defecto
	setActiveHud(n);
}

// --- BILLBOARDS ---
billboard *TMotor::newBillboard(obj3D *o) {
	//Creamos el nuevo billboard con su nombre
	billboard* b = new billboard(o);
	//Lo añadimos al array de billboards
	billboards.push_back(b);
	//Lo devolvemos
	return b;
}

// --- SISTEMAS DE PARTICULAS ---
particleSystem *TMotor::newParticleSystem() {
	//Creamos el nuevo billboard con su nombre
	particleSystem *p = new particleSystem();
	//Lo añadimos al array de billboards
	particleSystems.push_back(p);
	//Lo devolvemos
	return p;
}

// --- NUBES ---
void TMotor::newClouds(int minW, int maxW, int minH, int maxH, int minL, int maxL, int numC) {
	//Se crea una area de nubes con los parametros especificados
	nubes = new cloudGenerator(minW, maxW, minH, maxH, minL, maxL, numC);
}

// -------------------------------
// M A L L A S
// -------------------------------

TMalla *TMotor::createMesh(const char *fich, bool sta) {
	return new TMalla(gestorRecursos->loadMesh(fich, sta, false));
}

TNodo  *TMotor::createMeshNode(TNodo *padre, TMalla *mesh, const char* name) {
	TNodo *nodo = new TNodo(name);
	nodo->setPadre(padre);
	nodo->setEntidad(mesh);
	padre->addHijo(nodo);
	return nodo;
}

TNodo  *TMotor::createStaticMeshNode(TNodo*padre, const char* path, const char* name) {
	TNodo *nodo = new TNodo(name);
	nodo->setPadre(padre);
	nodo->setEntidad(createMesh(path, false));
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
	nodo->setPadre(padre);//Se asigna el nodo padre
	nodo->setEntidad(cam);
	padre->addHijo(nodo);
	cameras.push_back(nodo);//se guarda en el vector de camaras con su identificador;
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
	nodo->setPadre(padre); //Se asigna el nodo padre
	nodo->setEntidad(luz);
	padre->addHijo(nodo);
	lights.push_back(nodo); //se guarda en el vector de luces con su identificador;
	//cuando se crea se activa automaticamente
	setActiveLight(nodo);
	return nodo;
}

//------------------------
// A N I M A C I O N E S
//------------------------

TNodo  *TMotor::createAnimationNode(TNodo *padre, TAnimacion *mesh, const char* name) {
	TNodo *nodo = new TNodo(name);
	nodo->setEntidad(mesh);
	if (padre != NULL) {
		nodo->setPadre(padre);
		padre->addHijo(nodo);
	}
	return nodo;
}
void TMotor::addPadre(TNodo *padre, TNodo *hijo) {
	hijo->setPadre(padre);
	padre->addHijo(hijo);
}

TAnimacion *TMotor::createAnimation(const char *path, int framesIni, int framesFin) {
	const char * obj = ".obj";
	string ruta = path;
	ruta += to_string(framesIni);
	ruta += obj;
	std::vector<TMalla*> animation;
	for (int i = framesIni; i < framesFin; i++) {
		string ruta = path;
		ruta += to_string(i);
		ruta += obj;
		animation.push_back(new TMalla(gestorRecursos->loadMesh(ruta.c_str(), false, true)));
	}
	return new TAnimacion(animation, framesIni, framesFin);
}

// ------------------------
//  M E T O D O S   G E T
// ------------------------

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
Shader *TMotor::getShaderDebugBbox() { return shaderDebugBbox; }
Shader *TMotor::getShaderSilhouette() { return shaderSilhouette; }
GestorSonido *TMotor::getGestorSonido() { return gestorSonido; }
glm::mat4 TMotor::getActiveViewMatrix() { return activeViewMatrix; }
bool TMotor::getBoundingBoxes() { return boundingBoxes; }
float TMotor::getDrawingDistance() { return levelOfDrawingDistance; }
bool TMotor::getLevelOfDetail() { return levelOfDetail; }
std::vector <billboard *> TMotor::getBillboards() { return billboards; }
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
hud* TMotor::getActiveHud() { return activeHud; }
TGestorRecursos *TMotor::getGR() { return gestorRecursos; }
//Funcion qude devuelve una camara del array a partir de su posicion en el mismo
TNodo *TMotor::getCameraByIndex(int i) {
	return cameras.at(i);
}

// ------------------------
//  M E T O D O S   S E T
// ------------------------

void TMotor::setActiveCamera(TNodo *cam) { activeCamera = cam; drawCamera(); }
void TMotor::setActiveLight(TNodo *light) { activeLights.push_back(light); }
void TMotor::setRenderDebug(bool renderDebug) { this->renderDebug = renderDebug; }
void TMotor::setDebugBullet(bool b) { debugBullet = b; }
//Funcion que recibe dos puntos para dibujar una linea y los mete en el array de vertices para el debug de Bullet
void TMotor::setVerticesDebug(float a, float b, float c, float x, float y, float z) {
	vertices.push_back(a);
	vertices.push_back(b);
	vertices.push_back(c);
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}
//Funcion para activar un hud, que sera el que se utilice, solo puede haber un hud activo por iteracion
void TMotor::setActiveHud(const char* n) {
	hud* h = getHud(n); //Obtenemos el hud a partir del nombre
	activeHud = h; //Lo activamos
}
//Funcion para establecer el shader que se va a usar
void TMotor::setShaderActive(const char* s) {
	shaderName = s;
}
//Funcion para inicializar el skybox
void TMotor::setSkyBox() {
	skybox = new Skybox();
	skyboxActive = true;
}
//Funcion para establecer el dibujado o no de bounding boxes
void TMotor::setBoundingBoxes(bool b) {
	boundingBoxes = b;
}
//Funcion para activar/desactivar el face culling
void TMotor::setFaceCulling(bool b) {
	faceCulling = b;
}
//Funcion para activar/desactivar el anti-aliasing
void TMotor::setAntialiasing(bool b) {
	if (b == true) {
		glEnable(GL_MULTISAMPLE);
	}
	else {
		glDisable(GL_MULTISAMPLE);
	}
}
//Funcion para activar/desactivar la distancia de dibujado y establecer su nivel
void TMotor::setDrawingDistance(bool b, float f) {
	drawingDistance = b;
	levelOfDrawingDistance = f; //Nivel de clipping
}
//Funcion para activar/desactivar las sombras
void TMotor::setShadows(bool b) {
	projectedShadows = b;
}
//Funcion para activar/desactivar el level of detail
void TMotor::setLevelOfDetail(bool b) {
	levelOfDetail = b;
}
//Funcion para activar/desactivar las nubes
void TMotor::setClouds(bool b) {
	clouds = b;
}
obj3D* TMotor::getObjActiveCamera() {
	if (cameras.size() > 0)
		return new obj3D(activeCamera, activeCamera->getName(), 99);

	return NULL;
}
//Funcion para establecer el viewport (porcion de la pantalla) que se va a renderizar
void TMotor::setViewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

//------------------------------
// D I B U J A D O
//------------------------------

// ---- LIMPIEZA PRE-DIBUJADO ---- 
void TMotor::clean(float r, float g, float b, float a) {
	//Establecer el nuevo color de fondo
	glClearColor(r, g, b, a);
	//Limpiamos los buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Activamos el z-buffer
	glEnable(GL_DEPTH_TEST);
	//Activamos la transparencia
	glEnable(GL_BLEND);
	//Como cada uno de los canales rgba es calculado/computado
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// ---- DIBUJADO DEL ARBOL DE LA ESCENA ---- 
//Funcion que dibuje los objetos 3D creados en el motor con el estilo que hayas definido
void TMotor::draw() {
	if (faceCulling == true) {
		//Activamos el Face Culling (Si esta activado)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); //No dibujamos las caras traseras de los objetos
	}
	//Si el shader deferred esta activado...
	if (strcmp(shaderName, "shaderDeferred") == 0) {
		usingShaderDeferred();
	}
	//Si el shader cartoon esta activado...
	if (strcmp(shaderName, "shaderCartoon") == 0) {
		usingShaderCartoon();
	}

	//Desactivamos el Face Culling para que no afecte al resto de elementos dibujados
	glDisable(GL_CULL_FACE);

}

// ---- DIBUJADO DE LA CAMARA ACTIVA ---- 
//Funcion que calcula la viewMatrix de la camara activa
void TMotor::drawCamera() {
	//recorrer el nodo a la inversa hasta la raiz y guardar las matrices que se obtienen de sus entidades
	//para luego multiplicarlas en orden inverso
	std::vector<glm::mat4> matrixAux;
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

	//Guardamos la viewMatrix para calcular despues la MVP de cada objeto
	activeViewMatrix = glm::inverse(viewMatrix);
}

// ---- DIBUJADO DE LAS LUCES ---- 
//Funcion que establece los datos de cada luz del motor para su dibujado
void TMotor::drawLight(Shader *s) {
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
		static_cast<TLuz *> (activeLights.at(i)->getEntidad())->setLuz(s, activeLights.at(i)->getName());
	}
}


// ---- DIBUJADO DEL SKYBOX ---- 
//Funcion que dibuja el skybox del motor, si este esta activado
void TMotor::drawSkybox() {
	if (skyboxActive == true) {
		//Activamos el shader especifico del skybox
		shaderSkybox->use();
		//Llamamos al metodo de dibujado del skybox
		skybox->drawSkyBox();
	}
}

// ---- DIBUJADO DEL HUDs Y MENUS ----
//Funcion que dibuja el hud activo 
void TMotor::drawHudMenus() {
	//Desactivamos el z-buffer, queremos dibujarlo encima de todo
	glDisable(GL_DEPTH_TEST);
	//Se activa el shader para el dibujado del HUD
	shaderHUD->use();
	//Dibujamos el hud activo
	activeHud->drawHud(shaderHUD);
	//Desactivamos el z-buffer, queremos dibujarlo encima de todo
	glEnable(GL_DEPTH_TEST);
}

// ---- DIBUJADO DE LAS VENTANAS DE IMGUI ----
//Funcion que dibuja las ventanas creadas mediante la libreria IMGUI
void TMotor::drawIMGUI() {
	if (renderDebug) {
		if (ImGui::GetFrameCount() > 0) {
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
}

// ---- DIBUJADO DE LOS BILLBOARDS ----
//Funcion que dibuja los billboards del motor
void TMotor::drawBillboards() {
	//Desactivamos el z-buffer, queremos dibujarlo encima de todo
	glDisable(GL_DEPTH_TEST);
	//Activamos el shader para el dibujado de los billboards
	shaderBillboard->use();
	//Recorremos los diferentes billboards y los dibujamos
	for (int i = 0; i < billboards.size(); i++) {
		billboards.at(i)->draw(shaderBillboard);
	}
	//Reactivamos el z-buffer, para controlar la profundidad
	glEnable(GL_DEPTH_TEST);
}

// ---- DIBUJADO DE LOS SISTEMAS DE PARTICULAS ----
//Funcion que dibuja los distintos sistemas de particulas presentes en el motor
void TMotor::drawParticles() {
	//Activamos el shader para el dibujado de las particulas
	shaderParticles->use();
	//Recorremos los diferentes sistemas y los dibujamos
	for (int i = 0; i < particleSystems.size(); i++) {
		particleSystems.at(i)->draw(shaderParticles);
	}
}

// ---- DIBUJADO DE LAS NUBES ----
//Funcion que dibuja las nubes del motor
void TMotor::drawClouds() {
	//Si las nubes estan activadas, las dibujamos...
	if (clouds == true) {
		//Activamos el shader especifico de las nubes
		shaderClouds->use();
		//Dibujamos las nubes
		nubes->draw(shaderClouds);
	}
}

// ---- DIBUJADO DE LAS SOMBRAS PROYECTADAS ----
//Funcion que dibuja las sombras de los objetos a partir de la tecnica de las sombras proyectadas
void TMotor::drawProjectedShadows() {
	if (projectedShadows == true) { //Si las sombras estan activadas...
		//No dibujamos aquellos elementos que no queremos que proyecten sombras
		for (int i = 0; i < notShadowObjects.size(); i++) {
			notShadowObjects.at(i)->setVisible(false);
		}
		//Activamos el shader especifico para dibujar las sombras proyectadas
		shaderProjectedShadows->use();
		if (lights.size() > 0) {
			for (int i = 0; i < lights.size(); i++) {
				if (static_cast<TLuz *>(lights[i]->getEntidad())->getActive() == true) {
					//Le pasamos al shader la matriz view
					shaderProjectedShadows->setMat4("view", activeViewMatrix);
					//Le pasamos la matriz proyeccion de la luz (perspectiva)
					glm::mat4 projectionLight = glm::perspective(glm::radians(70.0f), (float)screenWIDTH / (float)screenHEIGHT, 0.01f, 10000.0f);
					shaderProjectedShadows->setMat4("projectionLight", projectionLight);
					//Le pasamos la posicion de la luz al shader
					shaderProjectedShadows->setVec3("lightPosition", static_cast<TLuz *>(lights[i]->getEntidad())->getPosition());
					shaderProjectedShadows->setVec4("lightDirection", static_cast<TLuz *>(lights[i]->getEntidad())->getDirection());
					glm::mat4 sc;
					sc = scale(sc, glm::vec3(1.1, 1, 1.1));
					//sc = glm::translate(sc, glm::vec3())
					shaderProjectedShadows->setMat4("scale", sc);
					//Dibujamos la escena con el shader de sombras proyectadas
					scene->draw(shaderProjectedShadows);
				}
			}
		}
		//Reactivamos el dibujado de los elementos 
		for (int i = 0; i < notShadowObjects.size(); i++) {
			notShadowObjects.at(i)->setVisible(true);
		}
	}
}

// ---- DIBUJADO DEL SHADOW MAPPING ----
//Funcion que realiza los calculos necesarios para el dibujado de las sombras mediante la tecnica del shadow mapping
void TMotor::drawMappingShadows() {
	//Llamamos al render de las luces para calcular el depth map que se usara para calcular las sombras
	if (lights.size() > 0) {
		for (int i = 0; i < lights.size(); i++) {
			if (static_cast<TLuz *>(lights[i]->getEntidad())->getActive() == true) {
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
}

// ---- DIBUJADO DEL DEBUG DE BULLET ----
//Funcion que dibuja los elementos (cajas de colision, fisicas...) de Bullet para su debugeo
void TMotor::drawDebugBullet() {
	if (debugBullet && vertices.size() > 0) { //Si el debug de bullet esta activado
		//Activamos el VAO
		glBindVertexArray(debugBulletVAO);
		//Activamos el VBO, al que se le pasan los datos de posicion de cada vertice
		glBindBuffer(GL_ARRAY_BUFFER, debugBulletVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		//Activamos el shader del debug
		shaderDebugBbox->use();
		//Creamos y le pasamos la matriz mvp al shader
		glm::mat4 model;
		glm::mat4 mvp = activeCamera->getEntidad()->getProjectionMatrix() * activeViewMatrix * model;
		shaderDebugBbox->setMat4("mvp", mvp);
		//Dibujamos las lineas del debug de rojo
		shaderDebugBbox->setVec4("color", glm::vec4(1.0, 0.0, 0.0, 1.0));
		//Establecemos el ancho de las lineas
		glLineWidth(3.0f);
		//Llamamos al dibujado de las distintas lineas
		glDrawArrays(GL_LINES, 0, vertices.size());
		//Desactivamos los buffers usados para dibujar las lineas
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//Limpiamos el vector de vertices para la siguiente iteracion
		vertices.clear();
	}
}

//---------------------------------------------------------
// D I B U J A D O   S E G U N   S H A D E R   A C T I V O
//---------------------------------------------------------

// ---- SHADER CARTOON ----
//Funcion que renderiza la escena aplicandole un estilo cartoon
void TMotor::usingShaderCartoon() {

	//1º RENDERIZADO = se renderizan todos los objetos de forma normal con sus texturas con un estilo cartoon
	//Se activa el shader para el renderizado 3D
	shaderCartoon->use();
	//Calcular posicion de la camara y pasarsela al fragment shader
	glm::vec4 defaultVector(0, 0, 0, 1);
	glm::vec4 posC = glm::inverse(activeViewMatrix) * defaultVector;
	shaderCartoon->setVec3("posCamera", glm::vec3(posC[0], posC[1], posC[2]));
	//Le pasamos al shader si la distancia de dibujado esta o no activada y el nivel del mismo
	shaderCartoon->setBool("drawingDistance", drawingDistance);
	shaderCartoon->setFloat("levelOfDrawingDistance", levelOfDrawingDistance);
	//Establecemos los datos de las distintas luces
	drawLight(shaderCartoon);
	//Si el debug de bullet no esta activo
	if (!debugBullet) {
		//Activamos el glPolygonOffset = a cada fragmento de los objetos se le añade una pequeña profundidad antes de realizar los calculos del z-buffer
		glEnable(GL_POLYGON_OFFSET_FILL);
		//Profundidad que se le añade
		float line_offset_slope = -1.f;
		//Corte maximo de profundidad
		float line_offset_unit = -35.f;
		//Especificamos los valores anteriores a OpenGL
		glPolygonOffset(line_offset_slope, line_offset_unit);
	}
	//Dibujamos los distintos nodos del arbol
	scene->draw(shaderCartoon);
	//2º RENDERIZADO = se renderizan los objetos en modo wireframe (solo las caras ocultas) para crear la silueta de los mismos

	//Se activa el shader para el renderizado 3D
	shaderSilhouette->use();
	//Desactivamos el offset anterior
	glDisable(GL_POLYGON_OFFSET_FILL);
	//Establecemos el dibujado del poligono en modo linea
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Activamos el cull face
	glEnable(GL_CULL_FACE);
	//Activamos el suavizado de las lineas
	glEnable(GL_LINE_SMOOTH);
	//Especificamos que solo queremos ocultar las caras frontales
	glCullFace(GL_FRONT);
	//Aumentamos el grosor de las lineas
	glLineWidth(4.50f);
	//Pasamos la posicion de la camara al shader para las operaciones de clipping
	shaderSilhouette->setVec3("posCamera", glm::vec3(posC[0], posC[1], posC[2]));
	//Le pasamos al shader si la distancia de dibujado esta o no activada y el nivel del mismo
	shaderSilhouette->setBool("drawingDistance", drawingDistance);
	shaderSilhouette->setFloat("levelOfDrawingDistance", levelOfDrawingDistance);
	//Dibujamos los distintos nodos del arbol
	scene->draw(shaderSilhouette);
	//Volvemos al modo de dibujado normal de los poligonos
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Desactivamos el cull face
	glDisable(GL_CULL_FACE);
}

// ---- SHADER DEFERRED ----
//Funcion que renderiza usando un shader deferred
void TMotor::usingShaderDeferred() {
	glDisable(GL_BLEND);
	//1º DIBUJADO = se guardan los datos de posicion, normales y color de los distintos objetos en el buffer mediante las texturas

	//Enlazamos el buffer
	glBindFramebuffer(GL_FRAMEBUFFER, defBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Activamos el shader que guardara los datos
	shaderGbuffer->use();
	//Dibujamos los distintos nodos del arbol
	scene->draw(shaderGbuffer);
	//Desenlazamos el buffer usado
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_BLEND);

	//2º DIBUJADO = se renderiza la imagen por pantalla en un quad a partir de los datos anteriores

	//Se limpian los buffers de color y profundidad de OpenGL
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Activamos el shader
	shaderDeferred->use();
	//Se activan las texturas con los datos de los objetos y se le pasan al shader
	//POSICION
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defPosition);
	glUniform1i(glGetUniformLocation(shaderDeferred->getID(), "texture_position"), 0);
	//NORMALES
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, defNormal);
	glUniform1i(glGetUniformLocation(shaderDeferred->getID(), "texture_normal"), 1);
	//COLOR DIFUSO Y BRILLO
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, defDiffuseSpecular);
	glUniform1i(glGetUniformLocation(shaderDeferred->getID(), "texture_diffuseSpecular"), 2);
	//Establecemos los datos de las distintas luces
	drawLight(shaderDeferred);
	//Calcular posicion de la camara y pasarsela al fragment shader
	glm::vec4 defaultVector(0, 0, 0, 1);
	glm::vec4 posC = activeViewMatrix * defaultVector;
	shaderDeferred->setVec3("posCamera", glm::vec3(posC[0], posC[1], posC[2]));
	unsigned int quadVAO, quadVBO;
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
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

//---------------------------------
// D E B U G   B U L L E T
//---------------------------------

//Funcion que inicializa los buffers que usara el debug para guardar y pasar los datos de las lineas de Bullet
void TMotor::initializeBuffersDebugBullet() {
	//Creamos los buffers de OpenGl
	glGenVertexArrays(1, &debugBulletVAO);
	glGenBuffers(1, &debugBulletVBO);
}

//---------------------------------
// D E F E R R E D   S H A D I N G
//---------------------------------

//Funcion que setea el buffer y texturas necesarias
void TMotor::setDeferredBuffers() {
	//Creamos y enlazamos el buffer del deferred shading
	glGenFramebuffers(1, &defBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, defBuffer);

	//Creamos las texturas que guardaran los distintos datos de posicion, normales y color
	glGenTextures(1, &defPosition);
	glGenTextures(1, &defNormal);
	glGenTextures(1, &defDiffuseSpecular);

	//Definimos cada una de las distintas texturas
	// --- TEXTURA DE POSICION ---
	glBindTexture(GL_TEXTURE_2D, defPosition); //Decimos que se trata de una textura 2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenWIDTH, screenHEIGHT, 0, GL_RGBA, GL_FLOAT, NULL); //Utilizamos una textura de 16 bits de precision
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //|
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //| Parametros de la textura
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, defPosition, 0); //Adjuntamos la textura al frambuffer definido

	// --- TEXTURA DE NORMALES ---
	glBindTexture(GL_TEXTURE_2D, defNormal); //Decimos que se trata de una textura 2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenWIDTH, screenHEIGHT, 0, GL_RGBA, GL_FLOAT, NULL); //Utilizamos una textura de 16 bits de precision
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //|
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //| Parametros de la textura
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, defNormal, 0); //Adjuntamos la textura al frambuffer definido

	// --- TEXTURA DE COLOR ---
	glBindTexture(GL_TEXTURE_2D, defDiffuseSpecular); //Decimos que se trata de una textura 2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWIDTH, screenHEIGHT, 0, GL_RGBA, GL_FLOAT, NULL); //Utilizamos una textura de 8 bits de precision
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //|
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //| Parametros de la textura
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, defDiffuseSpecular, 0); //Adjuntamos la textura al frambuffer definido

	//Le decimos a OpenGL las texturas adjuntas que va a utilizar para renderizar en el buffer
	GLenum textAdjuntas[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, textAdjuntas);

	//Desenlazamos el buffer hasta el dibujado
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//---------------------------------
// L E V E L   O F   D E T A I L
//---------------------------------

/* El Level Of Detail no esta completo en su totatildad, solo es una simple muestra. Como los modelos de nuestro juego ya son low-poly, realmente no era algo
imprescindible para hacerlo funcionar correctamente. Es por eso, que para cumplir con los terminos presupuestados, hemos implementado
un level of detail basico, que simplemente, en funcion de la distancia de dibujado seteada en el motor, sustituye las mallas por un simple cubo. Sigue
el mismo principio que cualquier Level Of Detail, pero sin usar modelos con menos poligonos de las mallas */

//Funcion que establece los buffers de OpenGL para el dibujado de un cubo
void TMotor::setBuffersCube() {
	//Array con la posicion de los vertices del cubo
	const float vertices_cubo[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	//Establecemos los buffers que pasaran los datos anteriores a los shaders para su dibujado
	//Creamos un VAO y un VBO
	glGenVertexArrays(1, &VAO_cube);
	glGenBuffers(1, &VBO_cube);
	//Activamos el VAO
	glBindVertexArray(VAO_cube);
	//Activamos el VBO, al que se le pasan los datos de posicion de cada vertice
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cubo), vertices_cubo, GL_STATIC_DRAW);
	//POSICION
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//Desactivamos el VAO hasta que dibujemos
	glBindVertexArray(0);
}

//Funcion para dibujar un cubo
void TMotor::drawCube(glm::mat4 modelMatrixObject, glm::vec3 centerPos) {
	//Activamos el shader debug para el dibujado del cubo (no queremos el mismo shader que para el resto de objetos)
	shaderDebugBbox->use();
	//Calculamos la matriz model del cubo
	glm::mat4 model; //Matriz identidad
	model = glm::translate(glm::mat4(1), centerPos); //Primero, trasladamos el cubo al centro del objeto
	model = modelMatrixObject * model; //Y por ultimo, lo traslamos al lugar que ocupa la malla en el mundo
	//Calculamos la matriz mvp a partir de la projection y view de la camara activa
	glm::mat4 mvp = TMotor::instancia().getActiveCamera()->getEntidad()->getProjectionMatrix() * TMotor::instancia().getActiveViewMatrix() * model;
	//Pasamos los datos necesarios al shader 
	TMotor::instancia().getShaderSkybox()->setMat4("mvp", mvp); //Matriz mvp
	//Pasamos un color transparente, no queremos pintar los cubos
	TMotor::instancia().getShaderDebugBbox()->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 0.0));
	//Activamos el VAO que hemos seteado antes
	glBindVertexArray(VAO_cube);
	//Dibujamos el cubo
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//Desactivamos el VAO
	glBindVertexArray(0);
}

