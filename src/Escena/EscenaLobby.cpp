#include "EscenaLobby.hpp"
#include "Pista.hpp"

EscenaLobby::EscenaLobby(Escena::tipo_escena tipo, std::string ipC) : Escena(tipo) {
	
	nElementos = 0;
	nElementos2 = 0;
	count = 3;
	iphost = "";
	ipConexion = ipC;
	connecting="";
	show_config=true;
	empiezaCarrera=false;
	end = false;
	infoLobby= new std::string("");
	time = Timer::getInstancia();

	laps=3;
	index=1;
	index_selected=false;
	index_mapa=1;
	index_interno=1;
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
	else if (ipConexion.compare("offline") == 0 ||  ipConexion.compare("offline_split") == 0){
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

	pressed = true;
	lanzado = false;
	conectado = false;


	//ELEMENTOS SEGUN LA ESCENA
	if (ipConexion.compare("offline_split") == 0){

		offline_split=true;	

		client->setArrayClients("", 3, false, true, -1);
				
	}else if (ipConexion.compare("offline") == 0){
		client->setArrayClients("", 3, false, true, -1);
		client->setArrayClients("", 1, true, false, -1);
		client->setArrayClients("", 0, true, false, -1);
		client->setArrayClients("", 2, true, false, -1);
	}

	/*if (offline){		

		for(int mandos = 1; mandos < 4; mandos++){
			if(glfwJoystickPresent(mandos) == 1){
				std::cout << "Mando " << mandos << " detectado!" << std::endl;
				numPantallas++;
			}
		}
		for(int numClientes = 0; numClientes < 4; numClientes++){
			if(numClientes < numPantallas)
				client->setArrayClients("", numClientes, false, true, -1);
			else
				client->setArrayClients("", numClientes, true, false, -1);		
		}

	}*/

	//---------CONFIGURACION HUD:----------------
	if (offline)
	crearHUD();

	TMotor::instancia().initDebugWindow();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/font/OCRAStd.ttf",28.0f);
	ImGui::StyleColorsLight(); 
	
}

EscenaLobby::~EscenaLobby() {
	cout << "\nDestructor ESCENA LOBBY. Entro.";
	limpiar();
	TMotor::instancia().cleanHUD();
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
	if (!conectado && !offline){
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x-300,30));
		ImGui::SetNextWindowSize( ImVec2( (float)display_w , (float)display_h ) );
	}else if (offline || conectado){
		if (!offline_split){
			ImGui::SetNextWindowPos(ImVec2(display_w/2+255, display_h/2+255));
		}else{
			ImGui::SetNextWindowPos(ImVec2(display_w/2+470, display_h/2+280));
		}
		ImGui::SetNextWindowSize( ImVec2( (float)display_w , (float)display_h ) );
	}
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::GetStyle().WindowBorderSize=0;
	
	if (ImGui::Begin("Hola", NULL,  ImGuiWindowFlags_NoResize 
	| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
	| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
	| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal)){

		if (display_w<1000){
			ImGui::SetWindowFontScale(0.5f);
		}else{
			if (offline){
				ImGui::SetWindowFontScale(0.9f);
			}else if (conectado){
				ImGui::SetWindowFontScale(0.75f);
			}
		}
		if (!offline){
			  

			if (!iniciar){   
				ImGui::Text("ONLINE!");
				if (!selection_online)  {
					if (connecting.compare("") != 0 && !conectado){
						ImGui::Text("Connection failed! \nTry to introduce the IP again to start an online race: ");
					}
					ImGui::InputInt4("Introduce IP", vec4a);
				}else{
					if (ImGui::Button("Start with 127.0.0.1")){
						selection_online = false;
						iniciar = true;
						ipConexion = "127.0.0.1";
					}
					
					if (ImGui::Button("Introduce IP")){
						selection_online = false;
						cout << "Introduce IP to begin an online race: ";
					}
				}
			}else{
				if (ipConexion.compare("")==0)
				ipConexion+=to_string(vec4a[0])+"."+to_string(vec4a[1])+"."+to_string(vec4a[2])+"."+to_string(vec4a[3]);
				updateLobbyOnline();
				ImGui::Text(infoLobby->c_str());

				
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x-400,30));
				ImGui::SetNextWindowSize( ImVec2( (float)display_w/2 , (float)display_h/2 ) );

				ImGui::SetNextWindowBgAlpha(0.0f);
				
				ImGui::GetStyle().WindowBorderSize=0;
				if (ImGui::Begin("Hola2", NULL,  ImGuiWindowFlags_NoResize 
				| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
				| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal))
					if (display_w<1000){
						ImGui::SetWindowFontScale(0.5f);
					}else{
						ImGui::SetWindowFontScale(0.8f);
					}					
					if (conectado){
						infoLobby->clear();
						infoLobby->append("ONLINE!\n");
						infoLobby->append("Connection stablished!\n"); 
					}
					crearFicheroIP();
					ImGui::End();
			}

	
		}else{

			std::vector<structClientes> clientes = client->getClientes();
			
			
			for (int i=0;i<clientes.size();i++){
				//mostrarInfoLobby(i);
				mostrarTipoPersonaje(i);
			}
			mostrarInfoLobbyPropia();
			ImGui::Text(infoLobby->c_str());
			
				
		}
		if (show_config && offline){

			int contIA=0;

			std::vector<structClientes> clientes = client->getClientes();
			for (int i=0;i<clientes.size();i++){
				if (clientes.at(i).corredorJugador==false){
					contIA++;
				}
			}
			int display_w,display_h;
			glfwGetFramebufferSize( TMotor::instancia().getVentana() , &display_w , &display_h );
			ImGui::SetNextWindowSize( ImVec2( (float)display_w , (float)display_h ) );
			if (offline_split || !offline){
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x-360,630));
			}else{
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x-360,275));
			}
			
			
			
			ImGui::SetNextWindowBgAlpha(0.0f); 
			ImGui::Begin("IA Info", &show_config,  ImGuiWindowFlags_NoResize 
			| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
			| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal);

			if (display_w<1000){
				ImGui::SetWindowFontScale(0.8f);
			}else{
				ImGui::SetWindowFontScale(1.2f);
			}	
			if (offline_split){
				ImGui::Text("%i | Max 5",contIA);
			}else if (offline) {
				ImGui::Text("%i/5|Min 3",contIA);
			}

			ImGui::End();
			ImGui::SetNextWindowSize( ImVec2( (float)display_w , (float)display_h ) );				
			if (offline_split || !offline){
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x-470,720));
			}else{
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x-390,385));
			}
			
			ImGui::SetNextWindowBgAlpha(0.0f); 
			ImGui::Begin("Vueltas info", &show_config,  ImGuiWindowFlags_NoResize 
			| ImGuiTreeNodeFlags_CollapsingHeader | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoTitleBar | ImGuiConfigFlags_NavEnableKeyboard 
			| ImGuiConfigFlags_NavEnableGamepad | ImGuiInputTextFlags_CharsHexadecimal);

			if (display_w<1000){
				ImGui::SetWindowFontScale(0.8f);
			}else{
				ImGui::SetWindowFontScale(1.2f);
			}	
			
			ImGui::Text("%i/3|Min 1",laps);

			ImGui::End();
		}
	}
	
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
			if (!offline && !conectado){
				crearHUD();
				
			}
			//debugClientInfo(); DEBUG DE RED!
			mostrarInfoLobbyPropia();
			nElementos = time->getTimer();
			conectado = true;

			std::vector<structClientes> clientes = client->getClientes();
			for (int i=0;i<clientes.size();i++){
				//mostrarInfoLobby(i);
				mostrarTipoPersonaje(i);
			}
			
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
void EscenaLobby::mostrarInfoLobbyPropia() {
	std::vector<structClientes> clientes = client->getClientes();
	bool checkReadyMe = true;
	bool checkReady = true;
	int id_player;
	bool bc=false;
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
		if (clientes.at(c).ready == 0) {	//comprobamos si han aceptado todos
			bc = true;
			if (!offline || offline_split && clientes.at(c).corredorJugador==true){
				if (c==0){
					TMotor::instancia().getActiveHud()->changeTextureElement("player1", "assets/HUD/LobbyMenu/player1_notready.png");
					TMotor::instancia().getActiveHud()->changeTextureElement("ReadyButton", "assets/HUD/LobbyMenu/ready.png");
				}else if (c==1){
					TMotor::instancia().getActiveHud()->changeTextureElement("player2", "assets/HUD/LobbyMenu/player2_notready.png");
				}else if (c==2){
					TMotor::instancia().getActiveHud()->changeTextureElement("player3", "assets/HUD/LobbyMenu/player3_notready.png");
				}else if (c==3){
					TMotor::instancia().getActiveHud()->changeTextureElement("player4", "assets/HUD/LobbyMenu/player4_notready.png");			
				}
			}
		}else if (!offline || offline_split && clientes.at(c).corredorJugador==true){
			if (c==0){
				TMotor::instancia().getActiveHud()->changeTextureElement("player1", "assets/HUD/LobbyMenu/player1.png");
			}else if (c==1){
				TMotor::instancia().getActiveHud()->changeTextureElement("player2", "assets/HUD/LobbyMenu/player2.png");
			}else if (c==2){
				TMotor::instancia().getActiveHud()->changeTextureElement("player3", "assets/HUD/LobbyMenu/player3.png");
			}else if (c==3){
				TMotor::instancia().getActiveHud()->changeTextureElement("player4", "assets/HUD/LobbyMenu/player4.png");			
			}
		}
	}
	if (clientes.size()==1){
		TMotor::instancia().getActiveHud()->changeTextureElement("player1", "assets/HUD/LobbyMenu/player1.png");	
		bc = false;
	}
	if (bc == false) {		//si todos los  clientes han aceptado
		//infoLobby->append(" [Ready] ");
	}
	else {		//si no han aceptado todos
		checkReady = false;
		checkReadyMe = false;
	}
	if (bc){
		if (clientes.at(id_player).ready){
			checkReadyMe = true;
			//infoLobby->append(" [Ready] ");
		}else{
			//infoLobby->append(" [Not Ready] ");
		}
	}
	
	if (checkReady){
		infoLobby->append("\n\n Everyone is ready.\n");// Press space to start the race
		TMotor::instancia().getActiveHud()->changeTextureElement("ReadyButton", "assets/HUD/LobbyMenu/start.png");
	}else{
		if (checkReadyMe){
			infoLobby->append("\n\n Waiting other players\n");// (Press space to cancel)
			TMotor::instancia().getActiveHud()->changeTextureElement("ReadyButton", "assets/HUD/LobbyMenu/ready_final.png");
		}else{
			infoLobby->append("\n\n Waiting players \n");//(Press space to be ready)
		}
	}
	
}

