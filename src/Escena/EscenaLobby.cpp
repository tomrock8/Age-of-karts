#include "EscenaLobby.hpp"

EscenaLobby::EscenaLobby(Escena::tipo_escena tipo, std::string ipC) : Escena(tipo) {
	nElementos = 0;
	nElementos2 = 0;
	count = 3;
	iphost = "";
	ipConexion = ipC;
	cout << "EscenaLobby: " << ipConexion << endl;
	time = Timer::getInstancia();

	offline = false;
	selection = false;
	//Comprobamos que tipo de lobby entramos
	if (ipConexion.compare("") == 0) {
		//Online sin ip (a introducir)
		cout << "ESC - SALIR\n\n";
		cout << "1. Conectar con 127.0.0.1 \n2. Introducir ip manualmente\n";
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

		if (client->getClientes().size() == 0)
			client->setArrayClients("", 3, false, true, -1);
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
	
	
	TMotor::instancia().newHud("LobbyHUD");
	TMotor::instancia().getActiveHud()->addElement(1.0f, 1.0f, "comenzarPartida", "assets/HUD/LobbyMenu/empezar.png");
	/*
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
	Motor3d::instancia().getDriver()->makeColorKeyTexture(logoAOK, core::position2d<s32>(0, 0));*/
}
EscenaLobby::~EscenaLobby() {
	cout << "Destructor ESCENA LOBBY. Entro.";
	limpiar();
	cout << "Salgo.\n";
}

void EscenaLobby::init() {
}

void EscenaLobby::dibujar() {
	/*Motor3d::instancia().iniciarDibujado();

	//Motor3d::instancia().getDriver()->draw2DImage(logoAOK, rect<s32>(0, 0, 100, 100), 0, SColor(255, 255, 255, 255), true);
	Motor3d::instancia().getDriver()->draw2DImage(logoAOK, position2d<s32>(Motor3d::instancia().getAnchoPantalla() / 4, 0),
		core::rect<s32>(0, 0, logoAOK->getSize().Width, logoAOK->getSize().Height), 0,
		video::SColor(255, 255, 255, 255), true);

	Motor3d::instancia().dibujar();
	Motor3d::instancia().terminarDibujado();*/
}

void EscenaLobby::limpiar() {
	/*Motor3d::instancia().getScene()->clear();
	Motor3d::instancia().getGUI()->clear();*/
}

void EscenaLobby::update() {
	if (offline) {
		//si es offline solo cargamos y dibujamos toda la informacion (modificable con inputs)
		std::vector<structClientes> clientes = client->getClientes();
		/*texto="";
		for (int i=0;i<clientes.size();i++){
			mostrarInfoLobby(i);
		}*/
	}
	else {
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
				cout << "\nConectando";
				show = false;
			}

			if (time->getTimer() - nElementos2 == 1) {
				if (count <= 6) {
					cout << ".";
					count++;

				}
				else {

					cout << "\nConectando";
					count = 0;

				}
				nElementos2 = time->getTimer();
			}

			if (client->ReceivePackets() == 3) {			//en caso de saltar already connected, tratamos de actualizar la informacion a partir del servidor
				client->ActualizarClienteConectado();
			}
			if (client->ReceivePackets() == 1 || time->getTimer() - nElementos > 8) {			//si la respuesta es 1, significa que se ha producido un error (conexion, tiempo agotadod de servidor,etc). 
																						//O bien se ha acabado el tiempo para conectar
				//reseteamos parametros
				cout << "Conexion fallida! \nVuelve a introducir IP para iniciar partida online: ";
				iniciar = false;
				iniciado = false;
				ipConexion = "";
				client->ShutDownClient();
				nElementos = time->getTimer();
			}

			//si despues de todo se ha recibido conexion y cliente esta conectado mostramos informacion relativa
			if (client->getConnected()) {
				nElementos = time->getTimer();
				conectado = true;
				cout << "Conexion establecida!\n";

				mostrarInfoLobby(-1);

			}
		}
	}
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
	bool checkReadyMe = true;		//Para online. Boolean para comprobar si tu esta listo (teniendo en cuenta si estas solo o hay mas gente)
	bool bc = false;				//boolean aux

	//Diferenciamos informacion en funcion de online/offline
	if (offline) {
		if (id_player == 0) {
			cout << "\nNumero de jugadores: ";

			cout << to_string(clientes.size()).c_str();
			cout << " / 6";
		}
		//indicador de jugador actual seleccionado (a modificar)
		if (count != id_player) {
			if (clientes.at(id_player).corredorJugador) {
				cout << "\nCorredor Jugador ";
			}
			else {
				cout << "\nCorredor IA ";
			}
		}
		else {
			if (clientes.at(id_player).corredorJugador) {
				cout << "\n**Corredor Jugador ";
			}
			else {
				cout << "\n**Corredor IA ";
			}
		}
	}
	else {
		cout << "\nJugadores conectados: ";
		cout << to_string(client->getNumClients()).c_str();
		cout << " / ";
		cout << to_string(client->getMaxPlayers()).c_str();
		cout << "\nEres el Jugador ";
	}
	cout << to_string(id_player).c_str();

	if (id_player == 0 && !offline) {	//info solo para online
		cout << " (Host)";
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
				cout << " [Listo] ";
			}
			else {		//si no han aceptado todos
				checkReady = false;
				checkReadyMe = false;
				cout << " [No listo] ";
			}

		}
		else {		//si estas solo tu 
			cout << " [Listo] ";
		}
	}
	cout << "\n <-- Selecciona Personaje -->: ";

	if (id_player < size && id_player != -1)
		mostrarTipoPersonaje(id_player);

	cout << "\n";

	//recorremos todos los demas jugadores que estan en la lobby
	for (int i = 0; i < size; i++) {
		if (i != id_player) {
			if (!offline) {
				cout << "\nJugador ";
				cout << to_string(i).c_str();

				if (i == 0) {
					cout << " (Host)";
				}
			}
			if (clientes.at(i).ready == 0) {	//jugador no listo
				checkReady = false;
				if (!offline)
					cout << " [No listo] ";

			}
			else {	//jugador listo
				if (!offline)
					cout << " [Listo] ";
			}
			if (!offline) {
				cout << " - Personaje: ";
				mostrarTipoPersonaje(i);
			}

		}
	}


	// Parte en la que se comprueba la situacion antes de empezar la apartida
	if (id_player == 0 && !offline || offline && indice == clientes.size() - 1) {	//host
		if (checkReady) {
			if (offline)
				//iniciar = true;		//si todos estan ready se puede iniciar partida offline

				cout << "\n\n Todos listos. Pulse espacio para iniciar la partida\n";
		}
		else {
			if (offline)
				iniciar = false;	//si no todos estan ready, no se puede iniciar partida offline

			if (checkReadyMe) {
				cout << "\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n";
			}
			else {
				if (offline) {
					cout << "\n\n Esperando a los demas jugadores (Pulse espacio para iniciar la partida)\n";

				}
				else {
					cout << "\n\n Pulsa espacio para indicar que estas listo\n";

				}
			}
		}

	}
	else if (!offline) {	//online
		if (checkReadyMe) {
			cout << "\n\n Esperando a los demas jugadores (Pulsa espacio para cancelar)\n";
		}
		else {
			cout << "\n\n Pulsa espacio para indicar que estas listo\n";
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
				cout << "\n\nIP conexion: ";
				while (getline(myfile, line)) {
					cont_line++;
					if (cont_line == 3) {
						iphost = line;
						cout << iphost.c_str();
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
		cout << "\n\nIP conexion: ";
		cout << iphost.c_str();
	}
}


void EscenaLobby::mostrarTipoPersonaje(int i) {		//traduce de int a texto (tipo de personaje)
	if (client->getClientes().at(i).tipoCorredor == 0) {
		cout << "GLADIADOR ";
	}
	else if (client->getClientes().at(i).tipoCorredor == 1) {
		cout << "PIRATA ";
	}
	else if (client->getClientes().at(i).tipoCorredor == 2) {
		cout << "VIKINGO ";
	}
	else if (client->getClientes().at(i).tipoCorredor == 3) {
		cout << "GUERRERO CHINO ";
	}
}

Escena::tipo_escena EscenaLobby::comprobarInputs() {


	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
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
				if (iniciar && clientes.size() > 0 || clientes.size() == 1)
					return Escena::tipo_escena::CARRERA;		//Iniciar la partida offline
			}
			else if (offline) {
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
			if (!offline) {
				if (!selection) {
					cout << "1";
					ipConexion += "1";
				}
				else {
					selection = false;
					iniciar = true;
					ipConexion = "127.0.0.1";
				}
			}
			else {
				if (client->getClientes().size() < 6) {
					client->setArrayClients("", 3, true, false, -1);
					//cout<<"client size: "<<client->getClientes().size()<<endl;
				}

			}
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_2) == GLFW_PRESS) {
		//Online: Selccionar opcion/introducir texto | Borrar clientes offline
		if (!pressed) {
			if (!offline) {
				if (!selection) {
					cout << "2";
					ipConexion += "2";
				}
				else {
					selection = false;
					cout << "Introduce IP para iniciar partida online: ";
				}
			}
			else {
				if (client->getClientes().size() > 1) {
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
			if (!offline) {
				cout << "3";
				ipConexion += "3";

			}
			else {
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
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_4) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << "4";
			ipConexion += "4";
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_5) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << "5";
			ipConexion += "5";
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_6) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << "6";
			ipConexion += "6";
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_7) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << "7";
			ipConexion += "7";
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_8) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << "8";
			ipConexion += "8";
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_9) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << "9";
			ipConexion += "9";
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_0) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << "0";
			ipConexion += "0";
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_PERIOD) == GLFW_PRESS && !selection && !offline) {
		if (!pressed) {
			cout << ".";
			ipConexion += ".";
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

std::string EscenaLobby::getIpConexion() {
	return ipConexion;
}
