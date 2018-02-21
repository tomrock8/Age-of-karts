#include "Server.hpp"
#include <iostream>
#include "NetworkEnums.hpp"

#define MAX_PLAYERS 6 //numero maximo de jugadores conectados al servidor


//======================================================================
// Se inicializan los parametros que necesita el servidor para funcionar
//======================================================================
Server::Server(int maxPlay)
{
	numSockets = 1; //numero de sockets
	numIPs = 1; //numero de ips
	numPlayers = 0; //numero de jugadores iniciales
	numClients = 0; //Numero de clientes conectados
	spawned = false; //false == el servidor aun no ha spwaneado su jugador ; true == ya ha spwaneado su jugador, no puede spwanear mas
	maxPlayers = maxPlay; //asignacion del numero max de jugadores
	serverPort = "6001"; //puerto que va a usar el servidor
}

//=======================================================================================
// Se crea la interfaz del servidor obteniendo una instancia de la clase RakPeerInterface
//=======================================================================================
void Server::CreateServerInterface()
{
	server = RakNet::RakPeerInterface::GetInstance();

	//establecer una contrasenya que deben introducir los clientes que quieran conectarse al servidor
	server->SetIncomingPassword("ageofkarts", (int)strlen("ageofkarts"));

	//tiempo que se espera a desconectar el servidor si este no es capaz de realizar ninguna entrega de mensajes
	server->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	//si el server ha fallado en su creacion, se notifica con un error
	if (!server)
	{
		std::cout << "Error N01: Failed to createServer\n";
	}
}

//=======================================================================================
// Puesta en marcha del servidor. En esta funcion, se inicia el servidor de manera 
// que ya es capaz de recibir conexiones de otros clientes y paquetes de datos
//=======================================================================================
void Server::ServerStartup()
{
	//Se establece el puerto para el socket
	socketDescriptor.port = atoi(serverPort.c_str()); 

	//el socket utiliza el protocolo IPv4
	socketDescriptor.socketFamily = AF_INET;	

	//se pone en marcha el servidor mediante el inicio de los hilos de red y la apertura del puerto de escucha esablecido anteriormente	  
	// Startup(num max de jugadores, descripcion del socket a usar, num de descripciones de sockets);
	bool b = server->Startup(maxPlayers, &socketDescriptor, 1) == RakNet::RAKNET_STARTED;

	//si la inicializacion no devuelve el mensaje RAKNET_STARTED, significa que ha fallado y en ese caso se muestra por pantalla
	if (!b)
	{
		std::cout << "Error N03: Failed to Start Server\n";
	}

	//se establecen algunos parametros del servidor
	server->SetMaximumIncomingConnections(maxPlayers); //num de conexiones maximas aceptadas
	server->SetOccasionalPing(true); 	//true == se realizan ping ocacionales a los clientes para comprobar, por ejemplo, paquetes perdidos 
										//false == no se hacen pings
	server->SetUnreliableTimeout(1000); //se modifica el tiempo de espera de desconexion del servidor

	//se llama al metodo DebugServerInfo() que contiene el resto de inicializacion del servidor
	DebugServerInfo();
}

//============================================================================================
// Complementa a Startup(). Se termina de poner en marcha el servidor obteniendo las 
// direcciones de los sockets usados, la IP del servidor y el identificador unico global GUID
//============================================================================================
void Server::DebugServerInfo()
{
	//se crea una estructura de sockets mediante un tipo List, usando la nueva clase RakNetSocket2
	DataStructures::List<RakNet::RakNetSocket2 *> sockets;

	//se obtienen los distintos sockets usados
	server->GetSockets(sockets);

	//se notifica por pantalla que el servidor se ha puesto en marcha :)
	std::cout << "Server Started! \n";

	//se muestran las direcciones de los sockets que se estan usando mediante la funcion GetBoundAddress()
	std::cout << "Socket addresses used: \n";
	for (unsigned int i = 0; i < sockets.Size(); i++)
	{
		std::cout << i + 1 << ". " << sockets[i]->GetBoundAddress().ToString(true) << std::endl;
		numSockets++;
	}

	//se muestran las diferentes IPs del servidor mediante GetNumberOfAddresses() y GetLocalIP()
	std::cout << "My IP addresses: \n";

	for (unsigned int i = 0; i < server->GetNumberOfAddresses(); i++)
	{
		std::cout << i + 1 << ". " << server->GetLocalIP(i);
		numIPs++;
	}

	//se obtiene y muestra por pantalla el GUID del servidor
	std::cout << " My GUID is " << server->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString() << std::endl;
}