void EscenaLobby::crearFicheroIP(){
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

void EscenaLobby::mostrarTipoPersonaje(int i) {		//traduce de int a texto (tipo de personaje)
	std::string str;
	std::string str2;
	int check=-1;
	float inc=-0.52;
	if (offline && !offline_split && i==0){
		check=0;
		str="jugador1";
	}else if (offline_split || !offline){
		check=1;	
		switch (i){
			case 0:
				str="jugador1";
				str2="habilidades";
			break;
			case 1:
				str="jugador2";
				str2="habilidades2";
				inc=-0.12;
			break;
			case 2:
				str="jugador3";
				str2="habilidades3";
				inc=0.3;
			break;
			case 3:
				str="jugador4";
				str2="habilidades4";
				inc=0.7;
			break;
		}
	} 
	if (client->getClientes().at(i).corredorJugador){
		if (check==0){
			TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_vacia.png");
			if (client->getClientes().at(i).tipoCorredor == 0){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_gladiador.png");
				TMotor::instancia().getActiveHud()->changeTextureElement("habilidades", "assets/HUD/LobbyMenu/habilidades4.png");
			}else if (client->getClientes().at(i).tipoCorredor == 1){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_pirata.png");
				TMotor::instancia().getActiveHud()->changeTextureElement("habilidades", "assets/HUD/LobbyMenu/habilidades2.png");
			}else if (client->getClientes().at(i).tipoCorredor == 2){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_vikingo.png");
				TMotor::instancia().getActiveHud()->changeTextureElement("habilidades", "assets/HUD/LobbyMenu/habilidades3.png");
			}else if (client->getClientes().at(i).tipoCorredor == 3){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_chino.png");
				TMotor::instancia().getActiveHud()->changeTextureElement("habilidades", "assets/HUD/LobbyMenu/habilidades1.png");
			}
		}else if (check==1){
			TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_vacia.png");
			
			if (client->getClientes().at(i).tipoCorredor == 0){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_gladiador.png");
				TMotor::instancia().getActiveHud()->changeTextureElement(str2.c_str(), "assets/HUD/LobbyMenu/habilidades_gladiador.png");
			}else if (client->getClientes().at(i).tipoCorredor == 1){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_pirata.png");
				TMotor::instancia().getActiveHud()->changeTextureElement(str2.c_str(), "assets/HUD/LobbyMenu/habilidades_pirata.png");
			}else if (client->getClientes().at(i).tipoCorredor == 2){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_vikingo.png");
				TMotor::instancia().getActiveHud()->changeTextureElement(str2.c_str(), "assets/HUD/LobbyMenu/habilidades_vikingo.png");
			}else if (client->getClientes().at(i).tipoCorredor == 3){
				TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_chino.png");
				TMotor::instancia().getActiveHud()->changeTextureElement(str2.c_str(), "assets/HUD/LobbyMenu/habilidades_chino.png");
			}
			
			TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), inc, 0.26f);
		}
	}
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {
	
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
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (!pressed) {
			if (index_selected){
				index_selected=false;
				actualizarSelector();
			}else{
				end = true;
			}
			pressed=true;
		}
	}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_SPACE) == GLFW_PRESS && !selection_online) {
		//Space==Control de ready/Iniciar partida
		if (!pressed) {
			pressed = true;
			iniciarCarrera();
			 
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS && !selection_online) {
		//Left==change character (one side)
		if (!pressed) {
			pressed = true;
			if (index_selected){
				movimientoHorizontal(index,false);	
			}else{
				movimientoHorizontal(-1,false);
			}
		}

	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS && !selection_online) {
		//Right==change character (other side)
		if (!pressed) {
			pressed = true;
			if (index_selected){
				movimientoHorizontal(index,true);
			}else{	
				movimientoHorizontal(-1,false);
			}
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS) {
		if (!pressed){
			if(!selection_online && !offline) {
				iniciar = true;					//Conectar con el servidor de la IP
				
			}
			if (index_selected){
				if (index==4 && !offline_split){
					iniciarCarrera();
				}
				index_selected=false;
			}else if (!index_selected){
				index_selected=true;
				if (index==4 && !offline_split){
					iniciarCarrera();
				}
			}
			if (conectado || offline)
			actualizarSelector();
			pressed = true;
		}
		
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_3) == GLFW_PRESS && !selection_online) {
		//Online: Introducir texto | Offline: Cambiar jugador/IA
		if (!pressed) {
			if (offline_split) {
				int k=0;
				std::vector<structClientes> clientes = client->getClientes();
				for (int l=0;l<clientes.size();l++){
					if (clientes.at(l).corredorJugador==true){
						k++;
					}
				}
				
				if (clientes.size()>1){
					bool checkIA=false;
					if (k<4){
						for (int l=0;l<clientes.size();l++){
							if (clientes.at(l).corredorJugador==false){
								client->setArrayClients(clientes.at(l).ip, clientes.at(l).tipoCorredor, false, true, l);
								checkIA=true;
								break;
							}
						}
						if (!checkIA){
							if (clientes.size()<6)
							client->setArrayClients("", 3, false, true, -1);
						}
					}
				}else{
					if (clientes.size()<6 && k<4)
					client->setArrayClients("", 3, false, true, -1);
				}
			}
			pressed = true;
			
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_4) == GLFW_PRESS && !selection_online) {
		if (!pressed) {
			std::string str;
			std::string str2;
			std::vector<structClientes> clientes = client->getClientes();
			for (int l=clientes.size()-1;l>0;l--){
				if (clientes.at(l).corredorJugador==true){
					client->BorrarCliente(l);
					if (l==1){
						str="jugador2";
						str2="habilidades2";
					}else if(l==2){
						str="jugador3";
						str2="habilidades3";
					}else if(l==3){
						str="jugador4";
						str2="habilidades4";
					}
					TMotor::instancia().getActiveHud()->changeTextureElement(str.c_str(), "assets/HUD/LobbyMenu/seleccion_vacia.png");
					TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), -0.52, 0.26f);
					if (count > 0) {
						count--;
					}
					break;
				}
			}
			pressed = true;
		}
	}	
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS && !selection_online) {
		if (!pressed) {
			/*if (client->getClientes().size() - 1 > count) {
				count++;
			}
			else {
				count = 0;
			}*/
			if (!index_selected){
				movimientoVertical(false);
			}else if (index_selected && index==2 && !conectado && !offline_split || offline_split && index==4){
				movimientoVertical(true);
			}
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS && !selection_online) {
		//Moverse entre jugadores para cambiar opciones de seleccion en cada uno
		if (!pressed) {
			/*if (count > 0) {
				count--;
			}
			else {
				count = client->getClientes().size() - 1;
			}*/
			if (!index_selected){
				movimientoVertical(false);
			}else if (index_selected && index==2 && !conectado && !offline_split || offline_split && index==4){
				movimientoVertical(true);
			}
			pressed = true;
		}
	}
	else pressed = false;
	//textoUI->setText(this->texto.c_str());

	if (iniciado)
		if (client->getStarted()){
			//if (empiezaCarrera)
			return Escena::tipo_escena::ONLINE;
		}

	if (empiezaCarrera){
		if (index_mapa==1){
			Pista::getInstancia()->setNombreMapa("pirata");
		}else if (index_mapa==2){
			Pista::getInstancia()->setNombreMapa("gladiador");
		}
		//Pista::getInstancia()->setNombreMapa(str1.c_str());
		Pista::getInstancia()->setNumVueltas(laps);
		return Escena::tipo_escena::CARRERA;		//Iniciar la partida offline
		
	}

	return Escena::tipo_escena::LOBBY;
}

