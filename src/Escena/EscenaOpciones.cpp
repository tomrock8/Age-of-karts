#include "EscenaOpciones.hpp"

EscenaOpciones::EscenaOpciones(Escena::tipo_escena tipo) : Escena(tipo) {
    end = false;
    TMotor::instancia().newHud("LobbyHUD");
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "comenzarPartida", "assets/HUD/LobbyMenu/empezar.png");
	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",30.0f);
	ImGui::StyleColorsLight(); 
	fuenteMusica = new AlSource();
	fuenteMusica->volume(TMotor::instancia().getGestorSonido()->getVolMusica());
	fuenteMusica->play(SOUND_MENU);
	
}

EscenaOpciones::~EscenaOpciones() {
	delete fuenteMusica;
}

void EscenaOpciones::init() {}

void EscenaOpciones::dibujar()
{
}

void EscenaOpciones::limpiar()
{
	std::cout << "Limpio escena opciones\n";
	TMotor::instancia().closeDebugWindow();
}

void EscenaOpciones::update()
{
bool  show_demo_window=true;
  	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  	// Mostrar ventanas
   	int display_w = 0 , display_h = 0;
	static int str0 = 192;
	static int vec4a[4] = { 192, 168, 1, 1 };
 
  
  	glfwGetFramebufferSize( TMotor::instancia().getVentana() , &display_w , &display_h );
	
  	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize( ImVec2( (float)display_w , (float)display_h ) );
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::GetStyle().WindowBorderSize=0;
	static float volGen = TMotor::instancia().getGestorSonido()->getVolGeneral();
	static float volEfe = TMotor::instancia().getGestorSonido()->getVolMusica();
	static float volMus = TMotor::instancia().getGestorSonido()->getVolEfectos();

	if (ImGui::Begin("Hola", NULL, ImGuiWindowFlags_NoResize 
	| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
	| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
	| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal)){
		ImGui::Text("OPCIONES");
		ImGui::SliderFloat("General", &volGen, 0.0f, 1.0f);
		ImGui::SliderFloat("Musica", &volMus, 0.0f, 1.0f);
		ImGui::SliderFloat("Efectos", &volEfe, 0.0f, 1.0f);
		
	}

	TMotor::instancia().getGestorSonido()->setVolEfectos(volEfe);
	TMotor::instancia().getGestorSonido()->setVolMusica(volMus);
	TMotor::instancia().getGestorSonido()->setVolGeneral(volGen);
	fuenteMusica->volume(TMotor::instancia().getGestorSonido()->getVolMusica());
	//ImGui::Text(infoLobby->c_str());
  	ImGui::End();
}

Escena::tipo_escena EscenaOpciones::comprobarInputs(){
    if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && end) {

		return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
	} else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS){
        end = true;
    } else end = false;
    return Escena::tipo_escena::OPCIONES;
}