//==================================================================================================================
// funcion que recoge los paquetes recibidos por el servidor y realiza unas acciones u otras segun su identificador
//==================================================================================================================
void Server::ReceivePackets()
{

	//bucle donde se reciben los distintos paquetes, se tratan y se deasignan
	for (p = server->Receive(); p; server->DeallocatePacket(p), p = server->Receive())
	{
		//se coge el identificador del paquete recibido para los casos del switch
		packetIdentifier = GetPacketIdentifier(p);

		//se inicializan el tipo de RakNet para los mensajes de entrada y salida
		RakNet::BitStream bsIn(p->data, p->length, false); //|mensaje de entrada
		RakNet::BitStream bsOut;						   //|mensaje de salida	

		//vector de posicion de Irrlicht
		float posicion[3];
		float rotacion[3];
		int id;
		
		unsigned short numConnections;
		RakNet::SystemAddress systems[10];


		//switch para comprobar el tipo de paquete recibido
		switch (packetIdentifier)
		{

		//uno de los clientes se ha desconectado del servidor
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "ID_DISCONNECTION_NOTIFICATION de " << p->systemAddress.ToString(true) << std::endl;
			std::cout<< "NumPlayers: " << numPlayers << std::endl;
			
			break;

		//el cliente ya esta conectado (en caso de realizar un connect)
		case ID_ALREADY_CONNECTED:
			std::cout << "ID_ALREADY_CONNECTED\n";
			break;

		//un nuevo cliente se ha conectado al servidor
		case ID_NEW_INCOMING_CONNECTION:
			std::cout << "ID_NEW_INCOMING_CONNECTION\n";
			typeID = ID_LOAD_CURRENT_CLIENTS;
			numConnections=10;
			server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
			id=numConnections;
			std::cout << "Numero de conexiones actuales: " << id << std::endl;
			bsOut.Write(typeID);
			bsOut.Write(id);
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			/*
			typeID = ID_LOAD_CURRENT_PLAYERS;
			bsOut.Write(typeID);
			bsOut.Write(numPlayers);
			
			std::cout << "Numero de jugadores actuales: " << numPlayers << std::endl;

			for (int i = 0; i < numPlayers; i++)
			{
				posicion[0] = player[i]->getPosition()[0];
				posicion[1] = player[i]->getPosition()[1];
				posicion[2] = player[i]->getPosition()[2];
				int id = player[i]->getID();
				bsOut.Write(posicion[0]);
				bsOut.Write(posicion[1]);
				bsOut.Write(posicion[2]);
				bsOut.Write(id);
				bsOut.Write(player[i]->GetNetworkID());
			}
			//														 Ip del emisor, false para enviar a todos menos a la ip
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
*/
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
			break;

		//se ha perdido la conexion con uno de los clientes
		case ID_CONNECTION_LOST:
			std::cout << "ID_CONNECTION_LOST de " << p->systemAddress.ToString(true) << std::endl;
			typeID = ID_LOAD_CURRENT_CLIENTS;
			numConnections=10;
			server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
			id=numConnections;
			bsOut.Write(typeID);
			bsOut.Write(id);
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);


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
			break;
		
		case ID_SPAWN_PLAYER:
			std::cout << "SPAWN DE UN JUGADOR\n";
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(posicion[0]);
			bsIn.Read(posicion[1]);
			bsIn.Read(posicion[2]);
			bsIn.Read(playerNetworkID);
			std::cout <<posicion[0] <<", "<<posicion[1] << ", " <<posicion[2] << std::endl;
			player[numPlayers] = new PlayerServer(posicion, numPlayers);
			//player[numPlayers]->setPosition(posicion);
			player[numPlayers]->SetNetworkIDManager(&networkIDManager);
			player[numPlayers]->SetNetworkID(playerNetworkID);

			typeID = ID_SPAWN_PLAYER;
			bsOut.Write(typeID);
			bsOut.Write(posicion[0]);
			bsOut.Write(posicion[1]);
			bsOut.Write(posicion[2]);
			bsOut.Write(numPlayers);
			bsOut.Write(playerNetworkID);
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			numPlayers++;

			std::cout << "Jugador creado, en total: " << numPlayers << std::endl;

			break;

		case ID_PLAYER_MOVE:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(posicion[0]);
			bsIn.Read(posicion[1]);
			bsIn.Read(posicion[2]);
			bsIn.Read(rotacion[0]);
			bsIn.Read(rotacion[1]);
			bsIn.Read(rotacion[2]);
			bsIn.Read(id);
			if(id == numPlayers) id--;  //Este caso solo se da si se ha eliminado un jugador en este conjunto de paquetes;
			player[id]->setPositionRotation(posicion, rotacion);
			//*posicion = *networkIDManager.GET_OBJECT_FROM_ID<PlayerServer *>(playerNetworkID)->getPosition();
			//networkIDManager.GET_OBJECT_FROM_ID<PlayerServer *>(playerNetworkID)->setPosition(posicion);
			typeID = ID_PLAYER_MOVE;
			bsOut.Write(typeID);
			bsOut.Write(posicion[0]);
			bsOut.Write(posicion[1]);
			bsOut.Write(posicion[2]);
			bsOut.Write(rotacion[0]);
			bsOut.Write(rotacion[1]);
			bsOut.Write(rotacion[2]);
			bsOut.Write(id);

			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);

			break;

		case ID_PLAYER_STATE:
			int estado1, estado2, estado3, estado4;
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(estado1);
			bsIn.Read(estado2);
			bsIn.Read(estado3);
			bsIn.Read(estado4);
			bsIn.Read(id);
			if(id == numPlayers) id--;  //Este caso solo se da si se ha eliminado un jugador en este conjunto de paquetes;
			player[id]->setEstados(estado1, estado2, estado3, estado4);

			typeID = ID_PLAYER_STATE;
			bsOut.Write(typeID);
			bsOut.Write(estado1);
			bsOut.Write(estado2);
			bsOut.Write(estado3);
			bsOut.Write(estado4);
			bsOut.Write(id);
			
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			break;

		case ID_PLAYER_SET_OBJECT:

			int t;

			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(t);
			bsIn.Read(id);
			if(id == numPlayers) id--;  //Este caso solo se da si se ha eliminado un jugador en este conjunto de paquetes;
			std::cout << "He cogido un objeto: " << id << std::endl;

			player[id]->setObj(t);

			typeID = ID_PLAYER_SET_OBJECT;
			bsOut.Write(typeID);
			bsOut.Write(t);
			bsOut.Write(id);

			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			break;	

		case ID_PLAYER_THROW_OBJECT:

			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);
			if(id == numPlayers) id--; //Este caso solo se da si se ha eliminado un jugador en este conjunto de paquetes;

			player[id]->setObj(0);

			typeID = ID_PLAYER_THROW_OBJECT;
			bsOut.Write(typeID);
			bsOut.Write(id);

			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			break;

		case ID_PLAYER_DISCONNECT:
			std::cout << "Borrando player\n";
			int playerDisconnect;
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(playerDisconnect);
			std::cout << "Jugador eliminado: " << playerDisconnect << " / " <<numPlayers << std::endl;

			//playerDisconnection(playerDisconnect);
			typeID = ID_LOAD_CURRENT_CLIENTS;
			numConnections=10;
			server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
			id=numConnections;
			bsOut.Write(typeID);
			bsOut.Write(id);
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

			std::cout << "Jugador Borrado\n";

			break;
		
		default:

			break;
		}
	}
}

