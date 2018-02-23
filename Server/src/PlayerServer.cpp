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
    obj = 0;
}

float* PlayerServer::getPosition(){
    return posicion;
}

void PlayerServer::setPositionRotation(float pos[3], float rot[3]){
    posicion[0] = pos[0];
    posicion[1] = pos[1];
    posicion[2] = pos[2];
    rotacion[0] = rot[0];
    rotacion[1] = rot[1];
    rotacion[2] = rot[2];
}

void PlayerServer::setEstados(int e1, int e2, int e3, int e4){
    estado1 = e1;
    estado2 = e2;
    estado3 = e3;
    estado4 = e4;
}

int PlayerServer::getID(){
    return id;
}

void PlayerServer::setObj(int t){
    obj = t;
}

void PlayerServer::acelerar(){

}

void PlayerServer::frenar(){

}
void PlayerServer::girarA(){

}

void PlayerServer::girarB(){

}

void PlayerServer::setAccion(int a){
    accion = a;
}

int PlayerServer::getAccion(){
    return accion;
}