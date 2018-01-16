#include "Client.hpp"
#include "NetworkEnums.hpp"
#include <iostream>

#define DEBUG_BUILD

//==================================================================================
// constructor que inicializa los valores de las variables requeridad por el cliente
//==================================================================================
Client::Client(int maxPlay)
{
	maxPlayers = maxPlay;
	serverPort = "6001"; //puerto de escucha del servidor a conectarse
	numPlayers = 0;
	spawned = false;   // true == el jugador del cliente ya ha spwaneado
	netLoaded = false; // true == si todos los clientes de la partida han spwaneado
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
void Client::SetIP()
{
	//se le pide al usuario que introduzca la IP del servidor al que quiera conectarse
	std::cout << "Introduce IP del servidor: ";
	std::cin >> serverIP;
	//serverIP = "192.168.1.233";
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
	RakNet::ConnectionAttemptResult car = client->Connect(serverIP.c_str(), atoi(serverPort.c_str()), "yoyo", (int)strlen("yoyo"));

	//se comprueba que la conexion se ha realizado correctamente, en caso contrario se aborta la conexion y se muestra
	//un mensaje de "connection attempt failed" por terminal
	RakAssert(car == RakNet::CONNECTION_ATTEMPT_STARTED);

	//si todo el proceso tiene exito, se avisa al usuario de que el cliente se ha creado y conectado al servidor
	std::cout << "Cliente creado!\n";
}
/*
void Client::UpdateNetworkKeyboard(CTeclado *teclado)
{
	vector3df posicion;

	if (spawned && netLoaded)
	{
		if (teclado->isKeyDown(KEY_KEY_W))
		{
			player[controlPlayer]->move = 1;
		}
		else if (teclado->isKeyDown(KEY_KEY_S))
		{
			player[controlPlayer]->move = -1;
		}
		else
		{
			player[controlPlayer]->move = 0;
		}

		if (teclado->isKeyDown(KEY_KEY_A))
		{
			player[controlPlayer]->turn = 1;
		}
		else if (teclado->isKeyDown(KEY_KEY_D))
		{
			player[controlPlayer]->turn = -1;
		}
		else
		{
			player[controlPlayer]->turn = 0;
		}

		posicion = player[controlPlayer]->getPosition();
		posicion.X += player[controlPlayer]->move;
		posicion.Z += player[controlPlayer]->turn;
		player[controlPlayer]->setPosition(posicion);
		int posX = posicion.X;
		int posY = posicion.Y;
		int posZ = posicion.Z;

		RakNet::BitStream bsOut;
		typeID = ID_PLAYER_MOVE;
		bsOut.Write(typeID);
		bsOut.Write(posX);
		bsOut.Write(posY);
		bsOut.Write(posZ);
		bsOut.Write(player[controlPlayer]->GetNetworkID());

		client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}*/

//==================================================================================================================
// funcion que recoge los paquetes recibidos por el cliente y realiza unas acciones u otras segun su identificador
//==================================================================================================================
int Client::ReceivePackets(ISceneManager *escena)
{

	//bucle donde se reciben los distintos paquetes, se tratan y se deasignan
	for (p = client->Receive(); p; client->DeallocatePacket(p), p = client->Receive())
	{
		//se coge el identificador del paquete recibido para los casos del switch
		packetIdentifier = GetPacketIdentifier(p);

		//se inicializan el tipo de RakNet para los mensajes de entrada y salida
		RakNet::BitStream bsIn(p->data, p->length, false); //|mensaje de entrada
		RakNet::BitStream bsOut;						   //|mensaje de salida

		//vector de posicion de Irrlicht
		vector3df posicion;

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
			return 3;
			break;

		case ID_SPAWN_PLAYER:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(posicion.X);
			bsIn.Read(posicion.Y);
			bsIn.Read(posicion.Z);
			std::cout << posicion.X << " + " << posicion.Z << std::endl;
			bsIn.Read(playerNetworkID);
			/*player[numPlayers] = new PlayerClient(escena);
			player[numPlayers]->setPosition(posicion);
			player[numPlayers]->SetNetworkIDManager(&networkIDManager);
			player[numPlayers]->SetNetworkID(playerNetworkID);*/
			numPlayers++;

			break;

		case ID_LOAD_CURRENT_PLAYERS:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(numPlayers);
			std::cout << numPlayers << std::endl;
			for (int i = 0; i < numPlayers; i++)
			{
				bsIn.Read(posicion.X);
				bsIn.Read(posicion.Y);
				bsIn.Read(posicion.Z);
				bsIn.Read(playerNetworkID);
				/*player[i] = new PlayerClient(escena);
				player[i]->setPosition(posicion);
				player[i]->SetNetworkIDManager(&networkIDManager);
				player[i]->SetNetworkID(playerNetworkID);*/
			}
			netLoaded = true;
			break;
		case ID_PLAYER_MOVE:
			if (netLoaded)
			{
				int x;
				int y;
				int z;
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(x);
				bsIn.Read(y);
				bsIn.Read(z);
				bsIn.Read(playerNetworkID);

				//posicion = networkIDManager.GET_OBJECT_FROM_ID<PlayerClient *>(playerNetworkID)->getPosition();

				posicion.X = x;
				posicion.Y = y;
				posicion.Z = z;

				//networkIDManager.GET_OBJECT_FROM_ID<PlayerClient *>(playerNetworkID)->setPosition(posicion);
			}

			break;

		case ID_REFRESH_SERVER:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			for (int i = 0; i < numPlayers; i++)
			{
				bsIn.Read(posicion.X);
				bsIn.Read(posicion.Y);
				bsIn.Read(posicion.Z);
				//player[i]->setPosition(posicion);
			}
			netLoaded = true;

			break;

		case ID_PLAYER_DISCONNECT:
			std::cout << "pollas \n";
			int playerDisconnect;

			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(playerDisconnect);

			//player[playerDisconnect]->deleteMesh();
			//delete player[playerDisconnect];
			for(int i=0; i<numPlayers; i++){	//Recorre todos los players
				if(playerDisconnect<=i){		//Si se encuentra con playerDisconnect asigna la posicion siguiente a esta
					//if(i<numPlayers-1)			//Si la posicion es la final asigna null al ultimo player para evitar basura
						//player[i]=player[i+1];
					//else
						//player[i]=NULL;
				}
			}
			numPlayers--;
			if(controlPlayer>playerDisconnect)	//Si el jugador controlado por este cliente esta por arriba del jugador eliminado se debe reducir
				controlPlayer--;				//controlPlayer para no cambiar de jugador con la reordenacion del array

			break;

		default:

			break;
		}
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

void Client::SpawnPlayer(ISceneManager *escena)
{
	if (!spawned)
	{
		/*player[numPlayers] = new PlayerClient(escena);
		vector3df posicion = player[numPlayers]->getPosition();
		typeID = ID_SPAWN_PLAYER;
		player[numPlayers]->SetNetworkIDManager(&networkIDManager);
		playerNetworkID = player[numPlayers]->GetNetworkID();
		assert(networkIDManager.GET_OBJECT_FROM_ID<PlayerClient *>(playerNetworkID) == player[numPlayers]);

		RakNet::BitStream bsOut;
		bsOut.Write(typeID);
		bsOut.Write(posicion.X); //Posicion X
		bsOut.Write(posicion.Y); //Posicion Y
		bsOut.Write(posicion.Z); //Posicion Z
		bsOut.Write(player[numPlayers]->GetNetworkID());
		client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
*/
		controlPlayer = numPlayers;
		numPlayers++;
		spawned = true;
	}
}