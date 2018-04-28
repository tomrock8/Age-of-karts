#include "EscenaOpciones.hpp"

EscenaOpciones::EscenaOpciones(Escena::tipo_escena tipo) : Escena(tipo) {
	end = false;
	TMotor::instancia().newHud("LobbyHUD");
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "comenzarPartida", "assets/HUD/LobbyMenu/empezar.png");
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
void EscenaOpciones::dibujar() {}

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
		ImGui::Text("Opciones");

		ImGui::Text("\nSonido\n");
		ImGui::SliderFloat("General", &volGen, 0.0f, 1.0f);
		ImGui::SliderFloat("Musica", &volMus, 0.0f, 1.0f);
		ImGui::SliderFloat("Efectos", &volEfe, 0.0f, 1.0f);

		TMotor::instancia().getGestorSonido()->ajustarVolumen(volGen, volMus, volEfe);

		ImGui::Text("\nGraficos\n");
		static bool detalleMinimo = false;
		static bool sombreado = false;
		ImGui::Checkbox("Detalle Minimo", &detalleMinimo);
		ImGui::Checkbox("Sombras", &sombreado);
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

	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W) == GLFW_PRESS && !fuenteEfectos->isPlaying()) 
		fuenteEfectos->play(SOUND_OPCION);
	return Escena::tipo_escena::OPCIONES;
}