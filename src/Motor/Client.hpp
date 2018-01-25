#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
//#include "CorredorRed.hpp"
//#include "CorredorJugador.hpp"
#include "GestorJugadores.hpp"
//#include "NetworkEnums.hpp"

#define MAX_PLAYERS 10

class Client
{

public:

	~Client(void);

	static Client *getInstancia();
	void CreateClientInterface();
	void SetIP();
	void ClientStartup();
	void ShutDownClient();
	void PlayerMovement();
	void PlayerAction();
	void PlayerSetObject(int tipo);
	int ReceivePackets(ISceneManager *escena);
	void SpawnPlayer(ISceneManager *escena);

	int getControlPlayer();
	//void UpdateNetworkKeyboard(CTeclado *teclado);
	
private:
	Client(int maxPlay);

	static Client *instancia;

	int numSockets;
	int numIPs;
	int maxPlayers;
	bool netLoaded;
	unsigned char packetIdentifier;

	RakNet::SocketDescriptor socketDescriptor;
	RakNet::NetworkIDManager networkIDManager;
	RakNet::NetworkID playerNetworkID;
	RakNet::MessageID typeID;
	RakNet::Packet *p;
	RakNet::RakPeerInterface *client;

	std::string serverPort;
	std::string clientPort;
	std::string serverIP;

	Corredor **player;
	int controlPlayer;
	int numPlayers;
	bool spawned;

	unsigned char GetPacketIdentifier(RakNet::Packet *p);

};

#endif