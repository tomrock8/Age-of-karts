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

Pista::Pista()
{
    inicializar();
}

bool Pista::inicializar()
{
    // Mapa cargado desde obj
    Motor3d *m = Motor3d::getInstancia();
    mapa = m->getScene()->getMesh("assets/mapa01.obj");

    if (!mapa)
    {
        cout << "Error al inicializar la malla del mapa." << endl;
        m->getDevice()->drop();
        return false;
    }

    // -----------------------------
    //  GEOMETRIA MAPA
    // -----------------------------

    // Cargar modelo mapa
    mapaNodo = m->getScene()->addOctreeSceneNode(mapa, 0, ID_COLISION);

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

ITriangleSelector *Pista::setColisiones(ITriangleSelector *selector)
{
    Motor3d *m = Motor3d::getInstancia();

    if (!mapaNodo)
    {
        cout << "Error al cargar el mapa. No se pueden inicializar las colisiones." << endl;
        return NULL;
    }

    selector = m->getScene()->createTriangleSelector(mapa, 0);
    mapaNodo->setTriangleSelector(selector);
    selector->drop();

    return selector;
}