#include "PlayerServer.hpp"

PlayerServer::PlayerServer(int *pos, int i)
{
    posicion = new int[3];
    posicion[0] = pos[0];
    posicion[1] = pos[1];
    posicion[2] = pos[2];
    id = i;
}

int* PlayerServer::getPosition(){
    return posicion;
}

int PlayerServer::getID(){
    return id;
}

void PlayerServer::acelerar(){

}

void PlayerServer::frenar(){

}
void PlayerServer::girarA(){

}

void PlayerServer::girarB(){

}
