#include "Client.hpp"
#include "NetworkEnums.hpp"
#include <iostream>

#define DEBUG_BUILD

Client *Client::instancia = NULL;

//==================================================================================
// constructor que inicializa los valores de las variables requeridad por el cliente
//==================================================================================
Client::Client(int maxPlay)
{
	maxPlayers = maxPlay;
	serverPort = "6001"; //puerto de escucha del servidor a conectarse
	numPlayers = 0;
	numClients = 0;
	spawned = false;   // true == el jugador del cliente ya ha spwaneado
	netLoaded = false; // true == si todos los clientes de la partida han spwaneado
	connected = false;
	started = false;
	pressed = true;
}

//==================================================================================
// coger instancia del singleton
//==================================================================================

Client *Client::getInstancia() {
	if (instancia == NULL)
		instancia = new Client(4);

	return instancia;
}

Client::~Client(){

}

//==================================================================================
// se crea el cliente obteniendo la instancia de la clase RakPeerInterface
//==================================================================================
void Client::CreateClientInterface()
{
	//se recoge la instancia del cliente del mismo modo que se hace en la parte del servidor
	client = RakNet::RakPeerInterface::GetInstance();

	//sino se ha podido crear el cliente, se muestra un mensaje de error por pantalla
	if (!client)
	{
		std::cout << "Error N02: Failed to Create Client Interface\n";
	}

	RakNet::SystemAddress clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
}

//==================================================================================
// se establece la IP del servidor y el puerto de escucha del cliente
//==================================================================================
void Client::SetIP(std::string ipConexion) {
	//se le pide al usuario que introduzca la IP del servidor al que quiera conectarse
	//std::cout << "Introduce IP del servidor: ";
	//std::cin >> serverIP;
	//serverIP = "127.0.0.1";
	serverIP = ipConexion;
	//puerto de escucha del cliente
	clientPort = "6003";
}

//==================================================================================
// se realiza la conexion del cliente con el servidor segun IP y puerto
//==================================================================================
void Client::ClientStartup()
{
	//de la misma forma que para el servidor, se describe el tipo de socket que se va a usar
	//en funcion del puerto y del protocolo (IPv4) utilizado
	socketDescriptor.port = atoi(clientPort.c_str());
	socketDescriptor.socketFamily = AF_INET;

	//se inicia el cliente especificando el numero de conexiones permitidas, la descripcion de los sockets
	//y el numero de descriptores pasados
	client->Startup(8, &socketDescriptor, 1);

	//true == se realizan pings de manera constante para comprobar el buen funcionamiento de la conexion
	client->SetOccasionalPing(true);

	//se conecta con el servidor detallando la IP, el puerto, la contrasenya y la longitud de esta ultima
	RakNet::ConnectionAttemptResult car = client->Connect(serverIP.c_str(), atoi(serverPort.c_str()), "ageofkarts", (int)strlen("ageofkarts"));

	//se comprueba que la conexion se ha realizado correctamente, en caso contrario se aborta la conexion y se muestra
	//un mensaje de "connection attempt failed" por terminal
	RakAssert(car == RakNet::CONNECTION_ATTEMPT_STARTED);

	//si todo el proceso tiene exito, se avisa al usuario de que el cliente se ha creado y conectado al servidor
	std::cout << "Cliente creado!\n";
	arrayTipoCorredor.push_back(3);
}

void Client::UpdateNetworkKeyboard()
{

	if (netLoaded)
	{
		int estadoMovimiento = 0;
		int direccionMovimiento = 0;
		bool reset = false;
		bool lanzar = false;
		typeID = ID_SEND_KEY_PRESS;
		RakNet::BitStream bsOut;
		bsOut.Write(typeID);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { // Acelera
			//if (!pressed) {
				pressed = true;
				estadoMovimiento = 1;
			//}
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ //Frena
			//if(!pressed){
				pressed = true;
				estadoMovimiento = 2;
			//}
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){ //Freno de mano
			//if(!pressed){
				pressed = true;
				estadoMovimiento = 4;
			//}
		} else {
			estadoMovimiento = 5;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ //Giro izq
			//if(!pressed){
				pressed = true;
				direccionMovimiento = 1;
			//}
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ //Giro dcha
			//if(!pressed){
				pressed = true;
				direccionMovimiento = 2;
			//}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){ //ResetWaypoint
			reset = true;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){ //Lanzar Item
			lanzar = true;
		}

		bsOut.Write(controlPlayer);
		bsOut.Write(estadoMovimiento);
		bsOut.Write(direccionMovimiento);
		bsOut.Write(reset);
		bsOut.Write(lanzar);
		//if(pressed)
		client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	}else{
		
	}
}

