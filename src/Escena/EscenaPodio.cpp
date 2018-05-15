#include "EscenaPodio.hpp"


EscenaPodio::EscenaPodio(Escena::tipo_escena tipo, std::vector<Corredor::tipo_jugador> jugadores) : Escena(tipo) {
	cout << "ESTOY EN EL PODIO" << endl;


	for (int i = 0; i < jugadores.size(); i++) {
		switch (jugadores.at(i)) {
		case Corredor::tipo_jugador::PIRATA:
			personajes.push_back(TMotor::instancia().newMeshNode("Jugador", "assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false));
			personajes.at(0)->setPosition(0, 0, 0);

			cout << "Posicion de jugador seteada" << endl;

			cout << "El PRIMERO ES EL PIRATA" << endl;
			break;

		case Corredor::tipo_jugador::VIKINGO:
			//personajes.at(i) = TMotor::instancia().newMeshNode("Jugador", "assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false); 
			cout << "El PRIMERO ES EL VIKINGO" << endl;
			break;

		case Corredor::tipo_jugador::GLADIADOR:
			//personajes.at(i) = TMotor::instancia().newMeshNode("Jugador", "assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false);  
			cout << "El PRIMERO ES EL GLADIADOR" << endl;
			break;

		case Corredor::tipo_jugador::CHINO:
			//personajes.at(i) = TMotor::instancia().newMeshNode("Jugador", "assets/Karts/Vikingo/vikingoConIzq.obj", "escena_raiz", false);  
			cout << "El PRIMERO ES EL CHINO" << endl;
			break;

		default:
			break;
		}
	}

	cout << "Posicion de jugador seteada" << endl;
	init();
}

EscenaPodio::~EscenaPodio() {

}

Escena::tipo_escena EscenaPodio::comprobarInputs() {

	//cout<<"estoy comprobando el input"<<endl;

	camera->lookAt(glm::vec3(0, 0, 0));
	//cout<<"lookAT"<<endl;

	return Escena::tipo_escena::PODIO;

}

void EscenaPodio::init() {
	// LUCES PUNTUALES
	luzPuntual_0 = TMotor::instancia().newLightNode("light_0", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, true, "escena_raiz");
	luzPuntual_0->translate(glm::vec3(100.0f, 700.0f, 0.0f));

	luzPuntual_1 = TMotor::instancia().newLightNode("light_1", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzPuntual_1->translate(glm::vec3(0.0f, 1500.0f, 0.0f));

	luzPuntual_2 = TMotor::instancia().newLightNode("light_2", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzPuntual_2->translate(glm::vec3(-150.0f, 12.0f, -300.0f));

	luzPuntual_3 = TMotor::instancia().newLightNode("light_3", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz");
	luzPuntual_3->translate(glm::vec3(0.0f, 12.0f, 300.0f));
	//-----------------------------
	//	ESCENARIO MAPA
	//-----------------------------
	podio = TMotor::instancia().newMeshNode("podio", "assets/MapaCelebracion/victoria.obj", "escena_raiz", false);
	camera = new cameraThird("camara_jugador3apersonaPodio", "escena_raiz");
	camera->setPositionStatic(glm::vec3(0, 5, -40));
	camera->setRotation(glm::vec3(0, 1, 0), 180);

}

void EscenaPodio::dibujar() {
	cout << "dibujando el podio" << endl;
	TMotor::instancia().draw();

}

void EscenaPodio::limpiar() {

}

void EscenaPodio::update() {

}