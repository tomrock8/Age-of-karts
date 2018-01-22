#include "PlayerServer.hpp"

PlayerServer::PlayerServer(float *pos, int i)
{
    posicion = new float[3];
    posicion[0] = pos[0];
    posicion[1] = pos[1];
    posicion[2] = pos[2];
    rotacion = new float[3];
    id = i;
}

int* PlayerServer::getPosition(){
    return posicion;
}

void PlayerServer::setPositionRotation(float *pos, float *rot){
    posicion = pos;
    rotacion = rot;
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