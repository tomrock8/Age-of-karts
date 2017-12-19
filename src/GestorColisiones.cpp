#include "GestorColisiones.hpp"

#define TAMANYOCAJAS 10

void GestorColisiones::ComprobarColisiones(Corredor *pj1Col_1, Caja *cajas[], Proyectil *item_1){
	MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    item=item_1;
    pj1Col=pj1Col_1;
    int numManifolds = mundo->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold *contactManifold = mundo->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject *obA = contactManifold->getBody0();
		const btCollisionObject *obB = contactManifold->getBody1();
   
		nodoA = static_cast<ISceneNode *>(obA->getUserPointer());
		nodoB = static_cast<ISceneNode *>(obB->getUserPointer());
        
      

        if(nodoA != 0 && nodoB != 0){

           /* if (strcmp("Proyectil", nodoB->getName()) == 0){
               cout<<"------------------------ID PROYECTIL: "<<nodoB->getID()<<endl;
               if (item->getNodo() !=NULL){
                   cout<<"------------------------ID PROYECTIL ITEM: "<<item->getNodo()->getID()<<endl;
               }else{
                   cout<<"------------------------ID PROYECTIL ITEM CORRUPTO: ";
               }
            }*/

            if(JugadorCaja(cajas)) continue;
            if(JugadorTurbo()) continue;
            if(objetoDestruible()) continue;
        }

    }

}

//
// Comprobar colisiones entre Jugador y Caja
//
bool GestorColisiones::JugadorCaja(Caja *cajas[]){
    bool reorganizar=false;
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    bool colision=false;
    
    if (strcmp("Jugador", nodoA->getName()) == 0)
    {
        if (strcmp("Caja", nodoB->getName()) == 0)
        {
            colision=true;
            cout << "Jug - Caja\n";
            int idB = nodoB->getID();
            for(int i=0; i< TAMANYOCAJAS; i++){
                if(cajas[i]!=NULL){
                    if(cajas[i]->getIDCaja()==idB){
                        cajas[i]->Delete(pj1Col);
                        delete cajas[i];
                        reorganizar=true;
                    }
                    if(reorganizar){
                        if(i<TAMANYOCAJAS-1)
                            cajas[i]=cajas[i+1];
                        else
                            cajas[i] = NULL;
                    }
                }
            }
        }
    }
		return colision;
}

//
// Comprobar colisiones entre Jugador y turbo
//
bool GestorColisiones::JugadorTurbo(){
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    bool colision=false;
    
    if (strcmp("Jugador", nodoA->getName()) == 0)
    {
        if (strcmp("Turbo", nodoB->getName()) == 0)
        {
            colision=true;
            pj1Col->getVehiculo()->applyEngineForce ( 80000 , 2 );
			pj1Col->getVehiculo()->applyEngineForce ( 80000 , 3 );
            cout << "Jugador - Turbo\n";
        }

    }
    return colision;
}


//
// Comprobar colisiones entre proyectil y objeto destruible
//
bool GestorColisiones::objetoDestruible(){
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    bool colision = false;
    
    if (strcmp("Destruible", nodoA->getName()) == 0)
    {
        if (strcmp("Proyectil", nodoB->getName()) == 0)
        {
            colision = true;
            cout << "Destruible - Item\n";
            //int idB = nodoB->getID();
            if (item->Delete()){
                delete item;
            }
            /*for(int i=0; i< TAMANYOCAJAS; i++){
                if(cajas[i]!=NULL){
                    if(cajas[i]->getIDCaja()==idB){
                        cajas[i]->Delete(objetosMundo, mundoMetodo, pj1Col);
                        delete cajas[i];
                        reorganizar=true;
                    }
                    if(reorganizar){
                        if(i<TAMANYOCAJAS-1)
                            cajas[i]=cajas[i+1];
                        else
                            cajas[i] = NULL;
                    }
                }
            }*/
        }
    }
    return colision;
}    
