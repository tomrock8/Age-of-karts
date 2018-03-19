#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>
//#include "CorredorRed.hpp"
//#include "CorredorJugador.hpp"
#include "GestorJugadores.hpp"
#include "Escena.hpp"
//#include "NetworkEnums.hpp"

#define MAX_PLAYERS 10

struct structClientes{
    std::string ip;
    int tipoCorredor;
    bool ready;
	bool corredorJugador;
};


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
	void FinalizarCarrera();
	void ActualizarClienteConectado();
	
	//METODOS GET
	int getControlPlayer();
	bool getConnected();
	int getNumPlayers();
	int getNumClients();
	int getMaxPlayers();
	vector<structClientes> getClientes();
	bool getStarted();

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
	int timeStamp;
	bool spawned;
	bool started;
	bool pressed;
	vector<structClientes> clientes;
	
	unsigned char GetPacketIdentifier(RakNet::Packet *p);

};

#endif