void Client::RaceStart() {
	std::cout << "Mandando iniciar carrera\n";

	typeID = ID_RACE_START;
	RakNet::BitStream bsOut;
	bsOut.Write(typeID);
	bsOut.Write(controlPlayer);

	client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
void Client::ChangeCharacter(bool i) {
	std::cout << "Cambiando personaje\n";
	
	typeID = ID_CHANGE_CHARACTER;
	RakNet::BitStream bsOut;
	bsOut.Write(typeID);
	bsOut.Write(controlPlayer);
	bsOut.Write(i);	//false == izquierda ;; true == derecha

	client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
//==================================================================================================================
// funcion que recoge los paquetes recibidos por el cliente y realiza unas acciones u otras segun su identificador
//==================================================================================================================
int Client::ReceivePackets()
{
	//cout <<"me gustar ver la vida pasar" << endl;
	GestorJugadores *jugadores = GestorJugadores::getInstancia();
	//bucle donde se reciben los distintos paquetes, se tratan y se deasignan
	for (p = client->Receive(); p; client->DeallocatePacket(p), p = client->Receive())
	{
		players = jugadores->getJugadores();
		Corredor *playerAux;
		//se coge el identificador del paquete recibido para los casos del switch
		packetIdentifier = GetPacketIdentifier(p);

		//se inicializan el tipo de RakNet para los mensajes de entrada y salida
		RakNet::BitStream bsIn(p->data, p->length, false); //|mensaje de entrada
		RakNet::BitStream bsOut;						   //|mensaje de salida

		//vector de posicion de Irrlicht
		btVector3 posicion;
		btVector3 pos2[6];
		pos2[0].setX(-10);
		pos2[0].setY(0);
		pos2[0].setZ(310);
		pos2[1].setX(-10);
		pos2[1].setY(0);
		pos2[1].setZ(290);
		pos2[2].setX(-20);
		pos2[2].setY(0);
		pos2[2].setZ(310);
		pos2[3].setX(-20);
		pos2[3].setY(0);
		pos2[3].setZ(290);
		pos2[4].setX(-30);
		pos2[4].setY(0);
		pos2[4].setZ(310);
		pos2[5].setX(-30);
		pos2[5].setY(0);
		pos2[5].setZ(290);
		btVector3 pos;
		float x,y,z;
		int id;
		int param;
		int param2;
		bool reset = false;
		bool lanzar = false;
		bool parambool;


		//switch para comprobar el tipo de paquete recibido
		switch (packetIdentifier)
		{

		//uno de los clientes se ha desconectado del servidor
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "ID_DISCONNECTION_NOTIFICATION de " << p->systemAddress.ToString(true) << std::endl;
			break;

		//el cliente ya esta conectado (en caso de realizar un connect)
		case ID_ALREADY_CONNECTED:
			std::cout << "ID_ALREADY_CONNECTED\n";
			break;

		//un nuevo cliente se ha conectado al servidor
		case ID_NEW_INCOMING_CONNECTION:
			std::cout << "ID_NEWServer_INCOMING_CONNECTION\n";

			break;

		//cliente y servidor no comparten el mismo tipo de protocolo (IPv4-IPv6)
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			std::cout << "ID_INCOMPATIBLE_PROTOCOL_VERSION\n";
			break;

		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "ID_REMOTE_DISCONNECTION_NOTIFICATION\n";
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			std::cout << "ID_REMOTE_NEW_INCOMING_CONNECTION\n";
			break;

		//se le notifica al cliente que ha sido expulsado del servidor por x motivos
		case ID_CONNECTION_BANNED:
			std::cout << "Has sido baneado del server!\n";
			break;

		//la conexion del cliente con el servidor ha fallado
		case ID_CONNECTION_ATTEMPT_FAILED:
			std::cout << "La conexion no ha tenido exito!\n";
			return 1;
			break;

		//se ha perdido la conexion con uno de los clientes
		case ID_CONNECTION_LOST:
			std::cout << "ID_CONNECTION_LOST de " << p->systemAddress.ToString(true) << std::endl;
			return 2;
			break;

		//se le notifica al cliente que el servidor ya ha alcanzado su numero maximo de usuarios y, por lo tanto, no puede acceder
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "Lo sentimos, el servidor ya esta lleno\n";
			break;

		//el cliente ha proporcionado una contrasenya de acceso incorrecta
		case ID_INVALID_PASSWORD:
			std::cout << "Acceso denegado! Introduzca la clave correcta\n";
			break;

		//se ha aceptado la conexion con un cliente
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Tu conexion ha sido aceptada a " << p->systemAddress.ToString(true) << " con GUID " << p->guid.ToString() << std::endl;
			connected = true;
			controlPlayer = -1;

			break;
		case ID_CHANGE_CHARACTER:
			std::cout<<"ID_CHANGE_CHARACTER_CLIENT\n";
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);
			bsIn.Read(parambool);
			param=arrayTipoCorredor.at(id);	//a partir de ahora param es el tipo de jugador
			if (param==0 && parambool==false){
				param=4;
			}else if (param==4 && parambool==true){
				param=0;
			}else if (parambool){
				param++;
			}else{
				param--;
			}
			if (id!=-1)
			arrayTipoCorredor.at(id)=param;

			break;
		case ID_RACE_START:
			cout << "ID_RACE_START\n";
			started = true;
			break;

		case ID_SEND_KEY_PRESS:
			//cout << "ID_PRESS_KEY_PRESS\n";

			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);
			bsIn.Read(param);
			bsIn.Read(param2);
			bsIn.Read(reset);
			bsIn.Read(lanzar);
        	players.at(id)->getEstados()->setEstadoMovimiento(param);
			players.at(id)->getEstados()->setDireccionMovimiento(param2);
			if(reset){
				players.at(id)->recolocarWaypoint();
			}
			if(lanzar){
				if(players.at(id)->getTipoObj() != 0)
					players.at(id)->usarObjetos();
			}
			break;

		case ID_SPAWN_PLAYER:
			cout <<"ID_SPAWN_PLAYER\n";
			int id;
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(x);
			bsIn.Read(y);
			bsIn.Read(z);
			bsIn.Read(id);
			bsIn.Read(playerNetworkID);
			std::cout << "Creando jugador en pos: " << x <<" - "<< y <<" - "<< z <<" - "<< std::endl; 
			pos.setX(x);
			pos.setY(y);
			pos.setZ(z);
			playerAux = new CorredorRed("assets/coche.obj", pos, Corredor::tipo_jugador::GLADIADOR);
			playerAux->getNodo()->setID(id);
			players.push_back(playerAux);

			//player[numPlayers]->setPosition(posicion);
			players.at(numPlayers)->SetNetworkIDManager(&networkIDManager);
			players.at(numPlayers)->SetNetworkID(playerNetworkID);
			jugadores->aumentarJugadores();
			numPlayers++;

			break;

		case ID_LOAD_CURRENT_CLIENTS:
			cout << "ID_LOAD_CURRENT_CLIENTS\n";
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			param=numClients;
			bsIn.Read(numClients);
			if (param<numClients){
				arrayTipoCorredor.push_back(3);
			}
			if (controlPlayer == -1) controlPlayer = numClients - 1;
			cout << "Clientes: " << numClients << endl;
		break;

		case ID_LOAD_CURRENT_PLAYERS:
		/*	int i;
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(numPlayers);
			std::cout << "Cargando numero de jugadores: " << numPlayers << std::endl;
			for (i = 0; i < numPlayers; i++)
			{
				bsIn.Read(posicion.getX());
				bsIn.Read(posicion.getY());
				bsIn.Read(posicion.getZ());
				bsIn.Read(playerNetworkID);
				playerAux = new CorredorRed("assets/coche.obj", posicion,Corredor::tipo_jugador::GLADIADOR);
				players.push_back(playerAux);
				players.at(i)->getNodo()->setID(i);
				//player[i]->setPosition(posicion);
				players.at(i)->SetNetworkIDManager(&networkIDManager);
				players.at(i)->SetNetworkID(playerNetworkID);
				jugadores->aumentarJugadores();
			}
			cout << "ahora vamos a crear el suyo: " << endl;
			pos = pos2[i];
			playerAux = new CorredorJugador("assets/coche.obj", pos,Corredor::tipo_jugador::GLADIADOR);
			players.push_back(playerAux);
			players.at(i)->getNodo()->setID(i);
			typeID = ID_SPAWN_PLAYER;
			players.at(i)->SetNetworkIDManager(&networkIDManager);
			playerNetworkID = players.at(i)->GetNetworkID();
			assert(networkIDManager.GET_OBJECT_FROM_ID<CorredorJugador *>(playerNetworkID) == players.at(numPlayers));
			x = pos.getX();
			y = pos.getY();
			z = pos.getZ();
			bsOut.Write(typeID);
			cout << x <<" - "<< y <<" - "<< z <<endl;
			bsOut.Write(x); //Posicion X
			bsOut.Write(y); //Posicion Y
			bsOut.Write(z); //Posicion Z
			bsOut.Write(players.at(numPlayers)->GetNetworkID());
			client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			jugadores->aumentarJugadores();
			cout << "Jugador creado en pos: " << i << endl;

			controlPlayer = i;
			numPlayers++;
			netLoaded = true;
			*/
			break;
		case ID_PLAYER_MOVE:
			if (netLoaded)
			{
				float *pos = new float[3];
				float *ori = new float[3];
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(pos[0]);
				bsIn.Read(pos[1]);
				bsIn.Read(pos[2]);
				bsIn.Read(ori[0]);
				bsIn.Read(ori[1]);
				bsIn.Read(ori[2]);
				bsIn.Read(id);
				//cout << "Jugador "<< id <<": "<< pos[0] <<" - "<< pos[1] <<" - "<< pos[2] <<" - "<< endl;
				/*float prevY = player[id]->getNodo()->getPosition().Y;
				float dif = pos[1]-prevY;
				if (dif>-0.5 && dif<0.5){
					pos[1]=prevY;
				}*/
				//posicion = networkIDManager.GET_OBJECT_FROM_ID<PlayerClient *>(playerNetworkID)->getPosition();
				
				players.at(id)->setPosicion(pos, ori);

				//networkIDManager.GET_OBJECT_FROM_ID<PlayerClient *>(playerNetworkID)->setPosition(posicion);
			}

			break;

		case ID_PLAYER_STATE:
			if (netLoaded)
			{
				int estado1, estado2, estado3, estado4;
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(estado1);
				bsIn.Read(estado2);
				bsIn.Read(estado3);
				bsIn.Read(estado4);
				bsIn.Read(id);
				//std::cout << estado1 << std::endl;
				EstadosJugador *estados = players.at(id)->getEstados();

				estados->setEstadoMovimiento(estado1);
				estados->setDireccionMovimiento(estado2);
				estados->setEstadoObjeto(estado3);
				estados->setEstadoCoche(estado4);
			}

			break;

		case ID_PLAYER_SET_OBJECT:
			if (netLoaded)
			{
				int t, id;

				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(t);
				bsIn.Read(id);
				cout <<" asignando al jugador *** "<< id <<" *** ";
				players.at(id)->setTipoObj(t);

			}

			break;

		case ID_PLAYER_THROW_OBJECT:
			if (netLoaded)
			{
				int id;

				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(id);
				cout <<" el jugador "<< id<<" lanza el objeto" << endl;
				//players.at(id)->lanzarItemRed();
			}

		case ID_REFRESH_SERVER:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			if(netLoaded){
				float *pos = new float[3];
				float *ori = new float[3];
				for(int i=0; i<players.size(); i++){

					bsIn.Read(pos[0]);
					bsIn.Read(pos[1]);
					bsIn.Read(pos[2]);
					bsIn.Read(ori[0]);
					bsIn.Read(ori[1]);
					bsIn.Read(ori[2]);
					bsIn.Read(param);
					bsIn.Read(id);
					
					players.at(id)->setPosicion(pos, ori);
					if(param!=players.at(id)->getTipoObj())
						players.at(id)->setTipoObj(param);
				}
			}
			//PlayerMovement();

			break;

		case ID_PLAYER_DISCONNECT:
			std::cout << "Jugador desconectado \n";
			int playerDisconnect;

			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(playerDisconnect);
			players.erase(players.begin() + playerDisconnect);
			numPlayers--;
			jugadores->decrementarJugadores();
			if(controlPlayer>playerDisconnect)	//Si el jugador controlado por este cliente esta por arriba del jugador eliminado se debe reducir
				controlPlayer--;				//controlPlayer para no cambiar de jugador con la reordenacion del array

			break;

		default:

			break;
		}
	jugadores->setJugadores(players);
	}
	return 0;
}

