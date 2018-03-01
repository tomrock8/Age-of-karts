#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>
//#include "CorredorRed.hpp"
//#include "CorredorJugador.hpp"
#include "GestorJugadores.hpp"
//#include "NetworkEnums.hpp"

#define MAX_PLAYERS 10

using namespace std;

class Client
{

public:

	~Client(void);

	static Client *getInstancia();
	void CreateClientInterface();
	void SetIP(std::string ipConexion);
	void ClientStartup();
	void RaceStart();
	void UpdateNetworkKeyboard();
	void ShutDownClient();
	void PlayerMovement();
	void PlayerAction();
	void PlayerSetObject(int tipo);
	void PlayerThrowObject();
	void ChangeCharacter(bool i);
	int ReceivePackets();
	void SpawnPlayer();
	
	int getControlPlayer();
	bool getConnected();
	int getNumPlayers();
	int getNumClients();
	int getMaxPlayers();
	bool getStarted();

	void setNetloaded(bool b);
	
private:
	Client(int maxPlay);

	static Client *instancia;

	int numSockets;
	int numIPs;
	int maxPlayers;
	int numClients;
	bool netLoaded;
	bool connected;
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

	vector<Corredor*> players;
	int controlPlayer;
	int numPlayers;
	bool spawned;
	bool started;
	bool pressed;

	unsigned char GetPacketIdentifier(RakNet::Packet *p);

};

#endif