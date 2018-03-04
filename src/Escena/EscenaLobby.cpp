#include "EscenaLobby.hpp"

EscenaLobby::EscenaLobby() : Escena(Escena::tipo_escena::LOBBY) {
	nElementos = 0;
	count = 50;
	pressed = true;
	iniciar = false;
	iniciado = false;
	lanzado = false;
	conectado = false;
	ipConexion = "";

	texto = L"ESC - SALIR\n\n";
	texto = "Introduce IP para iniciar partida online: ";


	u16 xPos = Motor3d::instancia().getAnchoPantalla() / 3;
	u16 yPos = Motor3d::instancia().getAltoPantalla() / 4;

	fuente = Motor3d::instancia().getGUI()->getFont("assets/fuente.bmp");
	textoUI = Motor3d::instancia().getGUI()->addStaticText(
		texto.c_str(),				 // Texto
		rect<s32>(xPos, 2 * yPos, 2 * xPos, 3 * yPos), // Rectangulo de los bordes
		false,						 // Mostrar bordes
		true,						 // Cortar en varias lineas
		0,							 // Nodo padre
		0,							 // Id del elemento
		false);						 // Rellenado (o transparente)
	textoUI->setOverrideFont(fuente);

	logoAOK = Motor3d::instancia().getDriver()->getTexture("assets/logoAOK.png");
	Motor3d::instancia().getDriver()->makeColorKeyTexture(logoAOK, core::position2d<s32>(0, 0));
}

EscenaLobby::~EscenaLobby() {
	cout << "Destructor ESCENA LOBBY. Entro.";
	limpiar();
	cout << "Salgo.\n";
}

void EscenaLobby::init() {
}

void EscenaLobby::dibujar() {
	Motor3d::instancia().iniciarDibujado();

	//Motor3d::instancia().getDriver()->draw2DImage(logoAOK, rect<s32>(0, 0, 100, 100), 0, SColor(255, 255, 255, 255), true);
	Motor3d::instancia().getDriver()->draw2DImage(logoAOK, position2d<s32>(Motor3d::instancia().getAnchoPantalla() / 4, 0),
		core::rect<s32>(0, 0, logoAOK->getSize().Width, logoAOK->getSize().Height), 0,
		video::SColor(255, 255, 255, 255), true);

	Motor3d::instancia().dibujar();
	Motor3d::instancia().terminarDibujado();
}

void EscenaLobby::limpiar() {
	Motor3d::instancia().getScene()->clear();
	Motor3d::instancia().getGUI()->clear();
}

