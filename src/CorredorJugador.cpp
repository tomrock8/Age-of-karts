#include "CorredorJugador.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj,vector3df pos) :
	Corredor(rutaObj,pos) {

}

/**
	Movimiento del jugador controlado por el teclado
	TODO: Modificar para detectar el input que se recibe.
*/
void CorredorJugador::movimiento() {
	CTeclado *teclado = CTeclado::getInstancia();
	Motor3d *m = Motor3d::getInstancia();
	bool mierda=false;

			//-------ENTRADA TECLADO ----------//
			if (teclado->isKeyDown(KEY_ESCAPE))
			{
				m->getDevice()->closeDevice();
				
			}
			if (teclado->isKeyDown(KEY_KEY_S))
			{
			vehiculo-> applyEngineForce ( FuerzaFrenado , 2 );
			vehiculo-> applyEngineForce ( FuerzaFrenado , 3 );
			vehiculo-> setSteeringValue ( btScalar (0), 0 );
			vehiculo-> setSteeringValue ( btScalar (0), 1 );
			//vehiculo-> set
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_W))
			{
			vehiculo-> applyEngineForce ( Fuerza , 2 );
			vehiculo-> applyEngineForce ( Fuerza , 3 );
			vehiculo-> setSteeringValue ( btScalar (0), 0 );
			vehiculo-> setSteeringValue ( btScalar (0), 1 );
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_D))
			{
			vehiculo-> setSteeringValue ( FuerzaGiro, 0 );
			vehiculo-> setSteeringValue ( FuerzaGiro, 1 );
			
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_A)){

			vehiculo-> setSteeringValue ( -FuerzaGiro, 0 );
			vehiculo-> setSteeringValue ( -FuerzaGiro, 1 );
			
			mierda = true;
			}
			
			if(teclado->isKeyDown(KEY_SPACE)){

			vehiculo->setBrake(300, 2);
			vehiculo->setBrake(300, 3);

				
				if(teclado->isKeyDown(KEY_KEY_D))
				vehiculo->applyEngineForce(0,2);
				if(teclado->isKeyDown(KEY_KEY_A))
				vehiculo->applyEngineForce(0,3);

				if(!teclado->isKeyDown(KEY_KEY_A) && ! teclado->isKeyDown(KEY_KEY_D)){
				vehiculo->applyEngineForce(0,2);
				vehiculo->applyEngineForce(0,3);
				}
			mierda = true;
			}


			if(!mierda){
			vehiculo-> applyEngineForce ( 0 , 2 );
			vehiculo-> applyEngineForce ( 0 , 3 );
			
			vehiculo-> setSteeringValue ( 0, 0 );
			vehiculo-> setSteeringValue ( 0, 1 );	

			vehiculo->setBrake(60, 2);
			vehiculo->setBrake(60, 3);

			}
	
}

void CorredorJugador::update() {
	
	movimiento();
}