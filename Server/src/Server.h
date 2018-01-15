
#include <iostream>
#include "PlayerServer.h"
#include "CTeclado.cpp"

#define MAX_CLIENTS 10
#define MAX_PLAYERS 10

class Server
{
    private:
        int numSockets;
        int numIPs;
        int maxPlayers;
        unsigned char packetIdentifier;

        RakNet::SocketDescriptor socketDescriptor;
        RakNet::NetworkIDManager networkIDManager;
        RakNet::NetworkID playerNetworkID;
        RakNet::MessageID typeID;
        RakNet::Packet *p;

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
        
        RakNet::RakPeerInterface *server;

        std::string serverPort;
        std::string clientPort;
        std::string serverIP;
        std::string relayString;

        PlayerServer *player[MAX_PLAYERS];
        int controlPlayer;
        int numPlayers;
        bool spawned;

};