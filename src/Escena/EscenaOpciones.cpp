#include "EscenaOpciones.hpp"

EscenaOpciones::EscenaOpciones(Escena::tipo_escena tipo) : Escena(tipo) {
	end = false;
	TMotor::instancia().newHud("LobbyHUD");
	//TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "comenzarPartida", "assets/HUD/LobbyMenu/empezar.png");
	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf", 30.0f);
	ImGui::StyleColorsLight();
	fuenteMusica = new AlSource();
	fuenteMusica->volume(TMotor::instancia().getGestorSonido()->getVolMusica());
	fuenteMusica->setLoop(true);
	fuenteMusica->play(SOUND_MENU);
	fuenteEfectos = new AlSource();
	fuenteEfectos->volume(TMotor::instancia().getGestorSonido()->getVolEfectos());


}

EscenaOpciones::~EscenaOpciones() {
	delete fuenteMusica;
	delete fuenteEfectos;
}

void EscenaOpciones::init() {}
void EscenaOpciones::dibujar() {
	//Limpiamos el dibujado anterior asignando un color de fondo
	TMotor::instancia().clean(0.16f, 0.533f, 0.698f, 0.0f);
	//Establecemos la zona de renderizado
	TMotor::instancia().setViewport(0, 0, TMotor::instancia().getWidth(), TMotor::instancia().getHeight()); //Pantalla completa
	//Dibujamos el menu 
	TMotor::instancia().drawHudMenus();
	//Dibujamos IMGUI
	TMotor::instancia().drawIMGUI();
}

void EscenaOpciones::limpiar() {
	std::cout << "Limpio escena opciones\n";
	TMotor::instancia().closeDebugWindow();
}

void EscenaOpciones::update() {
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Mostrar ventanas
	int display_w = 0, display_h = 0;
	glfwGetFramebufferSize(TMotor::instancia().getVentana(), &display_w, &display_h);

	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::GetStyle().WindowBorderSize = 0;
	static float volGen, volEfe, volMus;
	TMotor::instancia().getGestorSonido()->getParametros(&volGen, &volMus, &volEfe);

	if (ImGui::Begin("Opciones de sonido", NULL, ImGuiWindowFlags_NoResize
		| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard
		| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal)) {

		ImGui::SameLine(TMotor::instancia().getWidth()/2.75);
		ImGui::SetWindowFontScale(2.0f);
		if (display_w < 1250) {
			ImGui::SetWindowFontScale(1.0f);
		}
		ImGui::Text("OPTIONS");
		ImGui::Text("");
		ImGui::SameLine(TMotor::instancia().getWidth()/2.75);
		ImGui::Text("-------");

		ImGui::SetWindowFontScale(1.25f);
		if (display_w < 1250) {
			ImGui::SetWindowFontScale(0.75f);
		}
		ImGui::Text("\n* Sound *\n");
		ImGui::SliderFloat("General", &volGen, 0.0f, 1.0f);
		ImGui::SliderFloat("Music", &volMus, 0.0f, 1.0f);
		ImGui::SliderFloat("FX", &volEfe, 0.0f, 1.0f);
		ImGui::SetWindowFontScale(1.00f);
		if (display_w < 1250) {
			ImGui::SetWindowFontScale(0.5f);
		}
		ImGui::Text("Press W to play sound\n");

		TMotor::instancia().getGestorSonido()->ajustarVolumen(volGen, volMus, volEfe);

		ImGui::SetWindowFontScale(1.25f);
		if (display_w < 1250) {
			ImGui::SetWindowFontScale(0.75f);
		}
		ImGui::Text("\n* Graphics *\n");
		ImGui::SetWindowFontScale(1.00f);
		if (display_w < 1250) {
			ImGui::SetWindowFontScale(0.5f);
		}
		const char* listbox_items[] = { "Low", "Medium", "High"};
		static int listbox_item_current = 0;
		static int listbox_item_current_2 = 0;
		if (personalisedGraphics == false){
			ImGui::ListBox("Graphic Level", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 3);
		}
		ImGui::Checkbox("Personalised Graphics", &personalisedGraphics);
		if (personalisedGraphics == true){
			ImGui::Text("");
			ImGui::Checkbox("Shadows", &shadows);
			ImGui::SameLine();
			ImGui::Checkbox("Anti-Aliasing", &msaa);
			ImGui::SameLine();
			ImGui::Checkbox("Face Culling", &faceCulling);
			ImGui::SameLine();
			ImGui::Checkbox("Level Of Detail", &levelOfDetail);
			ImGui::Text("Drawing Distance", &drawingDistance);
			ImGui::ListBox("", &listbox_item_current_2, listbox_items, IM_ARRAYSIZE(listbox_items), 3);
		}

		//Ajustamos los parametros graficos del motor en funcion del nivel grafico seleccionado
		if (personalisedGraphics == false){
			if (listbox_item_current == 0){
				shadows = false; //No hay sombras
				msaa = false; //Sin MSAA
				faceCulling = true; //Con Face Culling
				levelOfDetail = true; //Level Of Detail activado
				drawingDistance = true; //Con distancia de dibujado
				levelOfDrawingDistance = 750; //Nivel de distancia de dibujado = 750
			} else if (listbox_item_current == 1){
				shadows = true; //Hay sombras
				msaa = false; //Sin MSAA
				faceCulling = true; //Con Face Culling
				levelOfDetail = true; //Level Of Detail activado
				drawingDistance = true; //Con distancia de dibujado
				levelOfDrawingDistance = 1000; //Nivel de distancia de dibujado = 1000
			} else {
				shadows = true; //No hay sombras
				msaa = true; //Con MSAA
				faceCulling = false; //Sin Face Culling
				levelOfDetail = false; //Level Of Detail desactivado
				drawingDistance = false; //Sin distancia de dibujado
				levelOfDrawingDistance = 0; //Nivel de distancia de dibujado = ninguno
			}
		}else{
			if (listbox_item_current_2 == 0){
				drawingDistance = true; //Con distancia de dibujado
				levelOfDrawingDistance = 750; //Nivel de distancia de dibujado = 750
			} else if (listbox_item_current_2 == 1){
				drawingDistance = true; //Con distancia de dibujado
				levelOfDrawingDistance = 750; //Nivel de distancia de dibujado = 750
			} else {
				drawingDistance = false; //Sin distancia de dibujado
				levelOfDrawingDistance = 0; //Nivel de distancia de dibujado = ninguno
			}
		}
		
		//Modificamos los parametros graficos del motor
		TMotor::instancia().setShadows(shadows);
		TMotor::instancia().setAntialiasing(msaa);
		TMotor::instancia().setFaceCulling(faceCulling);
		TMotor::instancia().setDrawingDistance(drawingDistance, levelOfDrawingDistance);
		TMotor::instancia().setLevelOfDetail(levelOfDetail);
	}

	ImGui::End();

	fuenteMusica->volume(TMotor::instancia().getGestorSonido()->getVolMusica());
	fuenteEfectos->volume(30.0f * TMotor::instancia().getGestorSonido()->getVolEfectos());
}

Escena::tipo_escena EscenaOpciones::comprobarInputs() {
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && end) {
		return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		end = true;
	}
	else end = false;

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W) == GLFW_PRESS && !fuenteEfectos->isPlaying())
		fuenteEfectos->play(SOUND_OPCION);
	return Escena::tipo_escena::OPCIONES;
}