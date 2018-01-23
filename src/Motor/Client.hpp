
#include <iostream>
//#include "CorredorRed.hpp"
//#include "CorredorJugador.hpp"
#include "GestorJugadores.hpp"
//#include "NetworkEnums.hpp"

#define MAX_PLAYERS 10

class Client
{
  private:
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

  public:
    Client(int = 10);
    void CreateClientInterface();
    void SetIP();
    void ClientStartup();
    void ShutDownClient();
    void PlayerMovement();
    int ReceivePackets(ISceneManager *escena);
    void SpawnPlayer(ISceneManager *escena);

    int getControlPlayer();
    //void UpdateNetworkKeyboard(CTeclado *teclado);

};