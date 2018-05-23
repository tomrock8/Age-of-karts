#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>
#include "Corredor.hpp"
#include "GestorJugadores.hpp"
#include "Escena.hpp"

#define MAX_PLAYERS 10

struct structClientes{
    std::string ip;
    int tipoCorredor;
    bool ready;
	bool corredorJugador;
};



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
	void FinalizarCarrera();
	void ActualizarClienteConectado();
	void aumentarTimestamp();
	void GameLoad();
	
	//METODOS GET
	int getControlPlayer();
	bool getConnected();
	int getNumPlayers();
	int getNumClients();
	int getMaxPlayers();
	std::vector<structClientes> getClientes();
	bool getStarted();
	std::string getClientStats(int i);

	//METODOS SET
	void setNetloaded(bool b);
	void setArrayClients(std::string ip,int tipo,bool rdy,bool corredorJ,int nuevo);
	void BorrarClientes();
	void BorrarCliente(int i);
	
private:
	Client(int maxPlay);

	static Client *instancia;

	int numSockets;
	int numIPs;
	int maxPlayers;
	int numClients;
	int aux;
	bool netLoaded;
	bool connected;
	bool disconnection;
	unsigned char packetIdentifier;
	std::string packetName;

	RakNet::SocketDescriptor socketDescriptor;
	RakNet::NetworkIDManager networkIDManager;
	RakNet::NetworkID playerNetworkID;
	RakNet::MessageID typeID;
	RakNet::Packet *p;
	RakNet::RakPeerInterface *client;

	std::string serverPort;
	std::string clientPort;
	std::string serverIP;

	std::vector<Corredor*> players;
	int controlPlayer;
	int numPlayers;
	int timeStamp;
	bool spawned;
	bool started;
	bool pressed;
	bool pressed2;
	bool pressed3;
	std::vector<structClientes> clientes;
	
	unsigned char GetPacketIdentifier(RakNet::Packet *p);

};

#endif