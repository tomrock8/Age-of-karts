#include "Camara3persona.hpp"


Camara3persona::Camara3persona(ISceneManager *smgr){

camera = smgr->addCameraSceneNode(0, core::vector3df(0.0f,0.0f,0.0f) , core::vector3df(0.0f,0.0f,0.0f), -1);
direction=0;
zdirection=0;
}

void Camara3persona::moveCameraControl(Corredor *pj1,IrrlichtDevice *device)
{
     core::position2d<f32> cursorPos = device->getCursorControl()->getRelativePosition();
     camera = device->getSceneManager()->getActiveCamera();
     core::vector3df cameraPos = camera->getAbsolutePosition();
     
     float change_x = ( cursorPos.X - 0.5 ) * 256.0f;
     float change_y = ( cursorPos.Y - 0.5 ) * 256.0f;
     direction += change_x;
     zdirection -= change_y;
     if( zdirection <- 90 )
         zdirection = -90;
     else
     if( zdirection > 90 )
         zdirection = 90;
     device->getCursorControl()->setPosition( 0.5f, 0.5f );
     
     core::vector3df playerPos = pj1->getNodo()->getPosition();
     
     float xf = playerPos.X - cos( direction * PI / 180.0f ) * 64.0f;
     float yf = playerPos.Y - sin( zdirection * PI / 180.0f ) * 64.0f;
     float zf = playerPos.Z + sin( direction * PI / 180.0f ) * 64.0f;
     
     camera->setPosition( core::vector3df( xf, yf, zf ) );
     camera->setTarget( core::vector3df( playerPos.X, playerPos.Y+25.0f, playerPos.Z ) );
     //pj1->getNodo()->setRotation( core::vector3df( 0, direction, 0 ) );
}