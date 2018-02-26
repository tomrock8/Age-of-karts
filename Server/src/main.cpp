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

	clock_t tiempoRefresco = clock();
    // -----------------------------
    //  GAME LOOP
    // -----------------------------
    RakSleep(30);
    while (true)
    {
        server->ReceivePackets();
        clock_t tiempoActual = clock();
        clock_t timediff = tiempoActual - tiempoRefresco;
        float timediff_sec = ((float)timediff) / 100000;
        if (timediff_sec >= 0.01) {
            server->refreshServer();
            tiempoRefresco = clock();
        }
        if(server->getCommands()==1) return 0;
            //if(refresco==0){
            //    server->refreshServer();
            //    refresco=0;
            //}else refresco--;
    }
    server->ShutDownServer();

    return 0;
}