void EscenaLobby::update() {
	if (iniciar && !iniciado) {
		client = Client::getInstancia();
		client->CreateClientInterface();
		client->SetIP(ipConexion);
		client->ClientStartup();
		iniciado = true;
	}
	if (iniciado) {
		client->ReceivePackets();

		bool checkReady=true;
		bool checkReadyMe=true;
		bool bc=false;
		int size=client->getArrayTipoCorredor().size();
		vector<int> array=client->getArrayTipoCorredor();
		int id_player=client->getControlPlayer();


		if (client->getConnected()) {
			conectado = true;
			texto = L"Conexion establecida!\n";
			 
			texto += "\nJugadores conectados: ";
			texto += to_string(client->getNumClients()).c_str(); 
			texto += " / " ;
			texto += to_string(client->getMaxPlayers()).c_str();
			texto += "\nEres el Jugador ";
			texto += to_string(id_player).c_str();
		
			if (id_player==0){
				texto += " (Host)";
			}
			
			if (id_player<client->getArrayReady().size()){		//comprobamos que la id del jugador no supere el tamnyo del vector de ready
				if (client->getArrayReady().at(id_player)==0 && client->getArrayReady().size()>1){		//si estas no ready y hay mas jugadores 
					for (int c=0;c<client->getArrayReady().size();c++){
						if (client->getArrayReady().at(c)==0){	//comprobamos si han aceptado todos
							bc=true;
							break;
						}	
					}
					if (bc==false){		//si todos los  clientes han aceptado
						texto += " [Listo] ";
					}else{		//si no han aceptado todos
						checkReady=false;
						checkReadyMe=false;
						texto += " [No listo] ";
					}
					
				}else{		///si estas solo tu 
					texto += " [Listo] ";
				}
			}
			texto += "\n <-- Selecciona Personaje -->: " ;
			
			if (id_player<size && id_player!=-1)
			mostrarTipoPersonaje(id_player);
			
			texto += "\n";
			//recorremos todos los demas jugadores que estan en la lobby
			for (int i=0;i<size;i++){
				if (i!=id_player){
					texto += "\nJugador ";
					texto += to_string(i).c_str();
					if (i==0){
						texto += " (Host)";
					}
					if (client->getArrayReady().at(i)==0){	//jugador no listo
						checkReady=false;
						texto += " [No listo] ";
					}else{	//jugador listo
						texto += " [Listo] ";
					}
					texto += " - Personaje: ";
					mostrarTipoPersonaje(i);
					
				}
			}
			// Parte en la que se comprueba la situacion antes de empezar la apartida
			if (id_player==0){	//host
				if (checkReady){
					texto += "\n\n Todos listos. Pulse espacio para iniciar la partida\n";
				}else{
					if (checkReadyMe){
						texto += "\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n";
					}else{
						texto += "\n\n Pulsa espacio para indicar que estas listo\n";
					}
				}
				
			}else{	//no host
				if (checkReadyMe){
					texto += "\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n";
				}else{
					texto += "\n\n Pulsa espacio para indicar que estas listo\n";	
				}
				
			}
		}
	}

}
void EscenaLobby::mostrarTipoPersonaje(int i){		//traduce de int a texto (tipo de personaje)
	if (client->getArrayTipoCorredor().at(i)==0){
		texto += "GLADIADOR ";	
	}else if (client->getArrayTipoCorredor().at(i)==1){
		texto += "PIRATA ";
	}else if (client->getArrayTipoCorredor().at(i)==2){
		texto += "VIKINGO ";
	}else if (client->getArrayTipoCorredor().at(i)==3){
		texto += "GUERRERO CHINO ";
	}
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		if(conectado)
			client->ShutDownClient();

		return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en espacio\n";
				client->RaceStart();
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
		}
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en cambiar jugador izquierda\n";
				client->ChangeCharacter(false);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
		}

	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en cambiar jugador derecha\n";
				client->ChangeCharacter(true);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
		}
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
		if(!pressed){
			iniciar = true;					//Conectar con el servidor de la IP
			pressed = true;
		}
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
		if (!pressed) {
			texto += "1";
			ipConexion += "1";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
		if (!pressed) {
			texto += "2";
			ipConexion += "2";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
		if (!pressed) {
			texto += "3";
			ipConexion += "3";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){
		if (!pressed) {
			texto += "4";
			ipConexion += "4";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)){
		if (!pressed) {
			texto += "5";
			ipConexion += "5";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)){
		if (!pressed) {
			texto += "6";
			ipConexion += "6";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)){
		if (!pressed) {
			texto += "7";
			ipConexion += "7";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)){
		if (!pressed) {
			texto += "8";
			ipConexion += "8";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)){
		if (!pressed) {
			texto += "9";
			ipConexion += "9";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){
		if (!pressed) {
			texto += "0";
			ipConexion += "0";
			pressed = true;
		}
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Period)){
		if(!pressed){
			texto += ".";
			ipConexion += ".";
			pressed=true;
		}
	}else pressed = false;
		textoUI->setText(this->texto.c_str());

	if(iniciado)
		if(client->getStarted())
			return Escena::tipo_escena::ONLINE;
			
	return Escena::tipo_escena::LOBBY;
}

std::string EscenaLobby::getIpConexion() {
	return ipConexion;
}
