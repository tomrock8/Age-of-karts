#include "EscenaPodio.hpp"


EscenaPodio::EscenaPodio(Escena::tipo_escena tipo, std::vector<Corredor::tipo_jugador> jugadores) : Escena(tipo) {

	camera = new cameraThird("camara_jugador3apersona", "escena_raiz");


	animacionPirata = nullptr;
	animacionVikingo = nullptr;
	animacionChino = nullptr;
	animacionGladiador = nullptr;

	for (int i = 0; i < jugadores.size(); i++) {
		switch (jugadores.at(i)) {
		case Corredor::tipo_jugador::PIRATA:
			personajes.push_back(TMotor::instancia().newMeshNode("Pirata", " ", "escena_raiz", false));

			if (i == 0) {
				personajes.at(i)->setPosition(0, 5.5, 0);
			}
			else if (i == 1) {
				personajes.at(i)->setPosition(-3, 4, 0);
			}
			else if (i == 2) {
				personajes.at(i)->setPosition(3, 4, 0);
			}
			//personajes.at(i)->rotate(glm::vec3(0, 1, 0), 180);
			personajes.at(i)->setScale(0.5, 0.5, 0.5);
			animacionPirata = TMotor::instancia().createAnimationNode(personajes.at(i)->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Pirata/Celebracion/PirataCelebracion_000", 122, 242), "BailePirata");
			personajes.at(i)->setNode(animacionPirata);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setVisible(true);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setPlaying(true);
			cout << "El PRIMERO ES EL PIRATA" << endl;
			break;

		case Corredor::tipo_jugador::VIKINGO:
			personajes.push_back(TMotor::instancia().newMeshNode("Vikingo", " ", "escena_raiz", false));
			if (i == 0) {
				personajes.at(i)->setPosition(0, 5.5, 0);
			}
			else if (i == 1) {
				personajes.at(i)->setPosition(-3, 4, 0);
			}
			else if (i == 2) {
				personajes.at(i)->setPosition(3, 4, 0);
			}
			//personajes.at(i)->rotate(glm::vec3(0, 1, 0), 180);
			personajes.at(i)->setScale(0.5, 0.5, 0.5);
			animacionVikingo = TMotor::instancia().createAnimationNode(personajes.at(i)->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Vikingo/Celebracion/VikingoCelebracion_000", 443, 537), "BaileVikingo");
			personajes.at(i)->setNode(animacionVikingo);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setVisible(true);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setPlaying(true);
			cout << "El PRIMERO ES EL VIKINGO" << endl;
			break;

		case Corredor::tipo_jugador::GLADIADOR:
			personajes.push_back(TMotor::instancia().newMeshNode("Gladiador", " ", "escena_raiz", false));
			if (i == 0) {
				personajes.at(i)->setPosition(0, 5.5, 0);
			}
			else if (i == 1) {
				personajes.at(i)->setPosition(-3, 4, 0);
			}
			else if (i == 2) {
				personajes.at(i)->setPosition(3, 4, 0);
			}
			//personajes.at(i)->rotate(glm::vec3(0, 1, 0), 180);
			personajes.at(i)->setScale(0.5, 0.5, 0.5);
			animacionGladiador = TMotor::instancia().createAnimationNode(personajes.at(i)->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Gladiador/Celebracion/GladiadorCelebracion_000", 178, 263), "BaileGladiador");
			personajes.at(i)->setNode(animacionGladiador);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setVisible(true);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setPlaying(true);
			cout << "El PRIMERO ES EL GLADIADOR" << endl;
			break;

		case Corredor::tipo_jugador::CHINO:
			personajes.push_back(TMotor::instancia().newMeshNode("Chino", " ", "escena_raiz", false));
			if (i == 0) {
				personajes.at(i)->setPosition(0, 5.5, 0);
			}
			else if (i == 1) {
				personajes.at(i)->setPosition(-3, 4, 0);
			}
			else if (i == 2) {
				personajes.at(i)->setPosition(3, 4, 0);
			}
			//personajes.at(i)->rotate(glm::vec3(0, 1, 0), 180);
			personajes.at(i)->setScale(0.5, 0.5, 0.5);
			animacionChino = TMotor::instancia().createAnimationNode(personajes.at(i)->getNode()->getPadre(), TMotor::instancia().createAnimation("assets/Animacion/Chino/Celebracion/ChinoCelebracion_000", 228, 317), "BaileChino");
			personajes.at(i)->setNode(animacionChino);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setVisible(true);
			static_cast<TAnimacion*>(personajes.at(i)->getNode()->getEntidad())->setPlaying(true); cout << "El PRIMERO ES EL CHINO" << endl;
			break;

		default:
			break;
		}



	}


	incremento = 0;

	pulsadoa = false;
	pulsadoz = false;
	pulsadow = false;
	pulsados = false;
	pulsadod = false;
	pulsadoc = false;

	aumento = false;
	decremento = false;

	x = 0;
	y = 0;
	z = 0;

	posxPodio = 10;
	posyPodio = 4;
	poszPodio = 13;

	altura = 4;

	podio = TMotor::instancia().newMeshNode("podio", "assets/MapaCelebracion/victoria.obj", "escena_raiz", false);

	init();
}

EscenaPodio::~EscenaPodio() {

}

Escena::tipo_escena EscenaPodio::comprobarInputs() {

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_D) == GLFW_PRESS) {

		pulsadod = true;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_A) == GLFW_PRESS) {

		pulsadoa = true;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W) == GLFW_PRESS) {

		pulsadow = true;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_Z) == GLFW_PRESS) {

		pulsadoz = true;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_C) == GLFW_PRESS) {

		pulsadoc = true;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_S) == GLFW_PRESS) {

		pulsados = true;
	}

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_D) == GLFW_RELEASE && pulsadod) {
		z += 0.5;
		pulsadod = false;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_A) == GLFW_RELEASE && pulsadoa) {
		x += 0.5;
		pulsadoa = false;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W) == GLFW_RELEASE && pulsadow) {
		y += 0.5;
		pulsadow = false;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_Z) == GLFW_RELEASE && pulsadoz) {
		x -= 0.5;
		pulsadoz = false;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_C) == GLFW_RELEASE && pulsadoc) {
		z -= 0.5;
		pulsadoc = false;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_S) == GLFW_RELEASE && pulsados) {
		y -= 0.5;
		pulsados = false;
	}

	//cout<<"POSICION EN X::::"<< x <<endl;
	//cout<<"POSICION EN Z::::"<< z <<endl;
	//cout<<"POSICION EN Y::::"<< y <<endl;




	return Escena::tipo_escena::PODIO;

}

