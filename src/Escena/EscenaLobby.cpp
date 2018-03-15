#include "EscenaLobby.hpp"

EscenaLobby::EscenaLobby(Escena::tipo_escena tipo, std::string ipC) : Escena(tipo) {
	nElementos = 0;
	nElementos2 = 0;
	count = 3;
	ipConexion = ipC;
    cout<<"EscenaLobby: "<<ipConexion<<endl;
	time= Timer::getInstancia();
	texto2="";
	offline=false;
	selection=false;
	if (ipConexion.compare("")==0){	
		texto = L"ESC - SALIR\n\n";
		//texto = "Introduce IP para iniciar partida online: ";
		texto += "1. Crear Sala\n 2. Unirse\n";
		selection=true;
        iniciar = false;
		iniciado = false;
		firstInit = true;
	}else if (ipConexion.compare("offline")==0){
		offline=true;
		client = Client::getInstancia();
		count=0;
		if (client->getClientes().size()==0)
		client->setArrayClients("",3,false,-1);
    }else{
		iniciar = true;
		iniciado = false;
		firstInit = false;
	}
        	
	pressed = true;
	lanzado = false;
	conectado = false;
	
	
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
	if (offline){
		vector<structClientes> clientes=client->getClientes();
		//cout<<"SIZE: "<<clientes.size()<<endl;
		texto="";
		for (int i=0;i<clientes.size();i++){
			mostrarInfoLobby(i);
		}
	}else{
		bool show=false;

		if (iniciar && !iniciado) {
			show=true;
			count=0;
			texto2="";
			nElementos2=time->getTimer();
			nElementos=time->getTimer();
			if (firstInit){
				client = Client::getInstancia();
				client->CreateClientInterface();
				client->SetIP(ipConexion);
				client->ClientStartup();
				
			}else{
				client = Client::getInstancia();	
				client->setNetloaded(false);
			}
			iniciado = true;
		}
		if (iniciado) {
			
			if (show){
				texto += "\nConectando";
				show=false;
			}
			//cout<<"timer: "<<time->getTimer()-nElementos2<<endl;
			if (time->getTimer()-nElementos2==1){
				if (count<=6){
					texto2 +=".";
					count++;
					
				}else{
					texto2="";
					texto = "\nConectando";
					count=0;
				
				}
				nElementos2=time->getTimer();
				//time->restartTimer();
			}
			
			texto += texto2;
			texto2="";
			if (client->ReceivePackets()==3){
				cout<<"voy a entrar\n";
				client->ActualizarClienteConectado();
			}
			if (client->ReceivePackets()==1 || time->getTimer()-nElementos>8){
				texto = "Conexion fallida! \nVuelve a introducir IP para iniciar partida online: ";
				iniciar = false;
				iniciado = false;
				ipConexion="";
				texto2="";
				client->ShutDownClient();
				nElementos=time->getTimer();
			}
			
			if (client->getConnected()) {
				nElementos=time->getTimer();
				conectado = true;
				texto = L"Conexion establecida!\n";
				
				mostrarInfoLobby(-1);
				
			}
		}
	}
}
void EscenaLobby::mostrarInfoLobby(int indice){
	vector<structClientes> clientes=client->getClientes();
	int size=clientes.size();
	int id_player;
	if (offline){
		id_player=indice;
	}else{
		id_player=client->getControlPlayer();
	}
	bool checkReady=true;
	bool checkReadyMe=true;
	bool bc=false;
	if (offline){
		if (indice==0){
			texto += "\nNumero de jugadores: ";

			texto += to_string(clientes.size()).c_str(); 
			texto += " / 6" ;
		}
		if (count!=indice){
			texto += "\nJugador ";
		}else{
			texto += "\n**Jugador ";
		}
	}else{
		texto += "\nJugadores conectados: ";
	
		texto += to_string(client->getNumClients()).c_str(); 
		texto += " / " ;
		texto += to_string(client->getMaxPlayers()).c_str();
		texto += "\nEres el Jugador ";
	}
	texto += to_string(id_player).c_str();

	if (id_player==0 && !offline){
		texto += " (Host)";
	}
	
	if (id_player < clientes.size()){		//comprobamos que la id del jugador no supere el tamnyo del vector de ready
		if (clientes.at(id_player).ready==false && clientes.size() > 1){		//si estas no ready y hay mas jugadores 
			for (int c=0;c<clientes.size();c++){
				if (clientes.at(c).ready==0){	//comprobamos si han aceptado todos
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
			if (!offline){
				texto += "\nJugador ";
				texto += to_string(i).c_str();
				
				if (i==0){
					texto += " (Host)";
				}
			}
			if (clientes.at(i).ready==0){	//jugador no listo
				checkReady=false;
				if (!offline)
				texto += " [No listo] ";
				
			}else{	//jugador listo
				if (!offline)
				texto += " [Listo] ";
				
			}
			if (!offline){
				texto += " - Personaje: ";
				mostrarTipoPersonaje(i);
			}
			
		}
	}
	
	
	// Parte en la que se comprueba la situacion antes de empezar la apartida
	if (id_player==0 && !offline || offline && indice==clientes.size()-1 ){	//host
		if (checkReady){
			if (offline)
			iniciar = true;	

			texto += "\n\n Todos listos. Pulse espacio para iniciar la partida\n";
		}else{
			if (offline)
			iniciar = false;	

			if (checkReadyMe){
				texto += "\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n";
			}else{
				if (offline){
					texto += "\n\n Esperando a los demas jugadores (Pulse espacio para iniciar la partida)\n";

				}else{
					texto += "\n\n Pulsa espacio para indicar que estas listo\n";
					
				}
			}
		}
		
	}else if (!offline){	//no host
		if (checkReadyMe){
			texto += "\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n";
		}else{
			texto += "\n\n Pulsa espacio para indicar que estas listo\n";	
		}
		
	}
	
}


void EscenaLobby::mostrarTipoPersonaje(int i){		//traduce de int a texto (tipo de personaje)
	if (client->getClientes().at(i).tipoCorredor == 0){
		texto += "GLADIADOR ";	
	}else if (client->getClientes().at(i).tipoCorredor == 1){
		texto += "PIRATA ";
	}else if (client->getClientes().at(i).tipoCorredor == 2){
		texto += "VIKINGO ";
	}else if (client->getClientes().at(i).tipoCorredor == 3){
		texto += "GUERRERO CHINO ";
	}
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {
	
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		if(conectado)
			client->ShutDownClient();
		
		if (offline)
				client->BorrarClientes();
		
		return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !selection) {
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en espacio\n";
				client->RaceStart();
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
			if (offline && iniciar){
				return Escena::tipo_escena::CARRERA;
			}else if (offline){
				vector<structClientes> clientes=client->getClientes();
				int k=count;
				if (clientes.at(k).ready==false){
					client->setArrayClients(clientes.at(k).ip,clientes.at(k).tipoCorredor,true,k);
					//clientes.at(k).ready=true;
				}else{
					client->setArrayClients(clientes.at(k).ip,clientes.at(k).tipoCorredor,false,k);
					//clientes.at(k).ready=false;
				}
			}
		}
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !selection){
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en cambiar jugador izquierda\n";
				client->ChangeCharacter(false);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
			if (offline){
				vector<structClientes> clientes=client->getClientes();
				int k=count;
				if (clientes.at(k).tipoCorredor==0){
					client->setArrayClients(clientes.at(k).ip,3,clientes.at(k).ready,k);
				}else{
					client->setArrayClients(clientes.at(k).ip,clientes.at(k).tipoCorredor-1,clientes.at(k).ready,k);
				}
			}
		}

	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !selection){
		if (!pressed) {
			pressed = true;
			if (conectado) {
				cout << "Entro en cambiar jugador derecha\n";
				client->ChangeCharacter(true);
				//return Escena::tipo_escena::ONLINE;		//Iniciar la partida
			}
			if (offline){
				vector<structClientes> clientes=client->getClientes();
				int k=count;
				if (clientes.at(k).tipoCorredor==3){
					client->setArrayClients(clientes.at(k).ip,0,clientes.at(k).ready,k);
				}else{
					client->setArrayClients(clientes.at(k).ip,clientes.at(k).tipoCorredor+1,clientes.at(k).ready,k);
				}
			}
		}
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !selection && !offline){
		if(!pressed){
			iniciar = true;					//Conectar con el servidor de la IP
			pressed = true;
		}
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
		if (!pressed) {
			if (!offline){
				if (!selection){
				texto += "1";
				ipConexion += "1";
				}else{
					selection=false;
					iniciar = true;
					ipConexion="127.0.0.1";
				}
			}else{
				if (client->getClientes().size()<6){
					client->setArrayClients("",3,true,-1);
					//cout<<"client size: "<<client->getClientes().size()<<endl;
				}
				
			}
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
		if (!pressed) {
			if (!offline){
				if (!selection){
				texto += "2";
				ipConexion += "2";
				}else{
					selection=false;
					texto = "Introduce IP para iniciar partida online: ";
				}
			}else{
				if (client->getClientes().size()>1){
					int l=client->getClientes().size()-1;
					client->BorrarCliente(l);
				}
				
			}
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && !selection && !offline){
		if (!pressed) {
			texto += "3";
			ipConexion += "3";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && !selection && !offline){
		if (!pressed) {
			texto += "4";
			ipConexion += "4";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) && !selection && !offline){
		if (!pressed) {
			texto += "5";
			ipConexion += "5";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && !selection && !offline){
		if (!pressed) {
			texto += "6";
			ipConexion += "6";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) && !selection && !offline){
		if (!pressed) {
			texto += "7";
			ipConexion += "7";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) && !selection && !offline){
		if (!pressed) {
			texto += "8";
			ipConexion += "8";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9) && !selection && !offline){
		if (!pressed) {
			texto += "9";
			ipConexion += "9";
			pressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) && !selection && !offline){
		if (!pressed) {
			texto += "0";
			ipConexion += "0";
			pressed = true;
		}
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Period) && !selection && !offline){
		if(!pressed){
			texto += ".";
			ipConexion += ".";
			pressed=true;
		}
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !selection && offline){
		if(!pressed){
			if (client->getClientes().size()-1>count){
				count++;
			}else{
				count=0;
			}
			pressed=true;
		}
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !selection && offline){
		if(!pressed){
			if (count>0){
				count--;
			}else{
				count=client->getClientes().size()-1;
			}
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
