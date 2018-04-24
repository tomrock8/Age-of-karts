#include "CorredorJugador.hpp"
#include "Client.hpp"

CorredorJugador::CorredorJugador(btVector3 pos, Corredor::tipo_jugador tipo) : Corredor(pos, tipo) {
	checkItem = false;
	nombre = "Jugador";
	pressed = false;
}

/**
	Movimiento del jugador controlado por el teclado
	TODO: Modificar para detectar el input que se recibe.
*/
void CorredorJugador::movimiento() {

	bool comprobadorMovimiento = false;

	//Comprobador de de mando y recoleccion de inputs
	bool mandoConectado = false;
	const unsigned char *buttons = nullptr;
	const float *axes = nullptr;
	if (1 == glfwJoystickPresent(id)) {
		mandoConectado = true;
		int buttonCount;
		buttons = glfwGetJoystickButtons(id, &buttonCount);
		int axesCount;
		axes = glfwGetJoystickAxes(id, &axesCount);
		//  || (mandoConectado && (GLFW_PRESS == buttons[12] || 0.5f <= axes[0]) ) 
	}

	int a = glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W);

	//-------ENTRADA TECLADO ----------//
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_S) == GLFW_PRESS || (mandoConectado && -0.5f <= axes[2])) {
		frenar();
		//vehiculo-> set
		comprobadorMovimiento = true;

	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_W) == GLFW_PRESS || (mandoConectado && -0.5f <= axes[5])) {

		acelerar();
		comprobadorMovimiento = true;

	}
	//GIRAR DERECHA
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_D) == GLFW_PRESS || (mandoConectado && (GLFW_PRESS == buttons[12] || 0.5f <= axes[0]))) {
		girarDerecha();

		comprobadorMovimiento = true;

	}
	//GIRAR IZQUIERDA
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_A) == GLFW_PRESS || (mandoConectado && (GLFW_PRESS == buttons[11] || -0.5f >= axes[0]))) {
		girarIzquierda();
		comprobadorMovimiento = true;
	}
	else {
		estado->setDireccionMovimiento(EstadosJugador::direccion_movimiento::RECTO);
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_SPACE) == GLFW_PRESS || (mandoConectado && GLFW_PRESS == buttons[1])) {
		frenodemano(true, false);
		comprobadorMovimiento = true;
	}
	else {
		frenodemano(false, false);
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_R) == GLFW_PRESS || (mandoConectado && GLFW_PRESS == buttons[5])) {
		recolocarWaypoint();
	}

	if (!comprobadorMovimiento) {
		if (!turboActivado) {
			desacelerar();
		}
	}

}

void CorredorJugador::updateHijos() {}

void CorredorJugador::actualizarItem() {

	int direccionItem = 1;
	setPosDisparo(btVector3(cuboNodo->getPosition().x + orientacion.getX() * 10, cuboNodo->getPosition().y, cuboNodo->getPosition().z + orientacion.getZ() * 10));
	btVector3 posDisparo2(cuboNodo->getPosition().x, cuboNodo->getPosition().y - 1, cuboNodo->getPosition().z);

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_1) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::FLECHA);
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_2) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::CAJA_FALSA);
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_3) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::TURBO);
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_4) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::ACEITE);
			pressed = true;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_5) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::ESCUDO);
			pressed = true;//escudo
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_6) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::FLECHA_TRIPLE);
			pressed = true;//proytectil x3
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_7) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::ITEM_TELEDIRIGIDO);
			pressed = true;//FlechaTeledirigida
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_8) == GLFW_PRESS) {
		if (!pressed) {
			setTipoObj(EstadosJugador::estado_objeto::TURBO_TRIPLE);
			pressed = true;//TurboTriple
		}
	}
	else {
		pressed = false;
	}

	//Comprobador de de mando y recoleccion de inputs
	bool mandoConectado = false;
	const unsigned char *buttons = nullptr;
	if (1 == glfwJoystickPresent(id))
	{
		mandoConectado = true;
		int buttonCount;
		buttons = glfwGetJoystickButtons(id, &buttonCount);
		//  || (mandoConectado && GLFW_PRESS == buttons[12]) 
	}

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_P) == GLFW_PRESS || (mandoConectado && GLFW_PRESS == buttons[0]))
	{
		if (getTipoObj() != 0 && !checkItem)
		{
			setCheckItem(true);
			//Para saber si se ha lanzado el item desde un jugador en red
			Client *client = Client::getInstancia();
			if (client->getConnected())
				client->PlayerThrowObject();
			//Llama a la funcion de la clase padre
			usarObjetos();
		}
	}
	else {
		if (getCheckItem() == true) {
			setCheckItem(false);
		}
	}
	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_O) == GLFW_PRESS || (mandoConectado && GLFW_PRESS == buttons[2])) {
		lanzarHabilidad();
	}
}

bool CorredorJugador::setComprobadorMovimiento(bool s) {
	comprobadorMovimiento = s;
	return comprobadorMovimiento;
}