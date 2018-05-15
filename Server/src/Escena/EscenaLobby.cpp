#include "EscenaLobby.hpp"
#include "Pista.hpp"

EscenaLobby::EscenaLobby(Escena::tipo_escena tipo, std::string ipC) : Escena(tipo) {
	infoLobby= new std::string("");
	system("ip addr show $dev | awk '/inet/ {print $2}' | cut -d/ -f1 > ip.txt");
	std::string line;
	std::ifstream myfile;
	myfile.open("./ip.txt");
	int cont_line = 0;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			cont_line++;
			if (cont_line == 3) {
				iphost = line;
				break;
			}
		}
		myfile.close();
	}
	else {
		iphost = "IP no encontrada\n";
	}

	TMotor::instancia().newHud("LobbyHUD");
	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",30.0f);
	ImGui::StyleColorsLight(); 


	
	Pista::getInstancia()->setNombreMapa("pirata");
	
}

EscenaLobby::~EscenaLobby() {
	cout << "\nDestructor ESCENA LOBBY. Entro.";
	limpiar();
	cout << "Salgo.\n";
}

void EscenaLobby::init() {}
void EscenaLobby::dibujar() {
	
	  //Limpiamos el dibujado anterior asignando un color de fondo
 
  TMotor::instancia().clean(0.16f, 0.533f, 0.698f, 0.0f);
 
  //Establecemos la zona de renderizado
 
  TMotor::instancia().setViewport(0, 0, TMotor::instancia().getWidth(), TMotor::instancia().getHeight()); //Pantalla completa
 
  //Dibujamos el menu 
 
  TMotor::instancia().drawHudMenus();
 
  //Dibujamos IMGUI
 
  TMotor::instancia().drawIMGUI();
}
void EscenaLobby::limpiar() {
	//TMotor::instancia().closeDebugWindow();
}

void EscenaLobby::update() {
	
	Server::getInstancia()->ReceivePackets();

	infoLobby->clear();
	infoLobby->append("\n\nIP conexion: ");
	infoLobby->append(iphost);
	infoLobby->append(Server::getInstancia()->getStringClients());
	// ------------------------------
  	// -------- IMGUI ---------------
  	// ------------------------------
	glfwPollEvents();

	bool  show_demo_window=true;
  	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  	// Mostrar ventanas
   	int display_w = 0 , display_h = 0;
 
	
  	glfwGetFramebufferSize( TMotor::instancia().getVentana() , &display_w , &display_h );

  	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::SetNextWindowPos(ImVec2(100, 100));
	ImGui::SetNextWindowSize( ImVec2( (float)display_w , (float)display_h ) );
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::GetStyle().WindowBorderSize=0;
	
	if (ImGui::Begin("Hola", NULL,  ImGuiWindowFlags_NoResize 
	| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
	| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
	| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal)){

		if (display_w<1000){
			ImGui::SetWindowFontScale(0.5f);
		}else{
			ImGui::SetWindowFontScale(1.0f);
		}
		ImGui::Text("Server iniciado!");  

		ImGui::Text(infoLobby->c_str());
	}
  	ImGui::End();
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		return Escena::tipo_escena::SALIR;
	}

	if(Server::getInstancia()->getStarted())
		return Escena::tipo_escena::ONLINE;

	return Escena::tipo_escena::LOBBY;
}

std::string EscenaLobby::getIpConexion() { return ipConexion; }
