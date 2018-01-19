#include "Server.hpp"


const int W_WIDTH = 1024;
const int H_WIDTH = 600;

Server *server;

int main()
{
    int x = 0;

    int refresco = 0;
    std::string mensaje;
    //RED

    server = new Server(8);
    server->CreateServerInterface();
    server->ServerStartup();

    // -----------------------------
    //  GAME LOOP
    // -----------------------------
    RakSleep(30);
    while (true)
    {
        server->ReceivePackets();
        if(server->getCommands()==1) return 0;
            //if(refresco==0){
            //    server->refreshServer();
            //    refresco=0;
            //}else refresco--;
    }
    server->ShutDownServer();

    return 0;
}