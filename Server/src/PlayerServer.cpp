#include "PlayerServer.hpp"

PlayerServer::PlayerServer(int *pos)
{
    posicion = new int[3];
    posicion[0] = pos[0];
    posicion[1] = pos[1];
    posicion[2] = pos[2];
}

int* PlayerServer::getPosition(){
    return posicion;
}

void PlayerServer::acelerar(){

}

void PlayerServer::frenar(){

}
void PlayerServer::girarA(){

}

void PlayerServer::girarB(){

}