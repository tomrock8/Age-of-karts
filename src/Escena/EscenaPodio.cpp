#include "EscenaPodio.hpp"


EscenaPodio::EscenaPodio(Escena::tipo_escena tipo, std::vector<Corredor::tipo_jugador> jugadores) : Escena(tipo) {

	camera = new cameraThird("camara_jugador3apersona", "escena_raiz");
	//obj3D para las poles
	primero = TMotor::instancia().newMeshNode("primeraPosicion", " ", "escena_raiz", false); //textura cualquiera pues se eliminara
	segundo = TMotor::instancia().newMeshNode("segundaPosicion", " ", "escena_raiz", false); //textura cualquiera pues se eliminara
	tercero = TMotor::instancia().newMeshNode("terceraPosicion", " ", "escena_raiz", false); //textura cualquiera pues se eliminara
	primero->setPosition(0, 6.5, 0);
	primero->setScale(0.75, 0.75, 0.75);
	segundo->setPosition(-5.06887, 3.83675, 0);
	segundo->setScale(0.75, 0.75, 0.75);
	tercero->setPosition(5.09683, 3.83675, 0);
	tercero->setScale(0.75, 0.75, 0.75);
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
	time= Timer::getInstancia();
	end=false;
	tm=time->getTimer();

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
					animacionVikingo = TMotor::instancia().createAnimationNode(NULL, TMotor::instancia().createAnimation("assets/Animacion/Vikingo/Celebracion/vikingoCelebracionobj_000", 462, 547), "BaileVikingo");

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
	entra=false;
	podio = TMotor::instancia().newMeshNode("podio", "assets/MapaCelebracion/victoria.obj", "escena_raiz", false);

	init();
}

EscenaPodio::~EscenaPodio() {

	
	delete podio;

    camera = nullptr;
	
	for(int i =0; i< luces.size();i++){
		delete luces.at(i);
	}

	particulas.clear();		
	
	delete primero;
	delete segundo;
	delete tercero;

	animacionPirata=nullptr;
	animacionVikingo=nullptr;
	animacionChino=nullptr;
	animacionGladiador=nullptr;
	animacionElegida=nullptr;
	
	delete time;


}

Escena::tipo_escena EscenaPodio::comprobarInputs() {
	bool mandoConectado = false;
	const unsigned char *buttons = nullptr;
	const float *axes = nullptr;
	int id=0;
	
	if (1 == glfwJoystickPresent(id)) {
		mandoConectado = true;
		int buttonCount;
		buttons = glfwGetJoystickButtons(id, &buttonCount);
		int axesCount;
		axes = glfwGetJoystickAxes(id, &axesCount);
		//  || (mandoConectado && (GLFW_PRESS == buttons[12] || 0.5f <= axes[0]) ) 
	}
	if (time->getTimer()-tm>4){
		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_RELEASE && end == true) {

			return Escena::tipo_escena::MENU; // Devuelve el estado de las escenas para que salga
		}

		if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ENTER) == GLFW_PRESS || (mandoConectado && (GLFW_PRESS == buttons[0]))) {
			return Escena::tipo_escena::MENU;
		}else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_ESCAPE) == GLFW_PRESS || (mandoConectado && (GLFW_PRESS == buttons[1]) )) {
			end = true;
		}
	}

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
	particulas.at(0)->setColor(glm::vec3(1.0, 0.0, 0.0));
	particulas.at(1)->setPosition(glm::vec3(10, 0.2, -8));
	particulas.at(1)->setSize(0.15f);
	particulas.at(1)->setType(1);
	particulas.at(1)->setColor(glm::vec3(0.0, 0.0, 1.0));
	particulas.at(2)->setPosition(glm::vec3(10, 0.2, 10));
	particulas.at(2)->setSize(0.15f);
	particulas.at(2)->setType(1);
	particulas.at(2)->setColor(glm::vec3(0.0, 1.0, 0.0));
	particulas.at(3)->setPosition(glm::vec3(-8.5, 0.2, 10));
	particulas.at(3)->setSize(0.15f);
	particulas.at(3)->setType(1);
	particulas.at(3)->setColor(glm::vec3(1.0, 0.5, 0.0));






}

void EscenaPodio::dibujar() {
	TMotor::instancia().setViewport(0, 0, TMotor::instancia().getWidth(), TMotor::instancia().getHeight()); //Pantalla completa
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
	
	if ((time->getTimer()-tm)>2 && !entra){
		entra=true;
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