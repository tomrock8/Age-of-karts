
#include <iostream>
#include "GestorJugadores.hpp"
#include <ctime>
#include <vector>
#include <fstream>
#include <string>

#define MAX_CLIENTS 4
#define MAX_PLAYERS 6

struct structPrediccion{
	int timeStamp;
	float posicion[3];
	float rotacion[3];
};

struct structClientes{
    std::string ip;
    int tipoCorredor;
    bool ready;
    structPrediccion prediccion;
};

struct structPaquetes{
    RakNet::BitStream *bitstreamStruct;
    PacketPriority priority;
    PacketReliability reliability;
    char desconocido;
    RakNet::AddressOrGUID receptor;
    bool envio;
};


class Server
{
    private:
        int numSockets;
        int numIPs;
        int maxPlayers;
        int numClients;
        unsigned char packetIdentifier;

        RakNet::SocketDescriptor socketDescriptor;
        RakNet::NetworkIDManager networkIDManager;
        RakNet::NetworkID playerNetworkID;
        RakNet::MessageID typeID;
        RakNet::Packet *p;

        vector<Corredor*> players;
        int controlPlayer;
        int numPlayers;
        bool spawned;

        bool started;
        vector<int> arrayReady;
        vector<int> arrayTipoCorredor;
        vector<structClientes> clientes;
        vector<structPaquetes> paquetes;

        unsigned char GetPacketIdentifier(RakNet::Packet *p);
        void DebugServerInfo();

    public:
        Server(int);
        void CreateServerInterface();
        void ServerStartup();
        void ShutDownServer();
        void ReceivePackets();
        void refreshServer();
        int getCommands();
        void GetConnectionList();
        void playerDisconnection(std::string str_param);
        void setStarted(bool b);
        bool getStarted();
        void deleteEntities();
        void AddSend(RakNet::BitStream *bitstreamStruct, PacketPriority priority, PacketReliability reliability, int desconocido, RakNet::AddressOrGUID receptor, bool envio);
        void Update();

        RakNet::RakPeerInterface *server;

        std::string serverPort;
        std::string clientPort;
        std::string serverIP;
        std::string relayString;


};