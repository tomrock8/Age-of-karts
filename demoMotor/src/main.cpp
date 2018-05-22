#include "TMotor.hpp"

// --- DEFINICION DE FUNCIONES ---
void resize_callback(GLFWwindow* window, int width, int height);
void processInputs();
void updateIMGUI();
obj3D *findObject(const char* n);

// --- DEFINICION DE VARIABLES ---
std::vector<obj3D *> objetos; //Vector para guardar los objetos creados
particleSystem* particulas; //Ejemplo de un sistema de particulas

// --- VARIABLES DE IMGUI ---

// **** Creacion de Objetos ****
	//HACHA
	bool axe = false;
	float axe_position[] = {0.0f, 0.0f, 0.0f};
	float axe_rotation[] = {0.0f, 0.0f, 0.0f};
	float axe_scale[] = {1.0f, 1.0f, 1.0f};
	//CAJA
	bool box = false;
	float box_position[] = {0.0f, 0.0f, 0.0f};
	float box_rotation[] = {0.0f, 0.0f, 0.0f};
	float box_scale[] = {1.0f, 1.0f, 1.0f};
	//PIRATA
	bool pirate = false;
	float pirate_position[] = {0.0f, 0.0f, 0.0f};
	float pirate_rotation[] = {0.0f, 0.0f, 0.0f};
	float pirate_scale[] = {1.0f, 1.0f, 1.0f};
	//COCHE
	bool car = false;
	float car_position[] = {0.0f, 0.0f, 0.0f};
	float car_rotation[] = {0.0f, 0.0f, 0.0f};
	float car_scale[] = {1.0f, 1.0f, 1.0f};

// **** Otras Funciones ****
	//SKYBOX
	bool sky = true;
	//BOUNDING BOXES
	bool bbox = false;
	//BILLBOARDS
	bool billboard = false;
	//PARTICULAS
	bool particles = false;
	float particles_position[] = {0.0f, 0.0f, 0.0f};
	float particles_color[] = {100.0f, 100.0f, 100.0f};
	float particles_size = 15.0f;
	//NUBES
	bool clouds = false;
	//HUD
	bool hud = false;
	float hud_position[] = {0.0f, 0.0f};
	float hud_rotation = 0.0f;
	float hud_scale[] = {1.0f, 1.0f};

// **** Optimizaciones ****
	//FACE CULLING
	bool faceCulling = false;
	//ANTI-ALIASING
	bool antialiasing = false;
	//DISTANCIA DE DIBUJADO
	bool drawingDistance = false;
	float levelOfDrawingDistance = 200.0f;


//Main del juego
int main(int argc, char* argv[]) {

	//Se inicializa el motor
	TMotor::instancia();

	//Parametros del motor
	TMotor::instancia().setShaderActive("shaderCartoon"); //Tipo de shader a usar
	TMotor::instancia().setSkyBox(); //Queremos skybox
	TMotor::instancia().setBoundingBoxes(true); //No queremos bounding boxes

	//Evento de glfw para redimensionar la pantalla
	glfwSetFramebufferSizeCallback(TMotor::instancia().getVentana(), resize_callback);

	//Incializamos la ventana de IMGUI
	TMotor::instancia().initDebugWindow();

	//Se crea la camara
	TMotor::instancia().newCameraNode("Camara", "escena_raiz")->setPosition(0.0,0.0,25.0f);

	//Creacion de OBJETOS
	objetos.push_back(TMotor::instancia().newMeshNode("Hacha","assets/hacha/hacha.obj", "escena_raiz", false)); 
	objetos.push_back(TMotor::instancia().newMeshNode("Caja","assets/caja/caja.obj", "escena_raiz", false));
	objetos.push_back(TMotor::instancia().newMeshNode("Pirata","assets/Pirata/p2.obj", "escena_raiz", false));
	objetos.push_back(TMotor::instancia().newMeshNode("Coche","assets/Karts/Pirata/kart_pirata.obj", "escena_raiz", false));

	//Creacion de billboards
	TMotor::instancia().newBillboard(findObject("Hacha"))->setImage("assets/logoAOK.png");
	TMotor::instancia().newBillboard(findObject("Caja"))->setImage("assets/logoAOK.png");
	TMotor::instancia().newBillboard(findObject("Pirata"))->setImage("assets/logoAOK.png");
	TMotor::instancia().newBillboard(findObject("Coche"))->setImage("assets/logoAOK.png");

	//Creacion de las particulas
	particulas = TMotor::instancia().newParticleSystem();
	particulas->setType(1);
	particulas->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	//Creacion del HUD
	TMotor::instancia().newHud("Ejemplo");
	TMotor::instancia().getActiveHud()->addElementHud(0.3f, 0.3f, "ej", "assets/logoAOK.png");
	TMotor::instancia().getActiveHud()->traslateElement("ej", 0.0f, 0.0f);

	//Creacion de nubes
	TMotor::instancia().newClouds(-2000, 2000, 150, 300, -2000, 2000, 50);

	//Mientras la aplicacion no se cierre...
	while(!glfwWindowShouldClose(TMotor::instancia().getVentana())){

		//Limpiamos el frame anterior
		TMotor::instancia().clean(0.16f, 0.533f, 0.698f, 0.0f);

		//Actualizamos IMGUI
		updateIMGUI();

		//Control de inputs
		processInputs();

		//Especificamos el viewport de OpenGL
		TMotor::instancia().setViewport(0, 0, TMotor::instancia().getWidth(), TMotor::instancia().getHeight()); //Pantalla completa

		//Especificamos la camara activa
		TMotor::instancia().setActiveCamera(TMotor::instancia().getCameraByIndex(0));

		//Dibujamos el skybox
		TMotor::instancia().drawSkybox();
		//Dibujamos las nubes
		TMotor::instancia().drawClouds();
		//Dibujamos los objetos 3D creados en la escena
		TMotor::instancia().draw();
		//Dibujamos las particulas
		if (particles) TMotor::instancia().drawParticles();
		//Dibujamos los billboards
		if (billboard) TMotor::instancia().drawBillboards();
		//Dibujamos el HUD
		if (hud) TMotor::instancia().drawHudMenus();
		//Dibujamos la ventana de IMGUI
		TMotor::instancia().drawIMGUI();

		//Intercambiamos buffers
		TMotor::instancia().swapBuffers();
		//Recogemos eventos de teclado
		TMotor::instancia().getInputs();
	}

	//Cerramos la ventana de IMGUI
	TMotor::instancia().closeDebugWindow();
	//Cerramos el motor 
	TMotor::instancia().close();
	//Salimos del programa
	return 0;
}

