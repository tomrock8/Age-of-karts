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
			vehiculo-> applyEngineForce ( -15000 , 2 );
			vehiculo-> applyEngineForce ( -15000 , 3 );
			vehiculo-> setSteeringValue ( btScalar (0), 0 );
			vehiculo-> setSteeringValue ( btScalar (0), 1 );
			//vehiculo-> set
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_W))
			{
			vehiculo-> applyEngineForce ( 10000 , 2 );
			vehiculo-> applyEngineForce ( 10000 , 3 );
			vehiculo-> setSteeringValue ( 0, 0 );
			vehiculo-> setSteeringValue ( 0, 1 );
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_D))
			{
			vehiculo-> setSteeringValue ( btScalar ( 0.3 ), 0 );
			vehiculo-> setSteeringValue ( btScalar ( 0.3 ), 1 );
			
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_A)){

			vehiculo-> setSteeringValue ( btScalar (- 0.3 ), 0 );
			vehiculo-> setSteeringValue ( btScalar (- 0.3 ), 1 );
			
			mierda = true;
			}
			
			if(teclado->isKeyDown(KEY_KEY_K)){

			vehiculo->setBrake(10000, 2);
			vehiculo->setBrake(10000, 3);
			mierda = true;
			}


			if(!mierda){
			vehiculo-> applyEngineForce ( 0 , 2 );
			vehiculo-> applyEngineForce ( 0 , 3 );
			
			vehiculo-> setSteeringValue ( 0, 0 );
			vehiculo-> setSteeringValue ( 0, 1 );	

			vehiculo->setBrake(20, 2);
			vehiculo->setBrake(20, 3);

			}
	
}

void CorredorJugador::update() {
	
	movimiento();
}