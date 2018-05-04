#include "EscenaLobby.hpp"

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
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "comenzarPartida", "assets/HUD/LobbyMenu/empezar.png");
	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",30.0f);
	ImGui::StyleColorsLight(); 
	
}

EscenaLobby::~EscenaLobby() {
	cout << "\nDestructor ESCENA LOBBY. Entro.";
	limpiar();
	cout << "Salgo.\n";
}

void EscenaLobby::init() {}
void EscenaLobby::dibujar() {}
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
	static int str0 = 192;
	static int vec4a[4] = { 192, 168, 1, 1 };
 
	
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

void EscenaLobby::mostrarInfoLobby(int indice) {
	//Inicializacion variables cliente
	/*
	std::vector<structClientes> clientes = client->getClientes();
	int size = clientes.size();
	int id_player;
	if (offline) {
		id_player = indice;
	}
	else {
		id_player = client->getControlPlayer();
	}
	bool checkReady = true;		//boolean para comprobar si todos estan listos
	bool checkReadyMe = true;		//Para online. Boolean para comprobar si tu estas listo (teniendo en cuenta si estas solo o hay mas gente)
	bool bc = false;				//boolean aux

	//Diferenciamos informacion en funcion de online/offline
	if (offline) {
		if (id_player == 0) {
			infoLobby->append("\nNumero de jugadores: ");

			infoLobby->append(to_string(clientes.size()));
			infoLobby->append(" / 6");
		}
		//indicador de jugador actual seleccionado (a modificar)
		if (count != id_player) {
			if (clientes.at(id_player).corredorJugador) {
				infoLobby->append("\nCorredor Jugador ");
			}
			else {
				infoLobby->append("\nCorredor IA ");
			}
		}
		else {
			if (clientes.at(id_player).corredorJugador) {
				infoLobby->append("\n**Corredor Jugador ");
			}
			else {
				infoLobby->append("\n**Corredor IA ");
			}
		}
	}
	else {
		infoLobby->append("\nJugadores conectados: ");
		infoLobby->append(to_string(client->getNumClients()));
		infoLobby->append(" / ");
		infoLobby->append(to_string(client->getMaxPlayers()));
		infoLobby->append("\nEres el Jugador ");
	}
	infoLobby->append(to_string(id_player));

	if (id_player == 0 && !offline) {	//info solo para online
		infoLobby->append(" (Host)");
	}

	if (id_player < clientes.size()) {		//comprobamos que la id del jugador no supere el tamnyo del vector de ready
		if (clientes.at(id_player).ready == false && clientes.size() > 1) {		//si estas no ready y hay mas jugadores 
			for (int c = 0; c < clientes.size(); c++) {
				if (clientes.at(c).ready == 0) {	//comprobamos si han aceptado todos
					bc = true;
					break;
				}
			}
			if (bc == false) {		//si todos los  clientes han aceptado
				infoLobby->append(" [Listo] ");
			}
			else {		//si no han aceptado todos
				checkReady = false;
				checkReadyMe = false;
				infoLobby->append(" [No listo] ");
			}

		}
		else {		//si estas solo tu 
			infoLobby->append(" [Listo] ");
		}
	}
	infoLobby->append("\n <-- Selecciona Personaje -->: ");

	if (id_player < size && id_player != -1)
		mostrarTipoPersonaje(id_player);

	infoLobby->append("\n");

	//recorremos todos los demas jugadores que estan en la lobby
	for (int i = 0; i < size; i++) {
		if (i != id_player) {
			if (!offline) {
				infoLobby->append("\nJugador ");
				infoLobby->append(to_string(i));

				if (i == 0) {
					infoLobby->append(" (Host)");
				}
			}
			if (clientes.at(i).ready == 0) {	//jugador no listo
				checkReady = false;
				if (!offline)
					infoLobby->append(" [No listo] ");

			}
			else {	//jugador listo
				if (!offline)
					infoLobby->append(" [Listo] ");
			}
			if (!offline) {
				infoLobby->append(" - Personaje: ");
				mostrarTipoPersonaje(i);
			}

		}
	}


	// Parte en la que se comprueba la situacion antes de empezar la apartida
	if (id_player == 0 && !offline || offline && indice == clientes.size() - 1) {	//host
		if (checkReady) {
			if (offline)
				//iniciar = true;		//si todos estan ready se puede iniciar partida offline

				infoLobby->append("\n\n Todos listos. Pulse espacio para iniciar la partida\n");
		}
		else {
			if (offline)
				iniciar = false;	//si no todos estan ready, no se puede iniciar partida offline

			if (checkReadyMe) {
				infoLobby->append("\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n");
			}
			else {
				if (offline) {
					infoLobby->append("\n\n Esperando a los demas jugadores (Pulse espacio para iniciar la partida)\n");

				}
				else {
					infoLobby->append("\n\n Pulsa espacio para indicar que estas listo\n");

				}
			}
		}

	}
	else if (!offline) {	//online
		if (checkReadyMe) {
			infoLobby->append("\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n");
		}
		else {
			infoLobby->append("\n\n Pulsa espacio para indicar que estas listo\n");
		}

	}

	if (!offline && iphost.compare("") == 0) {
		if (id_player == 0) {		//leemos ipserver
			system("ip addr show $dev | awk '/inet/ {print $2}' | cut -d/ -f1 > ip.txt");
			std::string line;
			std::ifstream myfile;
			myfile.open("./ip.txt");
			int cont_line = 0;
			if (myfile.is_open()) {
				infoLobby->append("\n\nIP conexion: ");
				while (getline(myfile, line)) {
					cont_line++;
					if (cont_line == 3) {
						iphost = line;
						infoLobby->append(iphost);
						break;
					}
				}
				myfile.close();
			}
			else {
				iphost = "IP no encontrada\n";
			}
		}
	}
	else if (!offline) {
		infoLobby->append("\n\nIP conexion: ");
		infoLobby->append(iphost);
	}
	*/
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		return Escena::tipo_escena::SALIR;
	}
	return Escena::tipo_escena::LOBBY;
}

std::string EscenaLobby::getIpConexion() { return ipConexion; }