void EscenaPodio::init() {


	TMotor::instancia().initDebugWindow();


	podio->setPosition(0, 0, 0);
	podio->setScale(1, 1, 1);
	camera->lookAt(glm::vec3(podio->getPosition().x, podio->getPosition().y + altura, podio->getPosition().z));

	// LUCES PUNTUALES
	luces.push_back(TMotor::instancia().newLightNode("light_0", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, true, "escena_raiz"));
	luces.at(0)->translate(glm::vec3(100.0f, 700.0f, 0.0f));

	luces.push_back(TMotor::instancia().newLightNode("light_1", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz"));
	luces.at(1)->translate(glm::vec3(0.0f, 1500.0f, 0.0f));

	luces.push_back(TMotor::instancia().newLightNode("light_2", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz"));
	luces.at(2)->translate(glm::vec3(-150.0f, 12.0f, -300.0f));

	luces.push_back(TMotor::instancia().newLightNode("light_3", glm::vec4(-1.0f, -1.f, -1.0f, 0.0f), 0.000000001f, glm::cos(glm::radians(60.0f)), false, false, "escena_raiz"));
	luces.at(3)->translate(glm::vec3(0.0f, 12.0f, 300.0f));
	//-----------------------------
	//	PARTICULAS
	//-----------------------------
	particulas.push_back(TMotor::instancia().newParticleSystem());
	particulas.push_back(TMotor::instancia().newParticleSystem());
	particulas.push_back(TMotor::instancia().newParticleSystem());
	particulas.push_back(TMotor::instancia().newParticleSystem());
	particulas.at(0)->setPosition(glm::vec3(-8.5, 0.2, -8));
	particulas.at(0)->setSize(0.15f);
	particulas.at(0)->setType(1);
	particulas.at(1)->setPosition(glm::vec3(10, 0.2, -8));
	particulas.at(1)->setSize(0.15f);
	particulas.at(1)->setType(1);
	particulas.at(2)->setPosition(glm::vec3(10, 0.2, 10));
	particulas.at(2)->setSize(0.15f);
	particulas.at(2)->setType(1);
	particulas.at(3)->setPosition(glm::vec3(-8.5, 0.2, 10));
	particulas.at(3)->setSize(0.15f);
	particulas.at(3)->setType(1);

}

void EscenaPodio::dibujar() {
	//cout << "dibujando el podio" << endl;
	//Limpiamos el dibujado anterior asignando un color de fondo
	TMotor::instancia().clean(0.16f, 0.533f, 0.698f, 0.0f);
	TMotor::instancia().drawSkybox();
	TMotor::instancia().draw();
	TMotor::instancia().drawParticles();
	TMotor::instancia().drawIMGUI();
}

void EscenaPodio::limpiar() {

}

void EscenaPodio::update() {

	float velocidad = 0.05;


	if (incremento <= -20) {
		aumento = true;
		decremento = false;
		//cout<<"voy a sumar"<<endl;
	}
	else if (incremento >= 0) {
		decremento = true;
		aumento = false;
		//cout<<"voy a restar"<<endl;
	}


	if (aumento)
		incremento += velocidad;
	else if (decremento)
		incremento -= velocidad;

	//cout<<"INCREMENTO"<<incremento<<endl;

	camera->setPositionStatic(glm::vec3(posxPodio + incremento, posyPodio, poszPodio));
	camera->lookAt(glm::vec3(podio->getPosition().x, podio->getPosition().y + altura, podio->getPosition().z));
	TMotor::instancia().setActiveCamera(camera->getNodo()->getNode());
}