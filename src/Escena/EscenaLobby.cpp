#include "EscenaLobby.hpp"
#include "Pista.hpp"
#include "elementoHud.hpp"
#include "TMotor.hpp"
#include "hud.hpp"

EscenaLobby::EscenaLobby(Escena::tipo_escena tipo, std::string ipC) : Escena(tipo) {

	nElementos = 0;
	nElementos2 = 0;
	count = 3;
	iphost = "";
	ipConexion = ipC;
	connecting = "";
	show_config = true;
	empiezaCarrera = false;
	pantallaCarga=true;
	end = false;
	checkfocus = true;
	infoLobby = new std::string("");
	time = Timer::getInstancia();
	mando = false;

	laps = 3;
	index = 1;
	index_selected = false;
	index_mapa = 1;
	index_interno = 1;
	checkCPU = false;
	offline = false;
	offline_split = false;
	selection_online = false;
	TMotor::instancia().newHud("LobbyHUD");

	//Comprobamos que tipo de lobby entramos
	if (ipConexion.compare("") == 0) {
		//Online sin ip (a introducir)
		//cout << "ESC - SALIR\n\n";
		//cout << "1. Conectar con 127.0.0.1 \n2. Introducir ip manualmente\n";
		selection_online = true;		//si este boolean es true, solo funcionaran teclas 1 ,2 y escape, para seleccion de opcion en online
		iniciar = false;
		iniciado = false;
		firstInit = true;
	}
	else if (ipConexion.compare("offline") == 0 || ipConexion.compare("offline_split") == 0) {
		//Offline 
		offline = true;
		count = 0;
		client = Client::getInstancia();

	}
	else {
		//Online con ip, cargada por sesion anterior 
		iniciar = true;
		iniciado = false;
		firstInit = false;
	}

	pressed[0] = true; pressed[1] = true; pressed[2] = true; pressed[3] = true; pressed[4] = true; pressed[5] = true;
	lanzado = false;
	conectado = false;


	//ELEMENTOS SEGUN LA ESCENA
	if (ipConexion.compare("offline_split") == 0) {

		offline_split = true;

		client->setArrayClients("", 3, false, true, -1);

	}
	else if (ipConexion.compare("offline") == 0) {
		client->setArrayClients("", 3, false, true, -1);
		client->setArrayClients("", 1, true, false, -1);
		client->setArrayClients("", 0, true, false, -1);
		client->setArrayClients("", 2, true, false, -1);
	}

	addControllers();
	//---------CONFIGURACION HUD:----------------
	if (offline)
		crearHUD();

	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf", 28.0f);
	ImGui::StyleColorsLight();
	if (!mando) {
		returnB = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "return1", "assets/HUD/LobbyMenu/return.png");
	}
	else {
		returnB = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "return1", "assets/HUD/LobbyMenu/return_xbox.png");
	}
	returnB->traslacion(-0.8f, -0.9f);
}

EscenaLobby::~EscenaLobby() {
	//cout << "\nDestructor ESCENA LOBBY. Entro.";
	limpiar();
	TMotor::instancia().cleanHUD();
	//cout << "Salgo.\n";
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
	TMotor::instancia().closeDebugWindow();
}

