#include "TMotor.hpp"

// --- DEFINICION DE FUNCIONES ---
void resize_callback(GLFWwindow* window, int width, int height);
void processInputs();
void updateIMGUI();
obj3D *findObject(const char* n);
obj3D *findPointLight(const char* n);
obj3D *findSpotLight(const char* n);

// --- DEFINICION DE VARIABLES ---
obj3D* camara; //Camara
std::vector<obj3D *> objetos; //Vector para guardar los objetos creados
obj3D* suelo; //Suelo de ejemplo para realizar pruebas
obj3D* pared; //Pared de ejemplo para realizar pruebas
particleSystem* particulas; //Ejemplo de un sistema de particulas
std::vector<obj3D*> lucesPuntuales; //Vector de luces puntuales
std::vector<obj3D*> lucesDirigidas; //Vector de luces dirigidas
obj3D* luzSolar; //Luz del sol

// --- VARIABLES DE IMGUI ---

// **** Creacion de Objetos ****
	//SUELO
	bool onTheFloor = true;
	float floor_position[] = {0.0f, -3.0f, 0.0f};
	//PARED
	bool wall = false;
	float wall_position[] = {0.0f, 0.0f, 0.0f};
	float wall_rotation[] = {0.0f, 0.0f, 90.0f};
	//HACHA
	bool axe = false;
	float axe_position[] = {0.0f, 0.0f, 0.0f};
	float axe_rotation[] = {0.0f, 0.0f, 0.0f};
	float axe_scale[] = {100.0f, 100.0f, 100.0f};
	//CAJA
	bool box = false;
	float box_position[] = {0.0f, 0.0f, 0.0f};
	float box_rotation[] = {0.0f, 0.0f, 0.0f};
	float box_scale[] = {100.0f, 100.0f, 100.0f};
	//PIRATA
	bool pirate = false;
	float pirate_position[] = {0.0f, 0.0f, 0.0f};
	float pirate_rotation[] = {0.0f, 0.0f, 0.0f};
	float pirate_scale[] = {100.0f, 100.0f, 100.0f};
	//COCHE
	bool car = false;
	float car_position[] = {0.0f, 0.0f, 0.0f};
	float car_rotation[] = {0.0f, 0.0f, 0.0f};
	float car_scale[] = {100.0f, 100.0f, 100.0f};

// **** LUCES ****
	//LUCES PUNTUALES
		//LUZ PUNTUAL 1
		bool pointLight_1 = false;
		float pointLight1_position[] = {0.0f, 5.0f, 0.0f};
		float pointLight1_ambient[] = {20.0f, 20.0f, 20.0f};
		float pointLight1_diffuse[] = {70.0f, 70.0f, 70.0f};
		float pointLight1_specular[] = {80.0f, 80.0f, 80.0f};
		bool pointLight1_shadows = false;
		//LUZ PUNTUAL 2
		bool pointLight_2 = false;
		float pointLight2_position[] = {0.0f, 5.0f, 0.0f};
		float pointLight2_ambient[] = {20.0f, 20.0f, 20.0f};
		float pointLight2_diffuse[] = {70.0f, 70.0f, 70.0f};
		float pointLight2_specular[] = {80.0f, 80.0f, 80.0f};
		bool pointLight2_shadows = false;
		//LUZ SOLAR
		bool sunLight = true;
		float sunLight_position[] = {0.0f, 100.0f, 0.0f};
		float sunLight_ambient[] = {20.0f, 20.0f, 20.0f};
		float sunLight_diffuse[] = {70.0f, 70.0f, 70.0f};
		float sunLight_specular[] = {80.0f, 80.0f, 80.0f};
		bool projectedShadows = false;
	//LUCES DIRIGIDAS
		//LUZ DIRIGIDA 1
		bool spotlight_1 = false;
		float spotlight1_angle = 60.0f;
		float spotlight1_position[] = {0.0f, 0.0f, 0.0f};
		float spotlight1_ambient[] = {20.0f, 20.0f, 20.0f};
		float spotlight1_diffuse[] = {70.0f, 70.0f, 70.0f};
		float spotlight1_specular[] = {80.0f, 80.0f, 80.0f};
		bool spotlight1_shadows = false;
		//LUZ DIRIGIDA 2
		bool spotlight_2 = false;
		float spotlight2_angle = 60.0f;
		float spotlight2_position[] = {0.0f, 0.0f, 0.0f};
		float spotlight2_ambient[] = {20.0f, 20.0f, 20.0f};
		float spotlight2_diffuse[] = {70.0f, 70.0f, 70.0f};
		float spotlight2_specular[] = {80.0f, 80.0f, 80.0f};
		bool spotlight2_shadows = false;
		
