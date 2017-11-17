#include <irrlicht.h>
#include "CTeclado.cpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

const int W_WIDTH = 1024;
const int H_WIDTH = 600;

int main()
{
    int x = 0;

    // -----------------------------
    //  PREPARAR LA VENTANA
    // -----------------------------
    CTeclado teclado;
    IrrlichtDevice *ventana = createDevice(EDT_OPENGL, dimension2d<u32>(W_WIDTH, H_WIDTH), 16, false, false, false, &teclado);

    if (!ventana)
        return 1;

    ventana->setWindowCaption(L"AGE OF KARTS");

    IVideoDriver *driver = ventana->getVideoDriver();
    ISceneManager *escena = ventana->getSceneManager();
    IGUIEnvironment *interfaz = ventana->getGUIEnvironment();

    // -----------------------------
    //  INTERFAZ
    // -----------------------------
    interfaz->addStaticText(L"ME GACODELKJAHNRE", rect<int>(10, 10, 200, 22), true);

    // -----------------------------
    //  GEOMETRIA
    // -----------------------------
    IMeshSceneNode *nodoCubo = escena->addCubeSceneNode(5.f);
    escena->getMeshManipulator()->setVertexColors(nodoCubo->getMesh(), SColor(100, 255, 0, 0));
    nodoCubo->setPosition(vector3df(10, 0, 10));
    nodoCubo->setMaterialFlag(EMF_LIGHTING, false);

    // -----------------------------
    //  IMPORTAR MALLA
    // -----------------------------
    //Mapa
    ventana->getFileSystem()->addFileArchive("./assets/map-20kdm2.pk3"); //Carga el arbol de elementos
    IAnimatedMesh *mapa = escena->getMesh("20kdm2.bsp"); // Elige del arbol de elementos el mapa
    ISceneNode *nodoPadre = 0;

    if(mapa){
        nodoPadre = escena->addOctreeSceneNode(mapa->getMesh(0), 0, -1, 1024);
    }

    //RISAS
    //ISceneNodeAnimator *rotacionmapa = escena->createRotationAnimator(vector3df(1,1,1));
    //nodoPadre->addAnimator(rotacionmapa);

    //Personaje
    IAnimatedMesh *malla = escena->getMesh("./assets/sydney.md2"); //Archivos validos: Quake2 (.md2), Maya(.obj), Quake3 map(.bsp), Milkshape(.ms3d)
    IAnimatedMeshSceneNode *nodoPJ = escena->addAnimatedMeshSceneNode(malla);
    nodoPJ->setScale(vector3df(0.5, 0.5, 0.5));

    if (nodoPJ)
    {
        nodoPJ->setMaterialFlag(EMF_LIGHTING, false);
        nodoPJ->setFrameLoop(0, 310); //Carga los frames del 0 al 310 (todos)
        nodoPJ->setMaterialTexture(0, driver->getTexture("./assets/sydney.bmp"));
    }

    // -----------------------------
    //  CAMARAS
    // -----------------------------

    //Camara Ortogonal
    //vector3df camara_posicion(-10, 40, -10);
    //escena->addCameraSceneNode(0, camara_posicion, nodoPJ->getPosition());

    //Camara FPS
    escena->addCameraSceneNodeFPS();
    ventana->getCursorControl()->setVisible(true); // Para no ver el raton

    // -----------------------------
    //  TIEMPO
    // -----------------------------
    //Delta time
    u32 antes = ventana->getTimer()->getTime();

    //FPS
    int fpsAntes = -1;

    // -----------------------------
    //  GAME LOOP
    // -----------------------------
    while (ventana->run())
    {
        if (ventana->isWindowActive())
        {
            //Actualizar el valor del delta time
            const u32 ahora = ventana->getTimer()->getTime();
            const f32 delta = (f32)(ahora - antes) / 1000.f;
            antes = ahora;

            if (teclado.isKeyDown(KEY_ESCAPE))
            {
                ventana->closeDevice();
                return 0;
            }

            driver->beginScene(true, true, SColor(255, 100, 101, 140));

            escena->drawAll();
            interfaz->drawAll();

            driver->endScene();

            // Calcular los fps
            int fpsAhora = driver->getFPS();
            if(fpsAntes != fpsAhora){
                stringw titulo = L"Age Of Karts [";
                titulo += driver->getName();
                titulo +="] FPS: ";
                titulo += fpsAhora;

                ventana->setWindowCaption(titulo.c_str());
                fpsAntes = fpsAhora;
            }
        }
        else
        {
            // Para el renderizado si la ventana no esta activa
            ventana->yield();
        }
    }

    ventana->drop(); // Se elimina el dispositivo de irrlicht

    return 0;
}