void EscenaLobby::update() {

	// ------------------------------
	// -------- IMGUI ---------------
	// ------------------------------
	infoLobby->clear();
	glfwPollEvents();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Mostrar ventanas
	int display_w = 0, display_h = 0;
	static int str0 = 192;
	static int vec4a[4] = { 192, 168, 1, 1 };


	glfwGetFramebufferSize(TMotor::instancia().getVentana(), &display_w, &display_h);

	ImGui_ImplGlfwGL3_NewFrame();
	if (!conectado && !offline) {
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x - 300, 30));
		ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
	}
	else if (offline || conectado) {
		if (!offline_split) {
			ImGui::SetNextWindowPos(ImVec2(display_w / 2 + 255, display_h / 2 + 255));
		}
		else {
			ImGui::SetNextWindowPos(ImVec2(display_w / 2 + 470, display_h / 2 + 280));
		}
		ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
	}
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::GetStyle().WindowBorderSize = 0;

	if (ImGui::Begin("Hola", NULL, ImGuiWindowFlags_NoResize
		| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard
		| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal)) {

		if (display_w < 1000) {
			ImGui::SetWindowFontScale(0.5f);
		}
		else {
			if (offline) {
				ImGui::SetWindowFontScale(0.9f);
			}
			else if (conectado) {
				ImGui::SetWindowFontScale(0.75f);
			}
		}
		if (!offline) {


			if (!iniciar) {
				fondo = fondo = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo2", "assets/HUD/MainMenu/fondo.png");
				ImGui::Text("ONLINE!");
				if (!selection_online) {
					if (connecting.compare("") != 0 && !conectado) {
						ImGui::Text("Connection failed! \nTry to introduce the IP again to start an online race: ");
					}
					ImGui::InputInt4("Introduce IP", vec4a);
					if (checkfocus) {
						ImGui::SetKeyboardFocusHere();
						checkfocus = false;
					}
				}
				else {
					if (ImGui::Button("Start with 127.0.0.1")) {
						selection_online = false;
						iniciar = true;
						ipConexion = "127.0.0.1";
					}

					if (ImGui::Button("Introduce IP")) {
						selection_online = false;
						//cout << "Introduce IP to begin an online race: ";
					}
				}
			}
			else {
				if (ipConexion.compare("") == 0)
					ipConexion += to_string(vec4a[0]) + "." + to_string(vec4a[1]) + "." + to_string(vec4a[2]) + "." + to_string(vec4a[3]);
				updateLobbyOnline();
				ImGui::Text(infoLobby->c_str());


				ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x * 3 / 4, ImGui::GetWindowSize().y*2.5 / 4));
				ImGui::SetNextWindowSize(ImVec2((float)display_w / 2, (float)display_h / 2));

				ImGui::SetNextWindowBgAlpha(0.0f);

				ImGui::GetStyle().WindowBorderSize = 0;
				if (ImGui::Begin("Hola2", NULL, ImGuiWindowFlags_NoResize
					| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
					| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard
					| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal))
					if (display_w < 1000) {
						ImGui::SetWindowFontScale(0.5f);
					}
					else {
						ImGui::SetWindowFontScale(0.8f);
					}
					if (conectado) {
						infoLobby->clear();
						infoLobby->append("ONLINE!\n");
						infoLobby->append("Connection stablished!\n");
						crearFicheroIP();
					}

					ImGui::End();
			}


		}
		else {

			std::vector<structClientes> clientes = client->getClientes();


			for (int i = 0; i < clientes.size(); i++) {
				//mostrarInfoLobby(i);
				mostrarTipoPersonaje(i);
			}
			mostrarInfoLobbyPropia();
			ImGui::Text(infoLobby->c_str());


		}
	}

	ImGui::End();
}
void EscenaLobby::updateLobbyOnline() {
	bool show = false;

	//Online. EN cuanto tengamos la ip inicializamos el cliente para conectar
	if (iniciar && !iniciado) {
		show = true;						//boolean para mostrar solo 1 vez conectado
		count = 0;						//contador limite de ...
		//texto2 = "";						//texto para actualizar los ...
		nElementos2 = time->getTimer();	//tiempo de mostrar informacion conectado (actualizacion puntos suspensivos)
		nElementos = time->getTimer();	//tiempo limite para conectar
		if (firstInit) {						//con ip introducida por teclado
			client = Client::getInstancia();
			client->CreateClientInterface();
			client->SetIP(ipConexion);
			client->ClientStartup();

		}
		else {								//con ip recordada por sesion
			client = Client::getInstancia();
			client->setNetloaded(false);
		}
		iniciado = true;
	}

	if (iniciado) {			//si se ha inicializado los parametros inicialies de cliente entramos

		if (show) {
			connecting = "Connecting";
			show = false;
		}

		if (time->getTimer() - nElementos2 == 1) {
			if (count <= 6) {
				connecting += ".";
				count++;
			}
			else {
				connecting = "Connecting";
				count = 0;
			}

			nElementos2 = time->getTimer();
		}
		if (!client->getConnected())
			ImGui::Text(connecting.c_str());

		if (client->ReceivePackets() == 3) {			//en caso de saltar already connected, tratamos de actualizar la informacion a partir del servidor
			client->ActualizarClienteConectado();
		}
		if (client->ReceivePackets() == 1 || time->getTimer() - nElementos > 8) {			//si la respuesta es 1, significa que se ha producido un error (conexion, tiempo agotadod de servidor,etc). 
																					//O bien se ha acabado el tiempo para conectar
			//reseteamos parametros
			iniciar = false;
			iniciado = false;
			ipConexion = "";
			client->ShutDownClient();
			nElementos = time->getTimer();
		}

		//si despues de todo se ha recibido conexion y cliente esta conectado mostramos informacion relativa
		if (client->getConnected()) {
			if (!offline && !conectado) {
				crearHUD();

			}
			//debugClientInfo(); DEBUG DE RED!
			mostrarInfoLobbyPropia();
			nElementos = time->getTimer();
			conectado = true;

			std::vector<structClientes> clientes = client->getClientes();
			for (int i = 0; i < clientes.size(); i++) {
				//mostrarInfoLobby(i);
				mostrarTipoPersonaje(i);
			}

		}
	}
}
void EscenaLobby::debugClientInfo() {
	static std::vector<float> ping;
	float vec[10];
	int vec2[10];
	ping.resize(10);
	ping.erase(ping.begin());
	float f = stof(client->getClientStats(0));
	ping.push_back(f);
	for (int i = 0; i < 10; i++) {
		vec2[i] = ping.at(i);
		vec[i] = vec2[i];

	}
	ImGui::PlotLines("PING", vec, IM_ARRAYSIZE(vec), 0, (to_string(vec2[0])).c_str());
	ImGui::Text("Last package recieved: ");
	ImGui::Text(client->getClientStats(2).c_str());
	ImGui::Text(client->getClientStats(1).c_str());


}
void EscenaLobby::mostrarInfoLobbyPropia() {
	std::vector<structClientes> clientes = client->getClientes();
	checkReady = true;
	int id_player;
	bool bc = false;
	if (offline) {
		id_player = 0;
	}
	else {
		id_player = client->getControlPlayer();
	}
	//infoLobby->append("\nNumber of players: ");
	//	infoLobby->append("\nNumero de mandos conectados: " + to_string(numPantallas));
	//infoLobby->append(to_string(clientes.size()));
	//infoLobby->append(" / 6");
	for (int c = 0; c < clientes.size(); c++) {
		/*	if (!clientes.at(c).ready) {	//comprobamos si han aceptado todos
			bc = false;
			/*if (!offline || offline_split && clientes.at(c).corredorJugador == true) {

				if (clientes.at(c).tipoCorredor == 0) {
					playersReady.at(c)->cambiarTransparencia(true, 0);
				}
				else if (clientes.at(c).tipoCorredor == 1) {
					playersReady.at(c)->cambiarTransparencia(true, 0);
				}
				else if (clientes.at(c).tipoCorredor == 2) {
					playersReady.at(c)->cambiarTransparencia(true, 0);
				}
				else if (clientes.at(c).tipoCorredor == 3) {
					playersReady.at(c)->cambiarTransparencia(true, 0);
				}
			}
		}*/
		/*
		GLADIADOR == 0
		PIRATA == 1
		VIKINGO == 2
		CHINO == 3
		*/
		/*else if (!offline || offline_split && clientes.at(c).corredorJugador == true) {
			/* Controlar si el jugador esta listo y cambiar a READY
			if (clientes.at(c).tipoCorredor == 0) {
				playersReady.at(c)->cambiarTransparencia(true, 1);
			}
			else if (clientes.at(c).tipoCorredor == 1) {
				playersReady.at(c)->cambiarTransparencia(true, 1);
			}
			else if (clientes.at(c).tipoCorredor == 2) {
				playersReady.at(c)->cambiarTransparencia(true, 1);
			}
			else if (clientes.at(c).tipoCorredor == 3) {
				playersReady.at(c)->cambiarTransparencia(true, 1);
			}
		}*/
		if ((!clientes.at(c).ready)) {
			checkReady = false;
		}
	}
	/*if (clientes.size() == 1) {
		playersReady.at(0)->cambiarTransparencia(true, 1);
	}*/
	if (!checkReady) {
		if (!mando) {
			start->cambiarTextura("assets/HUD/LobbyMenu/ready.png");
		}
		else {
			start->cambiarTextura("assets/HUD/LobbyMenu/ready_xbox.png");
		}
	}
	else {
		if (!mando) {
			start->cambiarTextura("assets/HUD/LobbyMenu/ready_final.png");
		}
		else {
			start->cambiarTextura("assets/HUD/LobbyMenu/ready_final_xbox.png");
		}
	}



}

