#include "CorredorJugador.hpp"

CorredorJugador::CorredorJugador(stringw rutaObj,vector3df pos) :
	Corredor(rutaObj,pos) {
			checkItem = false;
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

Proyectil *CorredorJugador::actualizarItem(Proyectil *item,int &id){

	CTeclado *teclado = CTeclado::getInstancia();

	if (teclado->isKeyup(KEY_KEY_P))
	{
		
		vector3df posDisparo(cuboNodo->getPosition().X, cuboNodo->getPosition().Y , cuboNodo->getPosition().Z+5);
		//if P is pressed, shoot a box
		if (checkItem == true)
		{
			item = new Proyectil(posDisparo,id);
			//Llama a la funcion de la clase padre
			lanzarItem(item);
			id++;
			checkItem = false;

		}
	}else if (teclado->isKeyDown(KEY_KEY_P)){
		if (cargador > 0)
		{
		checkItem = true;
		}
	}
	return item;
}