void EscenaLobby::movimientoHorizontal(int k,bool dcha){
	if (k==-1){
		if (index==1){
			if (offline && !offline_split){
				index=2;
			}else{
				if (dcha){
					index=4;
				}else{
					index=3;
				}
			}
		}else if (index==2){
			index=1;
		}else if (index==3){
			index=4;		
		}else{
			index=3;
		}
	}else{
		if (k==1){
			if (!dcha){
				if (conectado) {
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
			}else{
				if (conectado) {
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
		}else if (k==2 || index==4 && offline_split && index_selected){
			if (index_interno==2){
				if (dcha){
					if (laps<3)
					laps++;
				}else{
					if (laps>1)
					laps--;
					
				}
			}else{
				if (dcha){
					if (offline) {
						if (client->getClientes().size() < 6) {
							int r=0;
							if (client->getClientes().size()==2)
							r=2;
							if (client->getClientes().size()==3)
							r=3;
							if (client->getClientes().size()==4)
							r=1;
							if (client->getClientes().size()==5)
							r=2;

							client->setArrayClients("", 3-r, true, false, -1);
							//cout<<"client size: "<<client->getClientes().size()<<endl;
						}
					}
				}else{
					if (offline) {
						if (client->getClientes().size() > 4 && !offline_split || client->getClientes().size() > 1 && offline_split) {
							int l = client->getClientes().size() - 1;
							client->BorrarCliente(l);
							if (count > 0) {
								count--;
							}
						}

					}
				}
			}
		}else if (k==3){
			if (index_mapa==1){
				index_mapa=2;
				TMotor::instancia().getActiveHud()->changeTextureElement("Map", "assets/HUD/LobbyMenu/mapa_gladiador.png");
			}else{
				index_mapa=1;
				TMotor::instancia().getActiveHud()->changeTextureElement("Map", "assets/HUD/LobbyMenu/mapa_pirata.png");
			}
		}
	}
	actualizarSelector();
}
void EscenaLobby::movimientoVertical(bool interno){
	if (!interno){
		if (index==1){
			index=3;
			
		}else if (index==2){
			if (offline)
			index=4;

		}else if (index==3){
			index=1;

		}else{
			if (offline && !offline_split){
				index=2;
			}else{
				index=1;
			}

		}
	}else{

		if (offline){
			if (index_interno==1){
				index_interno=2;
			}else{
				index_interno=1;
			}
		}
	}
	actualizarSelector();
	
}

void EscenaLobby::actualizarSelector(){
	std::string str1="seleccion1";
	std::string str2="seleccion2";
	if (index_selected){
		TMotor::instancia().getActiveHud()->changeTextureElement("seleccion1","assets/HUD/LobbyMenu/seleccion1_activa.png");
		TMotor::instancia().getActiveHud()->changeTextureElement("seleccion2","assets/HUD/LobbyMenu/seleccion2_activa.png");

	}else{
		TMotor::instancia().getActiveHud()->changeTextureElement("seleccion1","assets/HUD/LobbyMenu/seleccion1.png");
		TMotor::instancia().getActiveHud()->changeTextureElement("seleccion2","assets/HUD/LobbyMenu/seleccion2.png");
	}
	TMotor::instancia().getActiveHud()->scaleElement(str1.c_str(), 1.0f, 1.0f);
	TMotor::instancia().getActiveHud()->scaleElement(str2.c_str(), 1.0f, 1.0f);
	if (index==3){
		TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), -0.83f, -0.5f);
		TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.03f, -0.5f);
		TMotor::instancia().getActiveHud()->scaleElement(str1.c_str(), 1.0f, 0.8f);
		TMotor::instancia().getActiveHud()->scaleElement(str2.c_str(), 1.0f, 0.8f);
		
	}else if (index==4){
		if (offline_split){
			if (index_selected){
				if (index_interno==1){
					TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  -0.2f);
					TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.9f,  -0.2f);
				}else{
					TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  -0.45f);
					TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.75f,  -0.45f);
				}
				TMotor::instancia().getActiveHud()->scaleElement(str1.c_str(), 0.5f, 0.3f);
				TMotor::instancia().getActiveHud()->scaleElement(str2.c_str(), 0.5f, 0.3f);
			}else{
				TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f, -0.4f);
				TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.74f, -0.4f);
			}

		}else{
			TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f, -0.53f);
			TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.68f, -0.53f);	
			TMotor::instancia().getActiveHud()->scaleElement(str1.c_str(), 0.8f, 0.8f);
		TMotor::instancia().getActiveHud()->scaleElement(str2.c_str(), 0.8f, 0.8f);	
		}
		
	}else if (index==1){
		if (!offline || offline_split){
			TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), -0.95f, 0.28f);
			TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.9f, 0.26f);
		}else{
			TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), -0.8f, 0.28f);
			TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.2f, 0.28f);
		}
	}else{
		
		if (index_selected){
			if (index_interno==1){
				TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  0.45f);
				TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.9f,  0.45f);
			}else{
				TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  0.18f);
				TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.75f,  0.18f);
			}
			TMotor::instancia().getActiveHud()->scaleElement(str1.c_str(), 0.5f, 0.3f);
			TMotor::instancia().getActiveHud()->scaleElement(str2.c_str(), 0.5f, 0.3f);
		}else{
			TMotor::instancia().getActiveHud()->traslateElement(str1.c_str(), 0.27f,  0.28f);
			TMotor::instancia().getActiveHud()->traslateElement(str2.c_str(), 0.9f,  0.28f);
		}
	}
}

