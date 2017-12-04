#include "Pista.hpp"
#include "IrrlichtLib.hpp"

using namespace std;

enum
{
    // No colisionable, para evitar cogerlo por error
    ID_NULO = 0,

    // Objetos que pueden colisionar
    ID_COLISION = 1 << 0,

    // Objetos para iluminar
    ID_ILUMINAR = 1 << 1
};

Pista::Pista(IrrlichtDevice *device)
{
    inicializar(device);
}

bool Pista::inicializar(IrrlichtDevice *device)
{
    // Mapa cargado desde obj
    mapa = device->getSceneManager()->getMesh("assets/mapa01.obj");

    if (!mapa)
    {
        cout << "Error al inicializar la malla del mapa." << endl;
        device->drop();
        return false;
    }

    // -----------------------------
    //  GEOMETRIA MAPA
    // -----------------------------

    // Cargar modelo mapa
    mapaNodo = device->getSceneManager()->addOctreeSceneNode(mapa, 0, ID_COLISION);

    if (!mapaNodo)
    {
        cout << "Error al cargar el mapa." << endl;
        return false;
    }

    mapaNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
    mapaNodo->setPosition(vector3df(0, 0, 0));
    mapaNodo->setName("MAPA");

    return true;
}

ITriangleSelector *Pista::setColisiones(IrrlichtDevice *device, ITriangleSelector *selector)
{
    if (!mapaNodo)
    {
        cout << "Error al cargar el mapa. No se pueden inicializar las colisiones." << endl;
        return NULL;
    }

    selector = device->getSceneManager()->createTriangleSelector(mapa, 0);
    mapaNodo->setTriangleSelector(selector);
    selector->drop();

    return selector;
}