//====================================================================================================
// Se recoge el identificador del paquete que se le pasa por parametro (se llama desde ReceivePackets)
//====================================================================================================
unsigned char Client::GetPacketIdentifier(RakNet::Packet *p)
{
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char)p->data[0];
}

//==================================================================================
// se cierra la conexion con el servidor y se destruye la instancia del cliente
//==================================================================================
void Client::ShutDownClient()
{
	std::cout << "Cerrando cliente\n";

	typeID = ID_PLAYER_DISCONNECT;
	RakNet::BitStream bsOut;
	bsOut.Write(typeID);
	bsOut.Write(controlPlayer); 

	client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	client->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(client);
}

void Client::SpawnPlayer()
{
	if (!spawned)
	{
		/*
		vector3df pos(10, 20, 300);
		player[numPlayers] = new CorredorRed("assets/coche.obj", pos);
		vector3df posicion;
		posicion = player[numPlayers]->getNodo()->getPosition();
		typeID = ID_SPAWN_PLAYER;
		player[numPlayers]->SetNetworkIDManager(&networkIDManager);
		playerNetworkID = player[numPlayers]->GetNetworkID();
		assert(networkIDManager.GET_OBJECT_FROM_ID<CorredorRed *>(playerNetworkID) == player[numPlayers]);

		RakNet::BitStream bsOut;
		bsOut.Write(typeID);

		bsOut.Write(posicion.X); //Posicion X
		bsOut.Write(posicion.Y); //Posicion Y
		bsOut.Write(posicion.Z); //Posicion Z
		bsOut.Write(player[numPlayers]->GetNetworkID());
		client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

		controlPlayer = numPlayers;
		numPlayers++;
		spawned = true;
		*/
	}
}