void EscenaLobby::iniciarCarrera(){
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
			empiezaCarrera=true;
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

void EscenaLobby::crearHUD(){
	if (offline){
		if (!offline_split){
			TMotor::instancia().getActiveHud()->addElement(0.32f, 0.5f, "jugador1", "assets/HUD/LobbyMenu/seleccion_chino.png");
			TMotor::instancia().getActiveHud()->traslateElement("jugador1", -0.55f, 0.26f);
			TMotor::instancia().getActiveHud()->addElement(0.5f, 0.65f, "habilidades", "assets/HUD/LobbyMenu/habilidades1.png");	
			TMotor::instancia().getActiveHud()->traslateElement("habilidades", -0.1f, 0.26f);
			TMotor::instancia().getActiveHud()->addElement(0.38f, 0.08f, "PlayerText", "assets/HUD/LobbyMenu/player_select.png");
			TMotor::instancia().getActiveHud()->traslateElement("PlayerText", -0.56f, 0.63f);
		}
		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.14f, "AIText", "assets/HUD/LobbyMenu/ai_players.png");
		TMotor::instancia().getActiveHud()->traslateElement("AIText", 0.45f, 0.45f);
		TMotor::instancia().getActiveHud()->addElement(0.15f, 0.07f, "LapsText", "assets/HUD/LobbyMenu/laps.png");
		TMotor::instancia().getActiveHud()->traslateElement("LapsText", 0.38f, 0.2f);
		
	}
	TMotor::instancia().getActiveHud()->addElement(0.05f, 0.7f, "seleccion1", "assets/HUD/LobbyMenu/seleccion1.png");	
	TMotor::instancia().getActiveHud()->traslateElement("seleccion1", -0.8f, 0.28f);
	TMotor::instancia().getActiveHud()->addElement(0.05f, 0.7f, "seleccion2", "assets/HUD/LobbyMenu/seleccion2.png");	
	TMotor::instancia().getActiveHud()->traslateElement("seleccion2", 0.2f, 0.28f);
	TMotor::instancia().getActiveHud()->addElement(0.28f, 0.12f, "return", "assets/HUD/LobbyMenu/return.png");	
	TMotor::instancia().getActiveHud()->traslateElement("return", -0.8f, -0.9f);
	TMotor::instancia().getActiveHud()->addElement(0.5f, 0.15f, "RaceText", "assets/HUD/LobbyMenu/race_settings.png");
	TMotor::instancia().getActiveHud()->traslateElement("RaceText", 0.0f, 0.85f);
	TMotor::instancia().getActiveHud()->addElement(0.18f, 0.1f, "MapText", "assets/HUD/LobbyMenu/map.png");
	TMotor::instancia().getActiveHud()->traslateElement("MapText", -0.4f, -0.23f);
	TMotor::instancia().getActiveHud()->addElement(0.6f, 0.4f, "Map", "assets/HUD/LobbyMenu/mapa_pirata.png");
	TMotor::instancia().getActiveHud()->traslateElement("Map", -0.4f, -0.52f);
	TMotor::instancia().getActiveHud()->addElement(0.11f, 0.06f, "Arrow1", "assets/HUD/LobbyMenu/arrow.png");
	TMotor::instancia().getActiveHud()->traslateElement("Arrow1", -0.62f, -0.05f);
	TMotor::instancia().getActiveHud()->addElement(0.11f, 0.06f, "Arrow2", "assets/HUD/LobbyMenu/arrow.png");
	TMotor::instancia().getActiveHud()->traslateElement("Arrow2", -0.48f, -0.05f);
	TMotor::instancia().getActiveHud()->rotateElement("Arrow2", 180);
	TMotor::instancia().getActiveHud()->addElement(0.11f, 0.06f, "Arrow3", "assets/HUD/LobbyMenu/arrow.png");
	TMotor::instancia().getActiveHud()->traslateElement("Arrow3", -0.75f, -0.5f);
	TMotor::instancia().getActiveHud()->addElement(0.11f, 0.06f, "Arrow4", "assets/HUD/LobbyMenu/arrow.png");
	TMotor::instancia().getActiveHud()->traslateElement("Arrow4", -0.05f, -0.5f);
	TMotor::instancia().getActiveHud()->rotateElement("Arrow4", 180);

	TMotor::instancia().getActiveHud()->addElement(0.35f, 0.18f, "ReadyButton", "assets/HUD/LobbyMenu/ready.png");
	TMotor::instancia().getActiveHud()->traslateElement("ReadyButton", 0.46f, -0.5f);
	TMotor::instancia().getActiveHud()->addElement(0.28f, 0.12f, "enter", "assets/HUD/LobbyMenu/enter.png");
	TMotor::instancia().getActiveHud()->traslateElement("enter", -0.45f, -0.9f);
	
	if (offline_split || !offline){
		
		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.06f, "player1", "assets/HUD/LobbyMenu/player1_notready.png");
		TMotor::instancia().getActiveHud()->traslateElement("player1", -0.68f, 0.62f);
		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.06f, "player2", "assets/HUD/LobbyMenu/player2_notready.png");
		TMotor::instancia().getActiveHud()->traslateElement("player2", -0.2f, 0.62f);
		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.06f, "player3", "assets/HUD/LobbyMenu/player3_notready.png");
		TMotor::instancia().getActiveHud()->traslateElement("player3", 0.25f, 0.62f);
		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.06f, "player4", "assets/HUD/LobbyMenu/player4_notready.png");
		TMotor::instancia().getActiveHud()->traslateElement("player4", 0.68f, 0.62f);

		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.48f, "jugador2", "assets/HUD/LobbyMenu/seleccion_vacia.png");
		TMotor::instancia().getActiveHud()->traslateElement("jugador2", -0.15f, 0.3f);
		TMotor::instancia().getActiveHud()->addElement(0.28f, 0.6f, "habilidades2", "assets/HUD/LobbyMenu/habilidades_chino.png");	
		TMotor::instancia().getActiveHud()->traslateElement("habilidades2", -0.52f, 0.26f);
		
		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.48f, "jugador3", "assets/HUD/LobbyMenu/seleccion_vacia.png");
		TMotor::instancia().getActiveHud()->traslateElement("jugador3",0.25f, 0.3f);
		TMotor::instancia().getActiveHud()->addElement(0.28f, 0.6f, "habilidades3", "assets/HUD/LobbyMenu/habilidades_chino.png");	
		TMotor::instancia().getActiveHud()->traslateElement("habilidades3", -0.52f, 0.26f);

		TMotor::instancia().getActiveHud()->addElement(0.3f, 0.48f, "jugador4", "assets/HUD/LobbyMenu/seleccion_vacia.png");	
		TMotor::instancia().getActiveHud()->traslateElement("jugador4", 0.65f, 0.3f);
		TMotor::instancia().getActiveHud()->addElement(0.28f, 0.6f, "habilidades4", "assets/HUD/LobbyMenu/habilidades_chino.png");	
		TMotor::instancia().getActiveHud()->traslateElement("habilidades4", -0.52f, 0.26f);

		TMotor::instancia().getActiveHud()->addElement(0.25f, 0.43f, "jugador1", "assets/HUD/LobbyMenu/seleccion_vacia.png");
		TMotor::instancia().getActiveHud()->traslateElement("jugador1", -0.79f, 0.32f);
		TMotor::instancia().getActiveHud()->addElement(0.28f, 0.6f, "habilidades", "assets/HUD/LobbyMenu/habilidades_chino.png");	
		TMotor::instancia().getActiveHud()->traslateElement("habilidades", -0.52f, 0.26f);

		TMotor::instancia().getActiveHud()->traslateElement("seleccion1", -0.95f, 0.28f);
		TMotor::instancia().getActiveHud()->traslateElement("seleccion2", 0.9f, 0.26f);
		//TMotor::instancia().getActiveHud()->traslateElement("PlayerText", -0.67f, 0.8f);
		TMotor::instancia().getActiveHud()->traslateElement("Arrow1", -0.86f, 0.05f);
		TMotor::instancia().getActiveHud()->traslateElement("Arrow2", -0.72f, 0.05f);
		if (offline){
			TMotor::instancia().getActiveHud()->traslateElement("AIText", 0.45f, -0.3f);
			TMotor::instancia().getActiveHud()->traslateElement("LapsText", 0.38f, -0.5f);
			TMotor::instancia().getActiveHud()->traslateElement("ReadyButton", 0.46f, -0.8f);
		}
		
	
	}
}

std::string EscenaLobby::getIpConexion() { return ipConexion; }