//Manejador para el evento de redimensionamiento de la pantalla
void resize_callback(GLFWwindow* ventana, int ancho, int alto) {
	TMotor::instancia().resizeScreen(ancho,alto);
	//Se redimensiona el viewport de Opengl en funcion de las medidas de la ventana
	glViewport(0, 0, ancho, alto);
}

//Funcion que recoge los inputs del teclado y actua en consecuencia
void processInputs(){

	//Si se aprieta la tecla ESC, se sale del programa
	 if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(TMotor::instancia().getVentana(), true);

}

//Funcion para crear y actualizar la ventana de IMGUI
void updateIMGUI(){
	//Se actualiza la ventana en cada frame
	ImGui_ImplGlfwGL3_NewFrame();

	//CONTENIDO DE LA VENTANA
	//-----------------------

	ImGui::Text("Render: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Separator();
	ImGui::Text(" > ENGINE DEMO < ");
	ImGui::Text("-----------------");

	//**** Creacion de Objetos ****
	if (ImGui::CollapsingHeader("Object Creation")) {

		//HACHA
		ImGui::Checkbox("Axe", &axe);
		if (axe == true){
			//Posicion
			ImGui::SliderFloat3("Position_Axe", axe_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation_Axe", axe_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale_Axe", axe_scale, -10, 10, "%.0f", 1.0f);
		}

		//CAJA
		ImGui::Checkbox("Box", &box);
		if (box == true){
			//Posicion
			ImGui::SliderFloat3("Position_Box", box_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation_Box", box_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale_Box", box_scale, -10, 10, "%.0f", 1.0f);
		}

		//PIRATA
		ImGui::Checkbox("Pirate", &pirate);
		if (pirate == true){
			//Posicion
			ImGui::SliderFloat3("Position_Pirate", pirate_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation_Pirate", pirate_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale_Pirate", pirate_scale, -10, 10, "%.0f", 1.0f);
		}

		//COCHE
		ImGui::Checkbox("Car", &car);
		if (car == true){
			//Posicion
			ImGui::SliderFloat3("Position_Car", car_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation_Car", car_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale_Car", car_scale, -10, 10, "%.0f", 1.0f);
		}

	}

	// **** Otras Funciones ****
	if (ImGui::CollapsingHeader("Another Functions")) {
		ImGui::Checkbox("SkyBox", &sky);
		ImGui::Checkbox("Bounding Boxes", &bbox);
		ImGui::Checkbox("Billboards", &billboard);
		ImGui::Checkbox("Particles", &particles);
		if (particles == true){
			ImGui::SliderFloat3("Position_Particles", particles_position, -100, 100, "%.0f", 1.0f);
			ImGui::SliderFloat3("Color_Particles", particles_color, 0, 100, "%.0f", 1.0f);
			ImGui::SliderFloat("Size_Particles", &particles_size, 0, 100, "%.0f", 1.0f);
		}
		ImGui::Checkbox("Clouds", &clouds);
		ImGui::Checkbox("HUD Example", &hud);
		if (hud == true){ 
			ImGui::SliderFloat2("Position_HUD", hud_position, -100, 100, "%.0f", 1.0f);
			ImGui::SliderFloat("Rotation_HUD", &hud_rotation, 0, 360, "%.0f", 1.0f);
			ImGui::SliderFloat2("Scale_HUD", hud_scale, -10, 10, "%.0f", 1.0f);
		}
	}

	// **** Optimizaciones ****
	if (ImGui::CollapsingHeader("Optimizations")) {
		ImGui::Checkbox("Face Culling", &faceCulling);
		ImGui::Checkbox("Anti-Aliasing", &antialiasing);
		ImGui::Checkbox("Drawing Distance", &drawingDistance);
		if (drawingDistance == true){
			ImGui::SliderFloat("Distance", &levelOfDrawingDistance, 1.0f, 200.0f, "%.1f", 1.0f);
		}
		
	}

	//Llamado de funciones del motor

	//**** Creacion de Objetos ****
	//HACHA
	findObject("Hacha")->setVisible(axe);
	findObject("Hacha")->setPosition(axe_position[0], axe_position[1], axe_position[2]);
	findObject("Hacha")->setRotation(axe_rotation[0], axe_rotation[1], axe_rotation[2]);
	findObject("Hacha")->setScale(axe_scale[0], axe_scale[1], axe_scale[2]);
	//CAJA
	findObject("Caja")->setVisible(box);
	findObject("Caja")->setPosition(box_position[0], box_position[1], box_position[2]);
	findObject("Caja")->setRotation(box_rotation[0], box_rotation[1], box_rotation[2]);
	findObject("Caja")->setScale(box_scale[0], box_scale[1], box_scale[2]);
	//PIRATA
	findObject("Pirata")->setVisible(pirate);
	findObject("Pirata")->setPosition(pirate_position[0], pirate_position[1], pirate_position[2]);
	findObject("Pirata")->setRotation(pirate_rotation[0], pirate_rotation[1], pirate_rotation[2]);
	findObject("Pirata")->setScale(pirate_scale[0], pirate_scale[1], pirate_scale[2]);
	//COCHE
	findObject("Coche")->setVisible(car);
	findObject("Coche")->setPosition(car_position[0], car_position[1], car_position[2]);
	findObject("Coche")->setRotation(car_rotation[0], car_rotation[1], car_rotation[2]);
	findObject("Coche")->setScale(car_scale[0], car_scale[1], car_scale[2]);

	//**** Otras Funciones *****
	//SKYBOX
	TMotor::instancia().setActiveSky(sky);
	//BOUNDING BOX
	TMotor::instancia().setBoundingBoxes(bbox);
	//PARTICLES
	particulas->setPosition(glm::vec3(particles_position[0], particles_position[1], particles_position[2]));
	particulas->setColor(glm::vec3(particles_color[0]/100.0f, particles_color[1]/100.0f, particles_color[2]/100.0f));
	particulas->setSize(particles_size/100.0f);
	//NUBES
	TMotor::instancia().setClouds(clouds);
	//HUD
	TMotor::instancia().getActiveHud()->traslateElement("ej", hud_position[0]/100.0f, hud_position[1]/100.0f);
	TMotor::instancia().getActiveHud()->rotateElement("ej", hud_rotation);
	TMotor::instancia().getActiveHud()->scaleElement("ej", hud_scale[0], hud_scale[1]);

	// **** Optimizaciones ****
	//FACE CULLING
	TMotor::instancia().setFaceCulling(faceCulling);
	//ANTI-ALIASING
	TMotor::instancia().setAntialiasing(antialiasing);
	//DISTANCIA DE DIBUJADO
	TMotor::instancia().setDrawingDistance(drawingDistance, levelOfDrawingDistance);

}

//Funcion que devuelve un objeto en funcion de su nombre
obj3D *findObject(const char* n){
	obj3D * o = NULL;
	//Recorremos el array de objetos
	for(int i = 0; i < objetos.size(); i++){
		//Si coincide el nombre del objeto, lo guardamos
		if (strcmp(objetos.at(i)->getName(), n) == 0){
			o = objetos.at(i);
		}
	}
	//Devolvemos el objeto
	return o;
}