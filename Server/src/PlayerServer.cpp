#include "PlayerServer.hpp"

PlayerServer::PlayerServer(float *pos, int i)
{
    posicion[0] = pos[0];
    posicion[1] = pos[1];
    posicion[2] = pos[2];
    rotacion[0] = 0;
    rotacion[1] = 0;
    rotacion[2] = 0;
    id = i;
}

float* PlayerServer::getPosition(){
    return posicion;
}

void PlayerServer::setPositionRotation(float pos[3], float rot[3]){
	std::cout << "setPositionRotation Inicio\n";
    std::cout <<"1 - " << pos[0] << " - " << rot[0] << std::endl;
    posicion[0] = pos[0];
    posicion[1] = pos[1];
    posicion[2] = pos[2];
    rotacion[0] = rot[0];
    rotacion[1] = rot[1];
    rotacion[2] = rot[2];
	std::cout << "setPositionRotation Fin\n";
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