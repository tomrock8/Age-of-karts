
#include <iostream>
#include "GestorJugadores.hpp"
#include <ctime>
#include <vector>

#define MAX_CLIENTS 4
#define MAX_PLAYERS 6

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
        vector<int> clientes;
        int controlPlayer;
        int numPlayers;
        bool spawned;

        bool started;
        vector<int> arrayTipoCorredor;

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
        void playerDisconnection(int playerDisconnect);
        void setStarted(bool b);
        bool getStarted();
        
        RakNet::RakPeerInterface *server;

        std::string serverPort;
        std::string clientPort;
        std::string serverIP;
        std::string relayString;


};