void EscenaLobby::crearFicheroIP() {
	if (!offline && iphost.compare("") == 0) {
		if (client->getControlPlayer() == 0) {		//leemos ipserver
			system("ip addr show $dev | awk '/inet/ {print $2}' | cut -d/ -f1 > ip.txt");
			std::string line;
			std::ifstream myfile;
			myfile.open("./ip.txt");
			int cont_line = 0;
			if (myfile.is_open()) {
				infoLobby->append("\n IP: ");
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
				iphost = "IP not found\n";
			}
		}
	}
	else if (!offline) {
		infoLobby->append("\n IP: ");
		infoLobby->append(iphost);
		ImGui::Text(infoLobby->c_str());

	}
}

void EscenaLobby::mostrarTipoPersonaje(int i) {		//traduce de int a imagen (tipo de personaje)
	//offline
	if (offline && !offline_split && i == 0) {

		if (client->getClientes().at(i).tipoCorredor == 0) {
			personajeOff->cambiarTextura("assets/HUD/LobbyMenu/offline/GladiOffline.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 1) {
			personajeOff->cambiarTextura("assets/HUD/LobbyMenu/offline/PirataOffline.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 2) {
			personajeOff->cambiarTextura("assets/HUD/LobbyMenu/offline/VikOffline.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 3) {
			personajeOff->cambiarTextura("assets/HUD/LobbyMenu/offline/ChinoOffline.png");
		}

	}
	//splitscreen u online
	else if (offline_split && client->getClientes().at(i).corredorJugador || !offline) {

		//Setear por defectos todas a noPlayer
		players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/NoPlayer.png");


		if (client->getClientes().at(i).tipoCorredor == 0 && offline || client->getClientes().at(i).tipoCorredor == 0 && !offline && !checkReady && client->getClientes().at(i).ready == false) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, false, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/GladiMultiplayer.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 1 && offline || client->getClientes().at(i).tipoCorredor == 1 && !offline && client->getClientes().at(i).ready == false) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, false, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/PirataMultiplayer.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 2 && offline || client->getClientes().at(i).tipoCorredor == 2 && !offline && client->getClientes().at(i).ready == false) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, false, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/VikMultiplayer.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 3 && offline || client->getClientes().at(i).tipoCorredor == 3 && !offline && client->getClientes().at(i).ready == false) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, false, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/ChinoMultiplayer.png");
		}
		//ready
		if (client->getClientes().at(i).tipoCorredor == 0 && index != 1 && offline || client->getClientes().at(i).tipoCorredor == 0  && !offline && client->getClientes().at(i).ready == true) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, true, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/GladiMultiplayerReady.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 1 && index != 1 && offline || client->getClientes().at(i).tipoCorredor == 1 && !offline && client->getClientes().at(i).ready == true) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, true, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/PirataMultiplayerReady.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 2 && index != 1 && offline || client->getClientes().at(i).tipoCorredor == 2 && !offline && client->getClientes().at(i).ready == true) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, true, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/VikMultiplayerReady.png");
		}
		else if (client->getClientes().at(i).tipoCorredor == 3 && index != 1 && offline || client->getClientes().at(i).tipoCorredor == 3 && !offline && client->getClientes().at(i).ready == true) {
			client->setArrayClients(client->getClientes().at(i).ip, client->getClientes().at(i).tipoCorredor, true, true, i);
			players.at(i)->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/ChinoMultiplayerReady.png");
		}
	}
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {

	//bool mandoConectado[numPantallas];
	bool *mandoConectado=new bool[numPantallas];
	
	for (int id = 0; id < numPantallas; id++) {
		//Comprobador de de mando y recoleccion de inputs
		mandoConectado[id] = false;
		const unsigned char *buttons = nullptr;
		const float *axes = nullptr;
		if (1 == glfwJoystickPresent(id)) {
			mandoConectado[id] = true;
			int buttonCount;
			buttons = glfwGetJoystickButtons(id, &buttonCount);
			int axesCount;
			axes = glfwGetJoystickAxes(id, &axesCount);
			//  || (mandoConectado && (GLFW_PRESS == buttons[12] || 0.5f <= axes[0]) ) 
		}
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && (offline && id == 0 || !offline) && end == true) {
			//if(conectado)
			if (offline) {
				client->BorrarClientes();
			}
			else if (iniciar) {
				nElementos = time->getTimer();
				client->ShutDownClient();
			}

			iniciar = false;
			iniciado = false;
			ipConexion = "";
			delete mandoConectado;
			return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
		}
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS && (offline && id == 0 || !offline)  || (mandoConectado[id] && (GLFW_PRESS == buttons[1]) && (offline && id == 0 || !offline))) {
			if (!pressed[id]) {
				if (index_selected) {
					index_selected = false;
					actualizarSelector();
				}
				else {
					end = true;
				}
				pressed[id] = true;
			}
		}
		else if ((glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_SPACE) == GLFW_PRESS && (offline && id == 0 || !offline) || (mandoConectado[id] && (GLFW_PRESS == buttons[7]) && (offline && id == 0 || !offline))) && !selection_online && (offline || conectado)) {
			//Space==Control de ready/Iniciar partida
			if (!pressed[id]) {
				pressed[id] = true;				
				//Si todos estan con la seleccion , se carga el juego
				if (checkReady && offline_split) iniciarCarrera();
				if (offline && !offline_split) iniciarCarrera();
				if (!offline)iniciarCarrera();

			}
		}
		else if ((glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS && (offline && id == 0 || !offline) || (mandoConectado[id] && (GLFW_PRESS == buttons[11] || -0.5f >= axes[0]))) && !selection_online && (offline || conectado)) {
			//Left==change character (one side)

			if (!pressed[id]) {
				pressed[id] = true;
				if (index_selected) {
					movimientoHorizontal(index, false,id);
				}
				else if (!index_selected && (offline && id == 0 || !offline)) {
					movimientoHorizontal(-1, false, -1);
				}
			}

		}
		else if ((glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS && (offline && id == 0 || !offline) || (mandoConectado[id] && (GLFW_PRESS == buttons[12] || 0.5f <= axes[0]))) && !selection_online && (offline || conectado)) {
			//Right==change character (other side)
			if (!pressed[id]) {
				pressed[id] = true;
				if (index_selected) {
					movimientoHorizontal(index, true, id);
				}
				else if (!index_selected && (offline && id == 0 || !offline)) {
					movimientoHorizontal(-1, false, -1);
				}
			}
		}
		else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS && (offline && id == 0 || !offline) || (mandoConectado[id] && (GLFW_PRESS == buttons[0]) && (offline && id == 0 || !offline)) ) {
			if (!pressed[0]) {
				if (!selection_online && !offline) {
					iniciar = true;					//Conectar con el servidor de la IP

				}
				if (index_selected) {
					index_selected = false;
				}
				else if (!index_selected) {
					index_selected = true;
				}
				if (conectado || offline)
					actualizarSelector();
				pressed[0] = true;
			}

		}
		else if ((glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS && (offline && id == 0 || !offline) || (mandoConectado[id] && (GLFW_PRESS == buttons[14] || 0.5f <= axes[1]))) && !selection_online && (offline || conectado)) {
			if (!pressed[0]) {
				/*if (client->getClientes().size() - 1 > count) {
					count++;
				}
				else {
					count = 0;
				}*/
				if (!index_selected && (offline && id == 0 || !offline)) {
					movimientoVertical(false);
				}
				else if (index_selected && index == 4 && !conectado) {
					movimientoVertical(true);
				}
				pressed[id] = true;
			}
		}
		else if ((glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS && (offline && id == 0 || !offline) || (mandoConectado[id] && (GLFW_PRESS == buttons[13] || -0.5f >= axes[1]))) && !selection_online && (offline || conectado)) {
			//Moverse entre jugadores para cambiar opciones de seleccion en cada uno
			if (!pressed[0]) {
				/*if (count > 0) {
					count--;
				}
				else {
					count = client->getClientes().size() - 1;
				}*/
				if (!index_selected && (offline && id == 0 || !offline)) {
					movimientoVertical(false);
				}
				else if (index_selected && index == 4 && !conectado) {
					movimientoVertical(true);
				}
				pressed[id] = true;
			}
		}
		else pressed[id] = false;
		//textoUI->setText(this->texto.c_str());

		if (iniciado)
			if (client->getStarted()) {
				//if (empiezaCarrera)
				if (pantallaCarga){
					if (!mando){
						fondo_final = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo3", "assets/HUD/LobbyMenu/load.png");
					}else{
						fondo_final = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo3", "assets/HUD/LobbyMenu/load_xbox.png");
					}
					pantallaCarga=false;
					delete mandoConectado;
					return Escena::tipo_escena::LOBBY;
				}
				if (index_mapa == 1) {
					Pista::getInstancia()->setNombreMapa("pirata");
				}
				else if (index_mapa == 2) {
					Pista::getInstancia()->setNombreMapa("gladiador");
				}
				Pista::getInstancia()->setNumVueltas(laps);
				delete mandoConectado;
				return Escena::tipo_escena::ONLINE;
			}

		if (empiezaCarrera) {
			if (pantallaCarga){
				if (!mando){
					fondo_final = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo3", "assets/HUD/LobbyMenu/load.png");
				}else{
					fondo_final = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo3", "assets/HUD/LobbyMenu/load_xbox.png");
				}
				pantallaCarga=false;
				delete mandoConectado;
				return Escena::tipo_escena::LOBBY;
			}
			fondo = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo2", "assets/HUD/LobbyMenu/offline/fondoMulti.png");
			if (index_mapa == 1) {
				Pista::getInstancia()->setNombreMapa("pirata");
			}
			else if (index_mapa == 2) {
				Pista::getInstancia()->setNombreMapa("gladiador");
				TMotor::instancia().setDrawingDistance(true, 1100.0f);
				TMotor::instancia().setFaceCulling(false);
			}
			if (checkCPU) {
			
				client->setArrayClients("", 1, true, false, -1);
			
				//client->setArrayClients("", 0, true, false, -1);

			}
			//Pista::getInstancia()->setNombreMapa(str1.c_str());
			Pista::getInstancia()->setNumVueltas(laps);
			delete mandoConectado;
			return Escena::tipo_escena::CARRERA;		//Iniciar la partida offline

		}
	}
	delete mandoConectado;
	return Escena::tipo_escena::LOBBY;
}

