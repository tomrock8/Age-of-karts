#include "GestorColisiones.hpp"

#define TAMANYOCAJAS 10

void GestorColisiones::ComprobarColisiones(Corredor *pj1Col_1, Caja *cajas[], Proyectil *item_1){
	MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();

    int numManifolds = mundo->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold *contactManifold = mundo->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject *obA = contactManifold->getBody0();
		const btCollisionObject *obB = contactManifold->getBody1();
        item=item_1;
        pj1Col=pj1Col_1;
		nodoA = static_cast<ISceneNode *>(obA->getUserPointer());
		nodoB = static_cast<ISceneNode *>(obB->getUserPointer());
        if(nodoA != 0 && nodoB != 0){
            JugadorCaja(cajas);
            objetoDestruible();
        }

    }

}

void GestorColisiones::JugadorCaja(Caja *cajas[]){
    bool reorganizar=false;
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    
    if (strcmp("Jugador", nodoA->getName()) == 0)
    {
        if (strcmp("Caja", nodoB->getName()) == 0)
        {
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
}

void GestorColisiones::objetoDestruible(){
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    
    if (strcmp("Proyectil", nodoA->getName()) == 0)
    {
        if (strcmp("Destruible", nodoB->getName()) == 0)
        {
            cout << "Destruible - Item\n";
            //int idB = nodoB->getID();
            item->Delete();
            delete item;
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
    
}    
