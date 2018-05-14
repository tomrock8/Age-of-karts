#include "EscenaPodio.hpp"


EscenaPodio::EscenaPodio(Escena::tipo_escena tipo,std::vector<Corredor::tipo_jugador> jugadores) : Escena(tipo){
    cout<<"ESTOY EN EL PODIO"<<endl;
    

    for(int i=0; i<jugadores.size();i++){


        switch(jugadores.at(i)){

            case Corredor::tipo_jugador::PIRATA:

        personajes.push_back(TMotor::instancia().newMeshNode("Jugador","assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false)); 
         personajes.at(0)->setPosition(0,0,0);

         cout<<"Posicion de jugador seteada"<<endl;
        
        cout<<"PIRATA"<<endl;   
            break;

            case Corredor::tipo_jugador::VIKINGO:
        //personajes.at(i) = TMotor::instancia().newMeshNode("Jugador", "assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false); 
        cout<<"VIKINGO"<<endl;     
            break;

            case Corredor::tipo_jugador::GLADIADOR:
        //personajes.at(i) = TMotor::instancia().newMeshNode("Jugador", "assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false);  
        cout<<"GLADIADOR"<<endl;     
            break;

            case Corredor::tipo_jugador::CHINO:
        //personajes.at(i) = TMotor::instancia().newMeshNode("Jugador", "assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false);  
        cout<<"CHINO"<<endl;     
            break;

            default:

            break;

        }

    }

   
    camera = new cameraThird("camara_jugador3apersona","escena_raiz");
    camera->setPositionStatic(glm::vec3(0,5,10));
    cout<<"Posicion de camara seteada"<<endl;
    
    
/*
    //-----------------------------
	//	ESCENARIO MAPA
	//-----------------------------
	podio = TMotor::instancia().newMeshNode("podio", objeto, "escena_raiz", false);
*/

}

EscenaPodio::~EscenaPodio(){

}

Escena::tipo_escena EscenaPodio::comprobarInputs(){

//cout<<"estoy comprobando el input"<<endl;


//cout<<"lookAT"<<endl;

return Escena::tipo_escena::PODIO;

}

void EscenaPodio::init() {

}

void EscenaPodio::dibujar() {

}

void EscenaPodio::limpiar() {

}

void EscenaPodio::update() {
camera->lookAt(glm::vec3(0,0,0));
}