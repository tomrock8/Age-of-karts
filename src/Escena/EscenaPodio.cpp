#include "EscenaPodio.hpp"


EscenaPodio::EscenaPodio(Escena::tipo_escena tipo, std::vector<Corredor::tipo_jugador> jugadores) : Escena(tipo) {

	camera = new cameraThird("camara_jugador3apersona", "escena_raiz");
	//obj3D para las poles
	primero = TMotor::instancia().newMeshNode("primeraPosicion", " ", "escena_raiz", false); //textura cualquiera pues se eliminara
	segundo = TMotor::instancia().newMeshNode("segundaPosicion", " ", "escena_raiz", false); //textura cualquiera pues se eliminara
	tercero = TMotor::instancia().newMeshNode("terceraPosicion", " ", "escena_raiz", false); //textura cualquiera pues se eliminara
	primero->setPosition(0, 3.83675, 0);
	segundo->setPosition(-5.06887, 7.40514, 0);
	tercero->setPosition(5.09683, 7.50068, 0);
	//comprobaciones de puesto
	segundoPuesto = false;
	tercerPuesto = false;
	//animaciones
	//padre inicializado a cualquiera , cuando se sepa donde va a ir se le metera el padre correspondiente
	animacionPirata = nullptr;
	animacionVikingo = nullptr;
	animacionGladiador = nullptr;
	animacionChino = nullptr;
	animacionElegida = nullptr;
	for (int i = 0; i < jugadores.size(); i++) {

		switch (jugadores.at(i)) {//recogemos cual sera la animacion que tocara en caso de que esten en el podio
		case Corredor::tipo_jugador::PIRATA:
			if (animacionPirata == nullptr) {
				animacionPirata = TMotor::instancia().createAnimationNode(NULL, TMotor::instancia().createAnimation("assets/Animacion/Pirata/Celebracion/PirataCelebracion_000", 122, 242), "BailePirata");
			}
			animacionElegida = animacionPirata;
			break;

		case Corredor::tipo_jugador::VIKINGO:
			if (animacionVikingo == nullptr) {
				animacionVikingo = TMotor::instancia().createAnimationNode(NULL, TMotor::instancia().createAnimation("assets/Animacion/Vikingo/Celebracion/VikingoCelebracion_000", 443, 537), "BaileVikingo");

			}
			animacionElegida = animacionVikingo;
			break;

		case Corredor::tipo_jugador::GLADIADOR:
			if (animacionGladiador == nullptr) {
				animacionGladiador = TMotor::instancia().createAnimationNode(NULL, TMotor::instancia().createAnimation("assets/Animacion/Gladiador/Celebracion/GladiatorCelebracion_000", 435, 594), "BaileGladiador");

			}
			animacionElegida = animacionGladiador;
			break;

		case Corredor::tipo_jugador::CHINO:
			if (animacionChino == nullptr) {
				animacionChino = TMotor::instancia().createAnimationNode(NULL, TMotor::instancia().createAnimation("assets/Animacion/Chino/Celebracion/ChinoCelebracion_000", 228, 317), "BaileChino");
			}
			animacionElegida = animacionChino;
			break;
		default:
			break;
		}
		if (i == 0) {//asignamos el nodo padre en funcion de su posicion y despues le asignamos el nodo a el objeto 
			TMotor::instancia().addPadre(primero->getNode()->getPadre(), animacionElegida);
			primero->setNode(animacionElegida);
		}
		else if (i == 1) {
			TMotor::instancia().addPadre(segundo->getNode()->getPadre(), animacionElegida);
			segundo->setNode(animacionElegida);
			segundoPuesto = true;
		}
		else if (i == 2) {
			TMotor::instancia().addPadre(tercero->getNode()->getPadre(), animacionElegida);
			tercero->setNode(animacionElegida);
			tercerPuesto = true;
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
	particulas.at(0)->setPosition(glm::vec3(-8.5, 1, -8));
	particulas.at(1)->setPosition(glm::vec3(10, 1, -8));
	particulas.at(2)->setPosition(glm::vec3(10, 1, 10));
	particulas.at(3)->setPosition(glm::vec3(-8.5, 1, 10));
	//activamos animaciones
	//primero
	static_cast<TAnimacion*>(primero->getNode()->getEntidad())->setVisible(true);
	static_cast<TAnimacion*>(primero->getNode()->getEntidad())->setPlaying(true);
	//segundo
	if (segundoPuesto) {
		static_cast<TAnimacion*>(segundo->getNode()->getEntidad())->setVisible(true);
		static_cast<TAnimacion*>(segundo->getNode()->getEntidad())->setPlaying(true);
	}
	//tercero
	if (tercerPuesto) {
		static_cast<TAnimacion*>(tercero->getNode()->getEntidad())->setVisible(true);
		static_cast<TAnimacion*>(tercero->getNode()->getEntidad())->setPlaying(true);
	}




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