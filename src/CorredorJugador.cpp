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
			frenar();
			//vehiculo-> set
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_W))
			{
			acelerar();
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_D))
			{
			girarDerecha();
			
			mierda = true;
			}
			if (teclado->isKeyDown(KEY_KEY_A)){

			girarIzquierda();
			
			mierda = true;
			}
			
			if(teclado->isKeyDown(KEY_SPACE)){

			frenodemano();
				
			mierda = true;
			}

			if(!mierda){
			
				desacelerar();
			}
	
}
Proyectil* CorredorJugador::actualizarItem(int &id,btDiscreteDynamicsWorld* mundo,core::list<btRigidBody *> objetos){
	CTeclado *teclado = CTeclado::getInstancia();
	Motor3d *m = Motor3d::getInstancia();
	if (teclado->isKeyup(KEY_KEY_P))
	{
	vector3df cam(cuboNodo->getPosition().X, cuboNodo->getPosition().Y + 10, cuboNodo->getPosition().Z);
	//if P is pressed, shoot a box
	if (checkItem == true)
	{
		Proyectil *item = new Proyectil(m, cam, id);
		id++;
		lanzarItem(item,mundo,objetos);
		
		checkItem = false;
	}
	}else if (teclado->isKeyDown(KEY_KEY_P)){
		if (cargador > 0)
		{
		checkItem = true;
		}
	}
}