// **** Shaders ****
		const char* listbox_shaders[] = { "Material Shader", "Texture Shader", "Cartoon Shader", "Lighting Map Shader", "Deferred Shader"};
		static int listbox_shader_current = 1;
	
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
	camara = TMotor::instancia().newCameraNode("Camara", "escena_raiz");
	camara->setPosition(0.0,4.0,25.0f);

	//Creacion de objetos
	objetos.push_back(TMotor::instancia().newMeshNode("Hacha","assets/hacha/hacha.obj", "escena_raiz", false)); 
	objetos.push_back(TMotor::instancia().newMeshNode("Caja","assets/wall/wall.obj", "escena_raiz", false));
	objetos.push_back(TMotor::instancia().newMeshNode("Pirata","assets/Pirata/p2.obj", "escena_raiz", false));
	objetos.push_back(TMotor::instancia().newMeshNode("Coche","assets/Karts/Pirata/kart_pirata.obj", "escena_raiz", false));

	suelo = TMotor::instancia().newMeshNode("Suelo","assets/suelo/wall.obj", "escena_raiz", false);
	suelo->setScale(15.0, 1.0, 15.0);
	pared = TMotor::instancia().newMeshNode("Pared","assets/suelo/wall.obj", "escena_raiz", false);
	pared->setScale(8.0, 0.5, 8.0);

	//Creacion de luces
	lucesPuntuales.push_back(TMotor::instancia().newLightNode("light_0", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz"));
	lucesPuntuales.push_back(TMotor::instancia().newLightNode("light_1", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.001f, glm::cos(glm::radians(60.0f)), true, false, "escena_raiz"));
	luzSolar = TMotor::instancia().newLightNode("light_2", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), true, false, "escena_raiz");

	lucesDirigidas.push_back(TMotor::instancia().newLightNode("light_0", glm::vec4(0.0f, 0.f, -1.0f, 1.0f), 0.0001f, glm::cos(glm::radians(60.0f)), true, false, "escena_raiz"));
	lucesDirigidas.push_back(TMotor::instancia().newLightNode("light_1", glm::vec4(0.0f, 0.f, -1.0f, 1.0f), 0.0001f, glm::cos(glm::radians(60.0f)), true, false, "escena_raiz"));

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

		//Generamos los mapas de sombras
		TMotor::instancia().drawMappingShadows();

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
		//Dibujamos las sombras proyectadas
		TMotor::instancia().drawProjectedShadows();
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

	//Traslacion de la camara
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS)
        camara->translate(glm::vec3(0.0, 0.5, 0.0));
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS)
        camara->translate(glm::vec3(0.0, -0.5, 0.0));
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        camara->translate(glm::vec3(0.5, 0.0, 0.0));
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS)
        camara->translate(glm::vec3(-0.5, 0.0, 0.0));

	//Rotacion de la camara
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W) == GLFW_PRESS)
        camara->rotate(glm::vec3(1.0, 0.0, 0.0), 0.5);
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_S) == GLFW_PRESS)
        camara->rotate(glm::vec3(1.0, 0.0, 0.0), -0.5);
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_D) == GLFW_PRESS)
        camara->rotate(glm::vec3(0.0, 1.0, 0.0), 0.5);
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_A) == GLFW_PRESS)
        camara->rotate(glm::vec3(0.0, 1.0, 0.0), -0.5);

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
	ImGui::Text("Direction keys for camera movement.");
	ImGui::Text("W-A-S-D for camera rotation.");

	//**** Creacion de Objetos ****
	if (ImGui::CollapsingHeader("Object Creation")) {
		//SUELO
		ImGui::Checkbox("Floor", &onTheFloor);
		if (onTheFloor == true){
			//Posicion
			ImGui::SliderFloat3("Position Floor", floor_position, -100, 100, "%.0f", 1.0f);
		}

		//SUELO
		ImGui::Checkbox("Wall", &wall);
		if (wall == true){
			//Posicion
			ImGui::SliderFloat3("Position Wall", wall_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation Wall", wall_rotation, 0, 360, "%.0f", 1.0f);
		}

		ImGui::Separator();

		//HACHA
		ImGui::Checkbox("Axe", &axe);
		if (axe == true){
			//Posicion
			ImGui::SliderFloat3("Position Axe", axe_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation Axe", axe_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale Axe", axe_scale, 1, 200, "%.0f", 1.0f);
		}

		//CAJA
		ImGui::Checkbox("Box", &box);
		if (box == true){
			//Posicion
			ImGui::SliderFloat3("Position Box", box_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation Box", box_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale Box", box_scale,  1, 200, "%.0f", 1.0f);
		}

		//PIRATA
		ImGui::Checkbox("Pirate", &pirate);
		if (pirate == true){
			//Posicion
			ImGui::SliderFloat3("Position Pirate", pirate_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation Pirate", pirate_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale Pirate", pirate_scale,  1, 200, "%.0f", 1.0f);
		}

		//COCHE
		ImGui::Checkbox("Car", &car);
		if (car == true){
			//Posicion
			ImGui::SliderFloat3("Position Car", car_position, -100, 100, "%.0f", 1.0f);
			//Rotacion
			ImGui::SliderFloat3("Rotation Car", car_rotation, 0, 360, "%.0f", 1.0f);
			//Escalado
			ImGui::SliderFloat3("Scale Car", car_scale,  1, 200, "%.0f", 1.0f);
		}

	}

	// **** Luces ****
	if (ImGui::CollapsingHeader("Lights")) {   
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		//LUCES PUNTUALES
		if (ImGui::CollapsingHeader("Point Lights")) {
			//LUZ PUNTUAL 1
			ImGui::Checkbox("Point Light 1", &pointLight_1);
			if (pointLight_1){
				sunLight = false;
				ImGui::SliderFloat3("Position PointLight 1", pointLight1_position, -100, 100, "%.0f", 1.0f);
				if (listbox_shader_current == 0 || listbox_shader_current == 1 || listbox_shader_current == 3)
				ImGui::Checkbox("Shadows Point Light 1", &pointLight1_shadows);
				ImGui::Text("Light Components");
				ImGui::SliderFloat3("Ambient PointLight 1", pointLight1_ambient, 0, 100, "%.0f", 1.0f);
				ImGui::SliderFloat3("Diffuse PointLight 1", pointLight1_diffuse, 0, 100, "%.0f", 1.0f);
				ImGui::SliderFloat3("Specular PointLight 1", pointLight1_specular, 0, 100, "%.0f", 1.0f);
			}
			//LUZ PUNTUAL 2
			ImGui::Checkbox("Point Light 2", &pointLight_2);
			if (pointLight_2){
				sunLight = false;
				ImGui::SliderFloat3("Position PointLight 2", pointLight2_position, -100, 100, "%.0f", 1.0f);
				if (listbox_shader_current == 0 || listbox_shader_current == 1 || listbox_shader_current == 3)
				ImGui::Checkbox("Shadows Point Light 2", &pointLight2_shadows);
				ImGui::Text("Light Components");
				ImGui::SliderFloat3("Ambient PointLight 2", pointLight2_ambient, 0, 100, "%.0f", 1.0f);
				ImGui::SliderFloat3("Diffuse PointLight 2", pointLight2_diffuse, 0, 100, "%.0f", 1.0f);
				ImGui::SliderFloat3("Specular PointLight 2", pointLight2_specular, 0, 100, "%.0f", 1.0f);
			}
			//LUZ SOLAR
			ImGui::Checkbox("Sun Light", &sunLight);
			if (sunLight == true){
				pointLight_1 = false;
				pointLight_2 = false;
				ImGui::SliderFloat3("Position SunLight", sunLight_position, -100, 100, "%.0f", 1.0f);
				ImGui::Text("Light Components");
				ImGui::SliderFloat3("Ambient SunLight", sunLight_ambient, 0, 100, "%.0f", 1.0f);
				ImGui::SliderFloat3("Diffuse SunLight", sunLight_diffuse, 0, 100, "%.0f", 1.0f);
				ImGui::SliderFloat3("Specular SunLight", sunLight_specular, 0, 100, "%.0f", 1.0f);
				ImGui::Checkbox("Shadows (Projected Shadows)", &projectedShadows);
			}
		}
		//LUCES DIRIGIDAS
		if (listbox_shader_current != 4){
			if (ImGui::CollapsingHeader("Directional Lights/Spotlights")) {
				//LUZ DIRIGIDA 1
				ImGui::Checkbox("Spotlight 1", &spotlight_1);
				if (spotlight_1){
					ImGui::SliderFloat3("Position Spotlight 1", spotlight1_position, -100, 100, "%.0f", 1.0f);
					ImGui::SliderFloat("Angle of Radius Spotlight 1", &spotlight1_angle, 0, 90, "%.0f", 1.0f);
					if (listbox_shader_current == 0 || listbox_shader_current == 1 || listbox_shader_current == 3)
					ImGui::Checkbox("Shadows Spotlight 1", &spotlight1_shadows);
					ImGui::Text("Direction Spotlight 1");
					if (ImGui::Button("Axis X Positive"))
					static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setDirection(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::SameLine();
					if (ImGui::Button("Axis X Negative"))
					static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setDirection(glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::Button("Axis Y Positive"))
					static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::SameLine();
					if (ImGui::Button("Axis Y Negative"))
					static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
					if (ImGui::Button("Axis Z Positive"))
					static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					ImGui::SameLine();
					if (ImGui::Button("Axis Z Negative"))
					static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
					ImGui::Text("Light Components");
					ImGui::SliderFloat3("Ambient Spotlight 1", spotlight1_ambient, 0, 100, "%.0f", 1.0f);
					ImGui::SliderFloat3("Diffuse Spotlight 1", spotlight1_diffuse, 0, 100, "%.0f", 1.0f);
					ImGui::SliderFloat3("Specular Spotlight 1", spotlight1_specular, 0, 100, "%.0f", 1.0f);
				}
				//LUZ DIRIGIDA 2
				ImGui::Checkbox("Spotlight 2", &spotlight_2);
				if (spotlight_2){
					ImGui::SliderFloat3("Position Spotlight 2", spotlight2_position, -100, 100, "%.0f", 1.0f);
					ImGui::SliderFloat("Angle of Radius Spotlight 2", &spotlight2_angle, 0, 90, "%.0f", 1.0f);
					if (listbox_shader_current == 0 || listbox_shader_current == 1 || listbox_shader_current == 3)
					ImGui::Checkbox("Shadows Spotlight 2", &spotlight2_shadows);
					ImGui::Text("Direction Spotlight 2");
					if (ImGui::Button("Axis X Positive (2)"))
					static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setDirection(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::SameLine();
					if (ImGui::Button("Axis X Negative (2)"))
					static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setDirection(glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::Button("Axis Y Positive (2)"))
					static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::SameLine();
					if (ImGui::Button("Axis Y Negative (2)"))
					static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
					if (ImGui::Button("Axis Z Positive (2)"))
					static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					ImGui::SameLine();
					if (ImGui::Button("Axis Z Negative (2)"))
					static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setDirection(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
					ImGui::Text("Light Components");
					ImGui::SliderFloat3("Ambient Spotlight 2", spotlight2_ambient, 0, 100, "%.0f", 1.0f);
					ImGui::SliderFloat3("Diffuse Spotlight 2", spotlight2_diffuse, 0, 100, "%.0f", 1.0f);
					ImGui::SliderFloat3("Specular Spotlight 2", spotlight2_specular, 0, 100, "%.0f", 1.0f);
				}
			}
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}

	// **** Shaders ****
	if (ImGui::CollapsingHeader("Shaders")) {
		ImGui::ListBox("", &listbox_shader_current, listbox_shaders, IM_ARRAYSIZE(listbox_shaders), 5);
	}

	// **** Otras Funciones ****
	if (ImGui::CollapsingHeader("Another Functions")) {
		ImGui::Checkbox("SkyBox", &sky);
		ImGui::Checkbox("Bounding Boxes", &bbox);
		ImGui::Checkbox("Billboards", &billboard);
		ImGui::Checkbox("Particles", &particles);
		if (particles == true){
			ImGui::SliderFloat3("Position Particles", particles_position, -100, 100, "%.0f", 1.0f);
			ImGui::SliderFloat3("Color Particles", particles_color, 0, 100, "%.0f", 1.0f);
			ImGui::SliderFloat("Size Particles", &particles_size, 0, 100, "%.0f", 1.0f);
		}
		ImGui::Checkbox("Clouds", &clouds);
		ImGui::Checkbox("HUD Example", &hud);
		if (hud == true){ 
			ImGui::SliderFloat2("Position HUD", hud_position, -100, 100, "%.0f", 1.0f);
			ImGui::SliderFloat("Rotation HUD", &hud_rotation, 0, 360, "%.0f", 1.0f);
			ImGui::SliderFloat2("Scale HUD", hud_scale, -10, 10, "%.0f", 1.0f);
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
	//SUELO
	suelo->setVisible(onTheFloor);
	suelo->setPosition(floor_position[0], floor_position[1], floor_position[2]);
	//PARED
	pared->setVisible(wall);
	pared->setPosition(wall_position[0], wall_position[1], wall_position[2]);
	pared->setRotation(wall_rotation[0], wall_rotation[1], wall_rotation[2]);
	//HACHA
	findObject("Hacha")->setVisible(axe);
	findObject("Hacha")->setPosition(axe_position[0], axe_position[1], axe_position[2]);
	findObject("Hacha")->setRotation(axe_rotation[0], axe_rotation[1], axe_rotation[2]);
	findObject("Hacha")->setScale(axe_scale[0]/100.0f, axe_scale[1]/100.0f, axe_scale[2]/100.0f);
	//CAJA
	findObject("Caja")->setVisible(box);
	findObject("Caja")->setPosition(box_position[0], box_position[1], box_position[2]);
	findObject("Caja")->setRotation(box_rotation[0], box_rotation[1], box_rotation[2]);
	findObject("Caja")->setScale(box_scale[0]/100.0f, box_scale[1]/100.0f, box_scale[2]/100.0f);
	//PIRATA
	findObject("Pirata")->setVisible(pirate);
	findObject("Pirata")->setPosition(pirate_position[0], pirate_position[1], pirate_position[2]);
	findObject("Pirata")->setRotation(pirate_rotation[0], pirate_rotation[1], pirate_rotation[2]);
	findObject("Pirata")->setScale(pirate_scale[0]/100.0f, pirate_scale[1]/100.0f, pirate_scale[2]/100.0f);
	//COCHE
	findObject("Coche")->setVisible(car);
	findObject("Coche")->setPosition(car_position[0], car_position[1], car_position[2]);
	findObject("Coche")->setRotation(car_rotation[0], car_rotation[1], car_rotation[2]);
	findObject("Coche")->setScale(car_scale[0]/100.0f, car_scale[1]/100.0f, car_scale[2]/100.0f);

	//**** Luces ****
	//LUCES PUNTUALES
		//LUZ PUNTUAL 1
		static_cast<TLuz *>(findPointLight("light_0")->getNode()->getEntidad())->setActive(pointLight_1);
		findPointLight("light_0")->setPosition(pointLight1_position[0], pointLight1_position[1], pointLight1_position[2]);
		static_cast<TLuz *>(findPointLight("light_0")->getNode()->getEntidad())->setAmbient(glm::vec3(pointLight1_ambient[0]/100.0f, pointLight1_ambient[1]/100.0f, pointLight1_ambient[2]/100.0f));
		static_cast<TLuz *>(findPointLight("light_0")->getNode()->getEntidad())->setDiffuse(glm::vec3(pointLight1_diffuse[0]/100.0f, pointLight1_diffuse[1]/100.0f, pointLight1_diffuse[2]/100.0f));
		static_cast<TLuz *>(findPointLight("light_0")->getNode()->getEntidad())->setSpecular(glm::vec3(pointLight1_specular[0]/100.0f, pointLight1_specular[1]/100.0f, pointLight1_specular[2]/100.0f));
		static_cast<TLuz *>(findPointLight("light_0")->getNode()->getEntidad())->setShadow(pointLight1_shadows);
		//LUZ PUNTUAL 2
		static_cast<TLuz *>(findPointLight("light_1")->getNode()->getEntidad())->setActive(pointLight_2);
		findPointLight("light_1")->setPosition(pointLight2_position[0], pointLight2_position[1], pointLight2_position[2]);
		static_cast<TLuz *>(findPointLight("light_1")->getNode()->getEntidad())->setAmbient(glm::vec3(pointLight2_ambient[0]/100.0f, pointLight2_ambient[1]/100.0f, pointLight2_ambient[2]/100.0f));
		static_cast<TLuz *>(findPointLight("light_1")->getNode()->getEntidad())->setDiffuse(glm::vec3(pointLight2_diffuse[0]/100.0f, pointLight2_diffuse[1]/100.0f, pointLight2_diffuse[2]/100.0f));
		static_cast<TLuz *>(findPointLight("light_1")->getNode()->getEntidad())->setSpecular(glm::vec3(pointLight2_specular[0]/100.0f, pointLight2_specular[1]/100.0f, pointLight2_specular[2]/100.0f));
		static_cast<TLuz *>(findPointLight("light_1")->getNode()->getEntidad())->setShadow(pointLight2_shadows);
		//LUZ SOLAR
		static_cast<TLuz *>(luzSolar->getNode()->getEntidad())->setActive(sunLight);
		luzSolar->setPosition(sunLight_position[0], sunLight_position[1], sunLight_position[2]);
		static_cast<TLuz *>(luzSolar->getNode()->getEntidad())->setAmbient(glm::vec3(sunLight_ambient[0]/100.0f, sunLight_ambient[1]/100.0f, sunLight_ambient[2]/100.0f));
		static_cast<TLuz *>(luzSolar->getNode()->getEntidad())->setDiffuse(glm::vec3(sunLight_diffuse[0]/100.0f, sunLight_diffuse[1]/100.0f, sunLight_diffuse[2]/100.0f));
		static_cast<TLuz *>(luzSolar->getNode()->getEntidad())->setSpecular(glm::vec3(sunLight_specular[0]/100.0f, sunLight_specular[1]/100.0f, sunLight_specular[2]/100.0f));	
		TMotor::instancia().setShadows(projectedShadows);
	//LUCES DIRIGIDAS
		//LUZ DIRIGIDA 1
		static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setActive(spotlight_1);
		static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setAngle(spotlight1_angle);
		findSpotLight("light_0")->setPosition(spotlight1_position[0], spotlight1_position[1], spotlight1_position[2]);
		static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setAmbient(glm::vec3(spotlight1_ambient[0]/100.0f, spotlight1_ambient[1]/100.0f, spotlight1_ambient[2]/100.0f));
		static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setDiffuse(glm::vec3(spotlight1_diffuse[0]/100.0f, spotlight1_diffuse[1]/100.0f, spotlight1_diffuse[2]/100.0f));
		static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setSpecular(glm::vec3(spotlight1_specular[0]/100.0f, spotlight1_specular[1]/100.0f, spotlight1_specular[2]/100.0f));
		static_cast<TLuz *>(findSpotLight("light_0")->getNode()->getEntidad())->setShadow(spotlight1_shadows);
		//LUZ DIRIGIDA 2
		static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setActive(spotlight_2);
		static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setAngle(spotlight2_angle);
		findSpotLight("light_1")->setPosition(spotlight2_position[0], spotlight2_position[1], spotlight2_position[2]);
		static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setAmbient(glm::vec3(spotlight2_ambient[0]/100.0f, spotlight2_ambient[1]/100.0f, spotlight2_ambient[2]/100.0f));
		static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setDiffuse(glm::vec3(spotlight2_diffuse[0]/100.0f, spotlight2_diffuse[1]/100.0f, spotlight2_diffuse[2]/100.0f));
		static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setSpecular(glm::vec3(spotlight2_specular[0]/100.0f, spotlight2_specular[1]/100.0f, spotlight2_specular[2]/100.0f));
		static_cast<TLuz *>(findSpotLight("light_1")->getNode()->getEntidad())->setShadow(spotlight2_shadows);

	//**** Shaders ****
	if (listbox_shader_current == 0){
		TMotor::instancia().setShaderActive("shaderMaterial");
	}else if (listbox_shader_current == 1){
		TMotor::instancia().setShaderActive("shaderTexture");
	}else if (listbox_shader_current == 2){
		TMotor::instancia().setShaderActive("shaderCartoon");
	}else if (listbox_shader_current == 3){
		TMotor::instancia().setShaderActive("shaderLighting");
	}else{
		TMotor::instancia().setShaderActive("shaderDeferred");
	}

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

//Funcion que devuelve una luz en funcion de su nombre
obj3D *findPointLight(const char* n){
	obj3D * l = NULL;
	//Recorremos el array de luces
	for(int i = 0; i < lucesPuntuales.size(); i++){
		//Si coincide el nombre de la luz, lo guardamos
		if (strcmp(lucesPuntuales.at(i)->getName(), n) == 0){
			l = lucesPuntuales.at(i);
		}
	}
	//Devolvemos la luz
	return l;
}

//Funcion que devuelve una luz en funcion de su nombre
obj3D *findSpotLight(const char* n){
	obj3D * l = NULL;
	//Recorremos el array de luces
	for(int i = 0; i < lucesDirigidas.size(); i++){
		//Si coincide el nombre de la luz, lo guardamos
		if (strcmp(lucesDirigidas.at(i)->getName(), n) == 0){
			l = lucesDirigidas.at(i);
		}
	}
	//Devolvemos la luz
	return l;
}