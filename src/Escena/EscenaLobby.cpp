#include "EscenaLobby.hpp"

EscenaLobby::EscenaLobby(Escena::tipo_escena tipo, std::string ipC) : Escena(tipo) {
	end = false;
	nElementos = 0;
	nElementos2 = 0;
	count = 3;
	iphost = "";
	ipConexion = ipC;
	connecting="";
	infoLobby= new std::string("");
	cout << "EscenaLobby: " << ipConexion << endl;
	time = Timer::getInstancia();

	offline = false;
	selection = false;
	TMotor::instancia().newHud("LobbyHUD");

	//Comprobamos que tipo de lobby entramos
	if (ipConexion.compare("") == 0) {
		//Online sin ip (a introducir)
		//cout << "ESC - SALIR\n\n";
		//cout << "1. Conectar con 127.0.0.1 \n2. Introducir ip manualmente\n";
		selection = true;		//si este boolean es true, solo funcionaran teclas 1 ,2 y escape, para seleccion de opcion en online
		iniciar = false;
		iniciado = false;
		firstInit = true;
	}
	else if (ipConexion.compare("offline") == 0) {
		//Offline 
		offline = true;
		count = 0;
		client = Client::getInstancia();

		if (client->getClientes().size() == 0){
			client->setArrayClients("", 3, false, true, -1);
			client->setArrayClients("", 0, true, false, -1);
			client->setArrayClients("", 1, true, false, -1);
			client->setArrayClients("", 2, true, false, -1);

			TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "jugador1", "assets/HUD/LobbyMenu/seleccion_chino.png");
			TMotor::instancia().getActiveHud()->traslateElement("jugador1", -0.5f, 0.35f);
			TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "jugador2", "assets/HUD/LobbyMenu/seleccion_gladiador.png");
			TMotor::instancia().getActiveHud()->traslateElement("jugador2", 0.0f, 0.35f);
			TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "jugador3", "assets/HUD/LobbyMenu/seleccion_pirata.png");
			TMotor::instancia().getActiveHud()->traslateElement("jugador3", 0.5f, 0.35f);	
			TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "jugador4", "assets/HUD/LobbyMenu/seleccion_vikingo.png");	
			TMotor::instancia().getActiveHud()->traslateElement("jugador4", -0.5f, -0.15f);	
			TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "jugador5", "assets/HUD/LobbyMenu/seleccion_vacia.png");	
			TMotor::instancia().getActiveHud()->traslateElement("jugador5", 0.0f, -0.15f);	
			TMotor::instancia().getActiveHud()->addElement(0.3f, 0.3f, "jugador6", "assets/HUD/LobbyMenu/seleccion_vacia.png");	
			TMotor::instancia().getActiveHud()->traslateElement("jugado6", 0.5f, -0.15f);	
		}
	}
	else {
		//Online con ip, cargada por sesion anterior 
		iniciar = true;
		iniciado = false;
		firstInit = false;
	}
	pressed = true;
	lanzado = false;
	conectado = false;


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
		static float f = 0.0f;
		if (!offline){
			ImGui::Text("ONLINE!");  

			if (!iniciar){                  
				if (!selection)  {
					if (connecting.compare("") != 0 && !conectado){
						ImGui::Text("Connection failed! \nTry to introduce the IP again to start an online race: ");
					}
					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             
					ImGui::InputInt4("Introduce IP", vec4a);
				}else{
					if (ImGui::Button("Start with 127.0.0.1")){
						selection = false;
						iniciar = true;
						ipConexion = "127.0.0.1";
					}
					
					if (ImGui::Button("Introduce IP")){
						selection = false;
						cout << "Introduce IP to begin an online race: ";
					}
				}
			}else{
				if (ipConexion.compare("")==0)
				ipConexion+=to_string(vec4a[0])+"."+to_string(vec4a[1])+"."+to_string(vec4a[2])+"."+to_string(vec4a[3]);
				updateLobbyOnline();
			}

	
		}else{
			std::vector<structClientes> clientes = client->getClientes();

			for (int i=0;i<clientes.size();i++){
				mostrarInfoLobby(i);
			}
		}
	}
	ImGui::Text(infoLobby->c_str());
  	ImGui::End();
}
void EscenaLobby::updateLobbyOnline(){
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
				connecting="Connecting";
				show = false;
			}

			if (time->getTimer() - nElementos2 == 1) {
				if (count <= 6) {
					connecting+=".";
					count++;
				}
				else {
					connecting="Connecting";
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
			debugClientInfo();
			nElementos = time->getTimer();
			conectado = true;
			ImGui::Text("Connection stablished!\n");
			mostrarInfoLobby(-1);
		}
	}
}