void EscenaLobby::movimientoHorizontal(int k, bool dcha, int id) {

	if (k == -1) {//movimiento a la izq
		if (index == 1) {
			if (offline) {
				if (dcha) {
					index = 4;
				}
				else {
					index = 3;
				}
			}
			else {
				index = 3;
			}
		}
		else if (index == 3 && offline) {
			index = 4;
		}
		else {
			index = 3;
		}
	}
	else if (k == 1) {
		if (!dcha) {
			if (conectado) {
				client->ChangeCharacter(false);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
			if (offline) {
				std::vector<structClientes> clientes = client->getClientes();

				if (clientes.at(id).tipoCorredor == 0) {
					client->setArrayClients(clientes.at(id).ip, 3, clientes.at(id).ready, clientes.at(id).corredorJugador, id);
				}
				else {
					client->setArrayClients(clientes.at(id).ip, clientes.at(id).tipoCorredor - 1, clientes.at(id).ready, clientes.at(id).corredorJugador, id);
				}
			}
		}
		else {
			if (conectado) {
				client->ChangeCharacter(true);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
			if (offline) {
				std::vector<structClientes> clientes = client->getClientes();

				if (clientes.at(id).tipoCorredor == 3) {
					client->setArrayClients(clientes.at(id).ip, 0, clientes.at(id).ready, clientes.at(id).corredorJugador, id);
				}
				else {
					client->setArrayClients(clientes.at(id).ip, clientes.at(id).tipoCorredor + 1, clientes.at(id).ready, clientes.at(id).corredorJugador, id);
				}
			}
		}
	}
	else if (k == 4 && offline) {//LAPS && CPU offline
	   /*
		   indice interno 1 == CPU
		   indice interno 2 == LAPS
	   */

		if (index_interno == 2) {


			if (dcha) {
				if (laps < 3)
					laps++;

			}
			else {
				if (laps > 1)
					laps--;

			}
			switch (laps) {
			case 1:
				checkLAPS->cambiarTextura("assets/HUD/LobbyMenu/offline/LAP1.png");
				break;
			case 2:
				checkLAPS->cambiarTextura("assets/HUD/LobbyMenu/offline/LAP2.png");
				break;
			case 3:
				checkLAPS->cambiarTextura("assets/HUD/LobbyMenu/offline/LAP3.png");
				break;
			}

		}
		else {
			if (dcha) {
				if (offline_split) {
					if (checkCPU) {
						checkCPU = false;
						CPU->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/CPUSPLITOFF.png");
					}


				}
				else if (offline) {
					if (client->getClientes().size() < 6) {
						int r = 0;
						if (client->getClientes().size() == 2)
							r = 2;
						if (client->getClientes().size() == 3)
							r = 3;
						if (client->getClientes().size() == 4)
							r = 1;
						if (client->getClientes().size() == 5)
							r = 2;

						client->setArrayClients("", 3 - r, true, false, -1);
						//cout<<"client size: "<<client->getClientes().size()<<endl;
					}
				}
			}
			else {
				if (offline_split) {
					if (!checkCPU) {
						checkCPU = true;
						CPU->cambiarTextura("assets/HUD/LobbyMenu/Multiplayer/CPUSPLIT.png");
					}
				}
				else if (offline) {
					if (client->getClientes().size() > 4 && !offline_split) {
						int l = client->getClientes().size() - 1;
						client->BorrarCliente(l);
						if (count > 0) {
							count--;
						}
					}
					else
						if (client->getClientes().size() > 1 && offline_split) {
							int l = -1;
							for (int i = client->getClientes().size() - 1; i >= 1; i--) {
								if (client->getClientes().at(i).corredorJugador == false) {
									l = i;
									break;
								}
							}
							if (l != -1) {
								client->BorrarCliente(l);
								if (count > 0) {
									count--;
								}
							}
						}

				}

			}
			if (!offline_split) {
				switch (int l = client->getClientes().size() - 1) {
				case 3:
					CPU->cambiarTextura("assets/HUD/LobbyMenu/offline/CPU3.png");
					break;
				case 4:
					CPU->cambiarTextura("assets/HUD/LobbyMenu/offline/CPU4.png");
					break;
				case 5:
					CPU->cambiarTextura("assets/HUD/LobbyMenu/offline/CPU5.png");
					break;
				}
			}

		}
	}
	else if (k == 3) {
		if (offline) {
			if (index_mapa == 1) {
				index_mapa = 2;
				mapa->cambiarTextura("assets/HUD/LobbyMenu/offline/Coliseo.png");
			}
			else {
				index_mapa = 1;
				mapa->cambiarTextura("assets/HUD/LobbyMenu/offline/Isla.png");
			}
		}
	}

	actualizarSelector();
}

void EscenaLobby::movimientoVertical(bool interno) {
	if (!interno) {
		if (index == 1) {
			index = 3;

		}
		else if (index == 3) {
			index = 1;

		}
		else {

			index = 1;


		}
	}
	else {

		if (offline) {
			if (index_interno == 1) {
				index_interno = 2;
			}
			else {
				index_interno = 1;
			}
		}
	}
	actualizarSelector();

}

void EscenaLobby::actualizarSelector() {
	std::string str1 = "seleccion1";
	std::string str2 = "seleccion2";
	if (index_selected) {
		seleccion->cambiarTextura("assets/HUD/LobbyMenu/offline/seleccionON.png");
	}
	else {
		seleccion->cambiarTextura("assets/HUD/LobbyMenu/offline/seleccionOFF.png");
	}
	//index 3 == seleccion Mapa
	//index 4 == cpu/laps
	if (index == 3) {
		if (offline) {
			seleccion->escalado(0.655, 0.725);
			seleccion->traslacion(-0.225f, -0.5050);
		}

		if (!offline || offline_split) {
			//Escalado y traslacion del Hover para la seleccion del mapa de splitScreen
			seleccion->escalado(0.68, 0.725);
			seleccion->traslacion(-0.290f, -0.5050);
			flechaIzq->traslacion(-0.63f, -0.525f);
			flechaDer->traslacion(0.18f, -0.525f);
		}



	}
	else if (index == 4) {
		// Escena multijugador
		seleccion->escalado(0.25, 0.625);
		seleccion->traslacion(0.650f, -0.4550);
		//index interno == 1 seleccion de personaje
		if (offline_split) {
			if (index_selected) {
				if (index_interno == 1) {//CPU
					seleccion->escalado(0.23, 0.13);
					seleccion->traslacion(0.650f, -0.3250);
				}
				else {//LAPS
					seleccion->escalado(0.23, 0.13);
					seleccion->traslacion(0.650f, -0.5050);

				}

			}


		}
		else {//offline LAPS CPU
			seleccion->escalado(0.455f, 0.755f);
			seleccion->traslacion(0.68f, -0.38f);
			if (index_selected) {
				//index_interno = 1 = CPU 
				if (index_interno == 1) {
					seleccion->escalado(0.40f, 0.14f);
					seleccion->traslacion(0.69f, -0.25f);
				}
				//index_interno = 2 = LAPS
				else {
					seleccion->escalado(0.40f, 0.14f);
					seleccion->traslacion(0.69f, -0.45f);
				}
			}
		}
		//solo para multiplayer	
		// index == 1 
	}
	else if (index == 1) {
		if (!offline || offline_split) {// Estando en la seleccion de mapa , subir a seleccion de personajes
			seleccion->escalado(1.020f, 0.99f);
			seleccion->traslacion(0.015, 0.355f);
		}
		else {//offline personaje
			seleccion->escalado(1, 1.0);
			seleccion->traslacion(-0.05f, 0.35f);
			flechaDer->traslacion(0.55f, 0.32f);
			flechaIzq->traslacion(-0.65f, 0.32f);
		}
	}
	else if (index_selected) {
		if (index_interno == 1) {

			//TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  0.45f);
			//TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.9f,  0.45f);
		}
		else {

			//TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  0.18f);
			//TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.75f,  0.18f);
		}

		//TMotor::instancia().getActiveHud()->scaleElement(str1.c_str(), 0.5f, 0.3f);
		//TMotor::instancia().getActiveHud()->scaleElement(str2.c_str(), 0.5f, 0.3f);
	}
	else {

		//TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  0.28f);
		//TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.9f,  0.28f);
	}

}

void EscenaLobby::iniciarCarrera() {
	if (conectado) {
		client->RaceStart();
		//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
	}
	if (offline) {
		iniciar = true;
		std::vector<structClientes> clientes = client->getClientes();
		for (int i = 0; i < clientes.size(); i++) {
			if (clientes.at(i).ready == false) {
				iniciar = false;
			}
		}
		if (iniciar && clientes.size() > 0 || clientes.size() == 1) {
			empiezaCarrera = true;
		}
		else {
			std::vector<structClientes> clientes = client->getClientes();
			int k = count;
			if (clientes.at(k).ready == false) {
				client->setArrayClients(clientes.at(k).ip, clientes.at(k).tipoCorredor, true, clientes.at(k).corredorJugador, k);
			}
			else {
				client->setArrayClients(clientes.at(k).ip, clientes.at(k).tipoCorredor, false, clientes.at(k).corredorJugador, k);
			}
		}
	}
}

void EscenaLobby::crearHUD() {

	if (offline) {
		if (!offline_split) {
			//fondo pantalla
			fondo = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo", "assets/HUD/LobbyMenu/offline/fondoOffline.png");
			//personaje 
			personajeOff = TMotor::instancia().getActiveHud()->addElementHud(1, 0.75, "jugador", "assets/HUD/LobbyMenu/offline/ChinoOffline.png");
			personajeOff->traslacion(-0.05f, 0.35f);
			flechaDer = TMotor::instancia().getActiveHud()->addElementHud(0.15, 0.15, "PressedRight", "assets/HUD/LobbyMenu/offline/flechaDerNoPulsada.png");
			flechaIzq = TMotor::instancia().getActiveHud()->addElementHud(0.15, 0.15, "PressedLeft", "assets/HUD/LobbyMenu/offline/flechaIzqNoPulsada.png");
			flechaDer->traslacion(0.55f, 0.32f);
			flechaIzq->traslacion(-0.65f, 0.32f);
			seleccion = TMotor::instancia().getActiveHud()->addElementHud(1, 0.75, "seleccion", "assets/HUD/LobbyMenu/offline/seleccionOFF.png");
			seleccion->traslacion(-0.05f, 0.35f);
			CPU = TMotor::instancia().getActiveHud()->addElementHud(0.20, 0.09, "CPU", "assets/HUD/LobbyMenu/offline/CPU3.png");
			CPU->traslacion(0.74f, -0.255f);
			checkLAPS = TMotor::instancia().getActiveHud()->addElementHud(0.20, 0.09, "LAPS", "assets/HUD/LobbyMenu/offline/LAP3.png");
			checkLAPS->traslacion(0.74f, -0.45f);
			if (!mando) {
				start = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "start", "assets/HUD/LobbyMenu/start.png");
				selectB = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "select", "assets/HUD/LobbyMenu/enter.png");
			}
			else {
				start = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "start", "assets/HUD/LobbyMenu/start_xbox.png");
				selectB = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "select", "assets/HUD/LobbyMenu/enter_xbox.png");
			}
			start->traslacion(0.8f, -0.9f);
			selectB->traslacion(0.0f, -0.9f);
			mapa = TMotor::instancia().getActiveHud()->addElementHud(0.65, 0.65, "mapa", "assets/HUD/LobbyMenu/offline/Isla.png");
			mapa->traslacion(-0.225f, -0.45);

		}

	}

	//Carga de elementos para la escena de pantalla partida
	if (offline_split || !offline) {
		if (offline_split) {
			fondo = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo", "assets/HUD/LobbyMenu/Multiplayer/fondoMulti.png");
		}
		else {
			fondo = TMotor::instancia().getActiveHud()->addElementHud(2, 2, "fondo", "assets/HUD/LobbyMenu/Multiplayer/HUD_Multiplayer.png");
		}
		players.push_back(TMotor::instancia().getActiveHud()->addElementHud(0.445, 0.945, "player1", "assets/HUD/LobbyMenu/Multiplayer/ChinoMultiplayer.png"));
		players.push_back(TMotor::instancia().getActiveHud()->addElementHud(0.445, 0.945, "player2", "assets/HUD/LobbyMenu/Multiplayer/NoPlayer.png"));
		players.push_back(TMotor::instancia().getActiveHud()->addElementHud(0.445, 0.945, "player3", "assets/HUD/LobbyMenu/Multiplayer/NoPlayer.png"));
		players.push_back(TMotor::instancia().getActiveHud()->addElementHud(0.445, 0.945, "player4", "assets/HUD/LobbyMenu/Multiplayer/NoPlayer.png"));
		players.at(0)->traslacion(-0.682f, 0.355f);
		players.at(1)->traslacion(-0.219f, 0.355f);
		players.at(2)->traslacion(0.244f, 0.355f);
		players.at(3)->traslacion(0.707f, 0.355f);


		seleccion = TMotor::instancia().getActiveHud()->addElementHud(1.88, 0.96, "seleccion", "assets/HUD/LobbyMenu/offline/seleccionOFF.png");
		seleccion->traslacion(0.012f, 0.355f);
		if (offline_split) {
			CPU = TMotor::instancia().getActiveHud()->addElementHud(0.20, 0.09, "CPU", "assets/HUD/LobbyMenu/Multiplayer/CPUSPLITOFF.png");
			CPU->traslacion(0.74f, -0.315f);
			checkLAPS = TMotor::instancia().getActiveHud()->addElementHud(0.20, 0.09, "LAPS", "assets/HUD/LobbyMenu/offline/LAP3.png");
			checkLAPS->traslacion(0.74f, -0.51f);
		}
		flechaDer = TMotor::instancia().getActiveHud()->addElementHud(0.15, 0.15, "PressedRight", "assets/HUD/LobbyMenu/offline/flechaDerNoPulsada.png");
		flechaIzq = TMotor::instancia().getActiveHud()->addElementHud(0.15, 0.15, "PressedLeft", "assets/HUD/LobbyMenu/offline/flechaIzqNoPulsada.png");
		flechaDer->traslacion(0.15f, -0.53f);
		flechaIzq->traslacion(-0.65f, -0.53f);
		if (!mando) {
			start = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "start", "assets/HUD/LobbyMenu/ready.png");
		}
		else {
			start = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "start", "assets/HUD/LobbyMenu/ready_xbox.png");
		}
		start->traslacion(0.8f, -0.9f);
		//mapa
		mapa = TMotor::instancia().getActiveHud()->addElementHud(0.65, 0.65, "mapa", "assets/HUD/LobbyMenu/offline/Isla.png");
		mapa->traslacion(-0.250f, -0.50);

	}
	if (!mando) {
		returnB = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "return", "assets/HUD/LobbyMenu/return.png");
	}
	else {
		returnB = TMotor::instancia().getActiveHud()->addElementHud(0.25, 0.15, "return", "assets/HUD/LobbyMenu/return_xbox.png");
	}
	returnB->traslacion(-0.8f, -0.9f);
}

