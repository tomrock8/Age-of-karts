/*#ifndef SKYBOX_H
#define SKYBOX_H


#include "libGL.hpp"
#include "TMotor.hpp"
#include <iostream>

class Skybox{

public:
    void deleteCubeMap(GLuint& texture);
    void setupCubeMap(GLuint& texture, SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos, SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg);
    void setupCubeMap(GLuint& texture);
    void createSkyBox();
    void createCubeMap();
    void createShader();
    void setCubeVertices();

private:
    SDL_Surface *xpos;
    SDL_Surface *ypos;
    SDL_Surface *zpos;
    SDL_Surface *xneg;
    SDL_Surface *yneg;
    SDL_Surface *zneg;
    
    GLint PVM
    GLint vertex;


};*/