void EscenaLobby::debugClientInfo(){
	static std::vector<float> ping;
	float vec[10];
	int vec2[10];
	ping.resize(10);
	ping.erase(ping.begin());
	float f=stof(client->getClientStats(0));
	ping.push_back(f);
	for (int i=0;i<10;i++){
		vec2[i]=ping.at(i);
		vec[i]=vec2[i];
		
	}
	ImGui::PlotLines("PING", vec, IM_ARRAYSIZE(vec),0,(to_string(vec2[0])).c_str());
	ImGui::Text("Last package recieved: " );
	ImGui::Text(client->getClientStats(2).c_str());
	ImGui::Text(client->getClientStats(1).c_str());
	
	
}

void EscenaLobby::mostrarInfoLobby(int indice) {
	//Inicializacion variables cliente
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
			infoLobby->append("\nNumber of players: ");

			infoLobby->append(to_string(clientes.size()));
			infoLobby->append(" / 6");
		}
		//indicador de jugador actual seleccionado (a modificar)
		if (count != id_player) {
			if (clientes.at(id_player).corredorJugador) {
				infoLobby->append("\nHuman Racer ");
			}
			else {
				infoLobby->append("\n AI Racer ");
			}
		}
		else {
			if (clientes.at(id_player).corredorJugador) {
				infoLobby->append("\n**Human Racer ");
			}
			else {
				infoLobby->append("\n**AI Racer");
			}
		}
	}
	else {
		infoLobby->append("\nPlayers connected: ");
		infoLobby->append(to_string(client->getNumClients()));
		infoLobby->append(" / ");
		infoLobby->append(to_string(client->getMaxPlayers()));
		infoLobby->append("\nYou are the player ");
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
				infoLobby->append(" [Ready] ");
			}
			else {		//si no han aceptado todos
				checkReady = false;
				checkReadyMe = false;
				infoLobby->append(" [Not Ready] ");
			}

		}
		else {		//si estas solo tu 
			infoLobby->append(" [Ready] ");
		}
	}
	infoLobby->append("\n <-- Select a Character -->: ");

	if (id_player < size && id_player != -1)
		mostrarTipoPersonaje(id_player);

	infoLobby->append("\n");

	//recorremos todos los demas jugadores que estan en la lobby
	for (int i = 0; i < size; i++) {
		if (i != id_player) {
			if (!offline) {
				infoLobby->append("\nPlayer ");
				infoLobby->append(to_string(i));

				if (i == 0) {
					infoLobby->append(" (Host)");
				}
			}
			if (clientes.at(i).ready == 0) {	//jugador no listo
				checkReady = false;
				if (!offline)
					infoLobby->append(" [Not ready] ");

			}
			else {	//jugador listo
				if (!offline)
					infoLobby->append(" [ready] ");
			}
			if (!offline) {
				infoLobby->append(" - Character: ");
				mostrarTipoPersonaje(i);
			}

		}
	}


	// Parte en la que se comprueba la situacion antes de empezar la apartida
	if (id_player == 0 && !offline || offline && indice == clientes.size() - 1) {	//host
		if (checkReady) {
			if (offline)
				//iniciar = true;		//si todos estan ready se puede iniciar partida offline

				infoLobby->append("\n\n Everyone is ready. Press space to start the race\n");
		}
		else {
			if (offline)
				iniciar = false;	//si no todos estan ready, no se puede iniciar partida offline

			if (checkReadyMe) {
				infoLobby->append("\n\n Waiting other players (Press space to cancel)\n");
			}
			else {
				if (offline) {
					infoLobby->append("\n\n Waiting other players (Press space to start the race)\n");

				}
				else {
					infoLobby->append("\n\n Press space to be ready\n");

				}
			}
		}

	}
	else if (!offline) {	//online
		if (checkReadyMe) {
			infoLobby->append("\n\n Waiting other players (Press space to cancel)\n");
		}
		else {
			infoLobby->append("\n\n Press space to be ready\n");
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
				infoLobby->append("\n\nConnection IP: ");
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
		infoLobby->append("\n\nConnection IP: ");
		infoLobby->append(iphost);
	}
	
}