std::string EscenaLobby::getIpConexion() { return ipConexion; }

void EscenaLobby::addControllers() {
	numPantallas = 1;
	if (glfwJoystickPresent(0) == 1) {
		mando = true;
	}
	if (offline_split) {
		for (int mandos = 1; mandos < 4; mandos++) {
			if (glfwJoystickPresent(mandos) == 1) {
				//std::cout << "Mando " << mandos << " detectado!" << std::endl;
				numPantallas++;
			}
		}
		if (offline_split && numPantallas > 1) {
			for (int p = 0; p < numPantallas - 1; p++) {
				int k = 0;
				std::vector<structClientes> clientes = client->getClientes();
				for (int l = 0; l < clientes.size(); l++) {
					if (clientes.at(l).corredorJugador == true) {
						k++;
					}
				}

				if (clientes.size() > 1) {
					bool checkIA = false;
					if (k < 4) {
						for (int l = 0; l < clientes.size(); l++) {
							if (clientes.at(l).corredorJugador == false) {
								client->setArrayClients(clientes.at(l).ip, clientes.at(l).tipoCorredor, true, true, l);
								checkIA = true;
								break;
							}
						}
						if (!checkIA) {
							if (clientes.size() < 6)
								client->setArrayClients("", 3 - p, true, true, -1);
						}
					}
				}
				else {
					if (clientes.size() < 6 && k < 4)
						client->setArrayClients("", 3 - p, true, true, -1);
				}
			}
		}
		if (numPantallas > 4) {
			numPantallas = 4;
		}
	}
}