//====================================================================================================
// Se recoge el identificador del paquete que se le pasa por parametro (se llama desde ReceivePackets)
//====================================================================================================
unsigned char Server::GetPacketIdentifier(RakNet::Packet *p)
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

//==================================================
// Se cierra el servidor y se destruye su instancia
//==================================================
void Server::ShutDownServer()
{
	//se muestra por pantalla que el servidor va a cerrarse
	std::cout << "Cerrando server\n";

	//se cierra el servidor especificando el numero de milisegundos a esperar para que los mensajes terminen de salir del servidor
	//si el tiempo == 0, entonces no se envia la notifiacion de desconexion
	server->Shutdown(300);

	//se destruye la instancia del servidor de la misma forma que se recogio en su momento
	RakNet::RakPeerInterface::DestroyInstance(server);
}

//=======================================================================================================================
// se refresca el servidor cada cierto periodo de tiempo pasandole la posicions de todos los objetos al resto de clientes
//=======================================================================================================================
void Server::refreshServer()
{
	std::cout << "Refresco\n";

	//tipo de RakNet para el mensaje de salida
	RakNet::BitStream bsOut;

	//vector de posicion de Irrlicht
	float* posicion;

	//ID del mensaje que notifica el refresco
	typeID = ID_REFRESH_SERVER;

	//se escribe el tipo de ID en el mensaje de salida
	bsOut.Write(typeID);

	std::cout << numPlayers << std::endl;

	//entidades a refrescar
	for (int i = 0; i < numPlayers; i++)
	{
		posicion = player[i]->getPosition();
		bsOut.Write(posicion[0]);
		bsOut.Write(posicion[1]);
		bsOut.Write(posicion[2]);
	}

	//se envia el mensaje de salida
	server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

int Server::getCommands(){
	std::string mensaje;
	std::string mensajeIP;
	std::string mensajePort;
	char msj[2048];
	RakNet::RakNetStatistics *rss;
	if(kbhit()){
		std::cin >> mensaje;
		if(mensaje.compare("shut")==0){
			ShutDownServer();
			return 1;
		}else if(mensaje.compare("getConnectionList")==0){
			GetConnectionList();
		}else if(mensaje.compare("stats")==0){
			/*
			Si descomentas esto y el rss del header pasa esto: 
			
			exec: malloc.c:2427: sysmalloc: Assertion `(old_top == initial_top (av) && old_size == 0) || ((unsigned long) (old_size) >= MINSIZE && prev_inuse (old_top) && ((unsigned long) old_end & (pagesize - 1)) == 0)' failed.
			Aborted (core dumped)

			*/
			rss=server->GetStatistics(server->GetSystemAddressFromIndex(0));
			StatisticsToString(rss, msj, 2);
			printf("%s", msj);
			printf("Ping %i\n", server->GetAveragePing(server->GetSystemAddressFromIndex(0)));
		}else if(mensaje.compare("kick")==0){
			GetConnectionList();
			int index;
			std::cout << "Introduzca el indice para kick: \n";
			std::cin>>index;
			playerDisconnection(index);
			server->CloseConnection(server->GetSystemAddressFromIndex(index), true, 0);
		}
	}
	return 0;
}

void Server::GetConnectionList(){
	RakNet::SystemAddress systems[10];
	unsigned short numConnections=10;
	server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
	std::cout << "Connection list: \n";
	for (int i=0; i < numConnections; i++)
	{
		std::cout << i << ". " << systems[i].ToString(true) << std::endl;
	}
}

void Server::playerDisconnection(int playerDisconnect){
	RakNet::BitStream bsOut;						   //|mensaje de salida	
	delete player[playerDisconnect];
	std::cout<< "NumPlayers: " << numPlayers << std::endl;
	for(int i=0; i<numPlayers; i++){	//Recorre todos los players
		std::cout << "Player " << i << std::endl;
		if(playerDisconnect<=i){		//Si se encuentra con playerDisconnect asigna la posicion siguiente a esta
			std::cout << "Reorganizo Array\n";
			if(i<numPlayers-1){			//Si la posicion es la final asigna null al ultimo player para evitar basura
				std::cout << "Asigno i + 1 a i\n";
				player[i]=player[i+1];
			}else{
				std::cout << "Asigno i a NULL\n";
				player[i]=NULL;
			}
		}
	}

	numPlayers--;

	typeID = ID_PLAYER_DISCONNECT;
	bsOut.Write(typeID);
	bsOut.Write(playerDisconnect);
	server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}