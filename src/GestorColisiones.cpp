#include "GestorColisiones.hpp"

#define TAMANYOCAJAS 10

void GestorColisiones::ComprobarColisiones(Corredor *pj1Col_1, Caja *cajas[])
{
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    pj1Col = pj1Col_1;
    int numManifolds = mundo->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold *contactManifold = mundo->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject *obA = contactManifold->getBody0();
        const btCollisionObject *obB = contactManifold->getBody1();

        nodoA = static_cast<ISceneNode *>(obA->getUserPointer());
        nodoB = static_cast<ISceneNode *>(obB->getUserPointer());

        if (nodoA != 0 && nodoB != 0)
        {

            /* if (strcmp("Proyectil", nodoB->getName()) == 0){
               cout<<"------------------------ID PROYECTIL: "<<nodoB->getID()<<endl;
               if (item->getNodo() !=NULL){
                   cout<<"------------------------ID PROYECTIL ITEM: "<<item->getNodo()->getID()<<endl;
               }else{
                   cout<<"------------------------ID PROYECTIL ITEM CORRUPTO: ";
               }
            }*/

            if (JugadorCaja(cajas))
                continue;
            if (JugadorTurbo())
                continue;
            if (objetoDestruible())
                continue;
        }
    }
}

//
// Comprobar colisiones entre Jugador y turbo
//
bool GestorColisiones::JugadorTurbo()
{
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    bool colision = false;
    Motor3d *m = Motor3d::getInstancia();
    ITimer *irrTimer;
    irrTimer = m->getDevice()->getTimer();
    u32 TimeStamp = irrTimer->getTime();

    //cout << TimeStamp << endl;

    if (strcmp("Jugador", nodoA->getName()) == 0)
    {
        if (strcmp("Turbo", nodoB->getName()) == 0)
        {
            colision = true;
            pj1Col->SetFuerzaVelocidad(30000);

            //cout << "Jugador - Turbo\n";
        }
    }
    return colision;
}

//
// Comprobar colisiones entre Jugador y Caja
//
bool GestorColisiones::JugadorCaja(Caja *cajas[])
{
    bool reorganizar = false;
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    bool colision = false;

    if (strcmp("Jugador", nodoA->getName()) == 0)
    {
        if (strcmp("Caja", nodoB->getName()) == 0)
        {
            colision = true;
            cout << "Jug - Caja\n";
            int idB = nodoB->getID();
            for (int i = 0; i < TAMANYOCAJAS; i++)
            {
                if (cajas[i] != NULL)
                {
                    if (cajas[i]->getIDCaja() == idB)
                    {
                        cajas[i]->Delete(pj1Col);
                        delete cajas[i];
                        reorganizar = true;
                    }
                    if (reorganizar)
                    {
                        if (i < TAMANYOCAJAS - 1)
                            cajas[i] = cajas[i + 1];
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
// Comprobar colisiones entre proyectil y objeto destruible
//
bool GestorColisiones::objetoDestruible()
{
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    Pista *pista = Pista::getInstancia();
    btDynamicsWorld *mundo = bullet->getMundo();
    core::list<Item *> items = pista->getItems();
    core::list<btRigidBody *> objetos = bullet->getObjetos();
    bool colision = false;

    if (strcmp("Destruible", nodoA->getName()) == 0)
    {
        if (strcmp("Proyectil", nodoB->getName()) == 0)
        {
            colision = true;
            cout << "Destruible - Item\n";
            int idB = nodoB->getID();
            for (core::list<Item *>::Iterator Iterator = items.begin(); Iterator != items.end(); ++Iterator)
            {
                Item *item = *Iterator;
                cout << "NodoB: " << idB << " == NodoItem: " << item->getNodo()->getID() << endl;
                if (item->getNodo()->getID() == idB)
                {
                    cout << "Entro\n";
                    item->Delete();
                    Iterator = items.erase(Iterator);
                    pista->setItems(items);

                    return true;
                }
            }
            /*
            if (item->Delete()){
                delete item;
            }
            for(int i=0; i< TAMANYOCAJAS; i++){
}

void GestorColisiones::objetoDestruible(core::list<btRigidBody *> &objetosMundo, btDiscreteDynamicsWorld *mundoMetodo)
{
    if (strcmp("Proyectil", nodoA->getName()) == 0)
    {
        if (strcmp("Destruible", nodoB->getName()) == 0)
        {
            cout << "Destruible - Item\n";
            //int idB = nodoB->getID();
            item->Delete(objetosMundo, mundoMetodo);
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
    return colision;
}