int Client::getControlPlayer(){
	return controlPlayer;
}

bool Client::getConnected(){
	return connected;
}

bool Client::getStarted(){
	return started;
}

int Client::getNumPlayers() {
	return numPlayers;
}

int Client::getNumClients(){
	return numClients;
}

int Client::getMaxPlayers() {
	return maxPlayers;
}
vector<int> Client::getArrayTipoCorredor(){
	return arrayTipoCorredor;
}
int Client::getTipoCorredor(int i){
	return arrayTipoCorredor.at(i);
}
void Client::PlayerAction(){
	/*int estado1 = players.at(numPlayers-1)->getEstados()->getEstadoMovimiento();
	int estado2 = players.at(numPlayers-1)->getEstados()->getDireccionMovimiento();
	int estado3 = players.at(numPlayers-1)->getEstados()->getEstadoObjeto();
	int estado4 = players.at(numPlayers-1)->getEstados()->getEstadoCoche();
	typeID = ID_PLAYER_STATE;
	RakNet::BitStream bsOut;

	bsOut.Write(typeID);
	bsOut.Write(estado1);
	bsOut.Write(estado2);
	bsOut.Write(estado3);
	bsOut.Write(estado4);
	bsOut.Write(controlPlayer);
	
	client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
*/
}

void Client::PlayerMovement(){

	if(netLoaded){
		GestorJugadores *jugadores = GestorJugadores::getInstancia();
		players = jugadores->getJugadores();
		btVector3 position = players.at(controlPlayer)->getRigidBody()->getCenterOfMassPosition();
		float *pos = new float[3];

		pos[0] = position.getX();
		pos[1] = position.getY();
		pos[2] = position.getZ();

		float *ori = new float[3];

		ori[0] = players.at(controlPlayer)->getNodo()->getRotation().X;
		ori[1] = players.at(controlPlayer)->getNodo()->getRotation().Y;
		ori[2] = players.at(controlPlayer)->getNodo()->getRotation().Z;

		typeID = ID_PLAYER_MOVE;
		RakNet::BitStream bsOut;
		bsOut.Write(typeID);
		bsOut.Write(pos[0]);
		bsOut.Write(pos[1]);
		bsOut.Write(pos[2]);
		bsOut.Write(ori[0]);
		bsOut.Write(ori[1]);
		bsOut.Write(ori[2]);
		bsOut.Write(controlPlayer);
		//std::cout << "Control: " << controlPlayer << std::endl;

		client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}

void Client::PlayerSetObject(int tipo){
	typeID = ID_PLAYER_SET_OBJECT;
	RakNet::BitStream bsOut;
	cout <<"He cogido el objeto ---"<< tipo <<" --- y lo comparto con los demas"<< endl;
	bsOut.Write(typeID);
	bsOut.Write(tipo);
	bsOut.Write(controlPlayer);
	client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::PlayerThrowObject(){
	typeID = ID_PLAYER_THROW_OBJECT;
	RakNet::BitStream bsOut;
	cout <<"He lanzado el objeto!" << endl;
	bsOut.Write(typeID);
	bsOut.Write(controlPlayer);
	client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::setNetloaded(bool b){
	netLoaded = b;
}