void EscenaLobby::mostrarTipoPersonaje(int i) {		//traduce de int a texto (tipo de personaje)
	std::string str;
	switch (i){
		case 0:
			str="jugador1";
		break;
		case 1:
			str="jugador2";
		break;
		case 2:
			str="jugador3";
		break;
		case 3:
			str="jugador4";
		break;
		case 4:
			str="jugador5";
		break;
		case 5:
			str="jugador6";
		break;
	}
	
	if (client->getClientes().at(i).tipoCorredor == 0){
		TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_gladiador.png");
		infoLobby->append("GLADIATOR ");
	}else if (client->getClientes().at(i).tipoCorredor == 1){
		TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_pirata.png");
		infoLobby->append("PIRATE ");
	}else if (client->getClientes().at(i).tipoCorredor == 2){
		TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_vikingo.png");
		infoLobby->append("VIKING ");
	}else if (client->getClientes().at(i).tipoCorredor == 3){
		TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_chino.png");
		infoLobby->append("CHINESE WARRIOR ");
	}
	if (client->getClientes().size() < 6){
		TMotor::instancia().getActiveHud()->changeTextureElement("jugador6", "assets/HUD/LobbyMenu/seleccion_vacio.png");
		if (client->getClientes().size() < 5)
		TMotor::instancia().getActiveHud()->changeTextureElement("jugador5", "assets/HUD/LobbyMenu/seleccion_vacio.png");

	}
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		end = true;
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && end == true) {
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

		return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
	}

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_SPACE) == GLFW_PRESS && !selection) {
		//Space==Control de ready/Iniciar partida
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en espacio\n";
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
				if (iniciar && clientes.size() > 0 || clientes.size() == 1){
					
					return Escena::tipo_escena::CARRERA;		//Iniciar la partida offline
				}else{
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
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS && !selection) {
		//Left==change character (one side)
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en cambiar jugador izquierda\n";
				client->ChangeCharacter(false);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
			if (offline) {
				std::vector<structClientes> clientes = client->getClientes();
				int k = count;
				if (clientes.at(k).tipoCorredor == 0) {
					client->setArrayClients(clientes.at(k).ip, 3, clientes.at(k).ready, clientes.at(k).corredorJugador, k);
				}
				else {
					client->setArrayClients(clientes.at(k).ip, clientes.at(k).tipoCorredor - 1, clientes.at(k).ready, clientes.at(k).corredorJugador, k);
				}
			}
		}

	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS && !selection) {
		//Right==change character (other side)
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en cambiar jugador derecha\n";
				client->ChangeCharacter(true);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
			if (offline) {
				std::vector<structClientes> clientes = client->getClientes();
				int k = count;
				if (clientes.at(k).tipoCorredor == 3) {
					client->setArrayClients(clientes.at(k).ip, 0, clientes.at(k).ready, clientes.at(k).corredorJugador, k);
				}
				else {
					client->setArrayClients(clientes.at(k).ip, clientes.at(k).tipoCorredor + 1, clientes.at(k).ready, clientes.at(k).corredorJugador, k);
				}
			}
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			iniciar = true;					//Conectar con el servidor de la IP
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_1) == GLFW_PRESS) {
		//Online: Selccionar opcion/introducir texto | Anyadir clientes offline
		if (!pressed) {
			if (offline) {
				if (client->getClientes().size() < 6) {
					int r=0;
					if (client->getClientes().size()==5)
					r=2;

					client->setArrayClients("", 3-r, true, false, -1);
					//cout<<"client size: "<<client->getClientes().size()<<endl;
				}

			}
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_2) == GLFW_PRESS) {
		//Online: Selccionar opcion/introducir texto | Borrar clientes offline
		if (!pressed) {
			if (offline) {
				if (client->getClientes().size() > 4) {
					int l = client->getClientes().size() - 1;
					client->BorrarCliente(l);
					if (count > 0) {
						count--;
					}
				}

			}
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_3) == GLFW_PRESS && !selection) {
		//Online: Introducir texto | Offline: Cambiar jugador/IA
		if (!pressed) {
			if (offline) {

				std::vector<structClientes> clientes = client->getClientes();
				int k = count;
				if (k != 0) {
					if (clientes.at(k).corredorJugador) {
						client->setArrayClients(clientes.at(k).ip, clientes.at(k).tipoCorredor, true, false, k);
					}
					else {
						client->setArrayClients(clientes.at(k).ip, clientes.at(k).tipoCorredor, false, true, k);
					}
				}
			}
			pressed = true;
			
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS && !selection && offline) {
		if (!pressed) {
			if (client->getClientes().size() - 1 > count) {
				count++;
			}
			else {
				count = 0;
			}
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS && !selection && offline) {
		//Moverse entre jugadores para cambiar opciones de seleccion en cada uno
		if (!pressed) {
			if (count > 0) {
				count--;
			}
			else {
				count = client->getClientes().size() - 1;
			}
			pressed = true;
		}
	}
	else pressed = false;
	//textoUI->setText(this->texto.c_str());

	if (iniciado)
		if (client->getStarted())
			return Escena::tipo_escena::ONLINE;


	return Escena::tipo_escena::LOBBY;
}

std::string EscenaLobby::getIpConexion() { return ipConexion; }
