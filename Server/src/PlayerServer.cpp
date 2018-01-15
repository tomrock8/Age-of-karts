#include "PlayerServer.h"

PlayerServer::PlayerServer()
{
    posicion[0] = 0;
    posicion[1] = 0;
    posicion[2] = 0;
}

int* PlayerServer::getPosition()
{
    return posicion;
}

void PlayerServer::setPosition(int posicionSet[])
{
    posicion[0] = posicionSet[0];
    posicion[1] = posicionSet[1];
    posicion[2] = posicionSet[2];
}

void PlayerServer::setPositionX(int x)
{

    posicion[0] = x;
    std::cout<<"SetPos: " <<x<< std::endl;
}

void PlayerServer::playerUpdate()
{

}
