#include "Camara3persona.hpp"

Camara3persona::Camara3persona() {
	camera = Motor3d::instancia().getScene()->addCameraSceneNode(0, core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 0.0f, 0.0f), -1);
	direction = 0;
	zdirection = 0;
	XCamera = 0;
	YCamera = 5;
	ZCamera = -10;
}

Camara3persona::~Camara3persona() {
	cout << "\nENTRO AL DESTRUCTOR DE CAMARA\n";
	//delete camera;
	cout << "\nSALGO DEL DESTRUCTOR DE CAMARA\n";
}

void Camara3persona::moveCamera(Corredor * pj1){
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	CTeclado *teclado = CTeclado::getInstancia();
	vector3df RelativeToCar;
	if (teclado->isKeyDown(KEY_KEY_Q) && XCamera >= 0) {
		if (XCamera < 30) {
			XCamera++;
			ZCamera += 0.5;
		}
	}
	else if (teclado->isKeyDown(KEY_KEY_E) && XCamera <= 0) {
		if (XCamera > -30) {
			XCamera--;
			ZCamera += 0.5;
		}
	}
	else if (XCamera < 0) {
		XCamera++;
		ZCamera -= 0.5;
	}
	else if (XCamera > 0) {
		XCamera--;
		ZCamera -= 0.5;
	}
	RelativeToCar.X = XCamera;
	RelativeToCar.Y = 5;
	RelativeToCar.Z = ZCamera;
	(XCamera, 5, ZCamera);
	if(teclado->isKeyDown(KEY_KEY_L))
		RelativeToCar.Z=-ZCamera;

	vector3df RelativeToCarTarget(0,1,0);
	pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCar);
	pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCarTarget);
	RelativeToCar.Y = 5;
	RelativeToCarTarget.Y = 1;
	camera->setPosition(RelativeToCar);
	camera->setTarget(RelativeToCarTarget);
	fpsActiva = false;
}

void Camara3persona::movefpsCamera(Corredor * pj1){
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	vector3df RelativeToCar(0,1,0);
	vector3df RelativeToCarTarget(0,10,20);
	pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCar);
	pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCarTarget);
	RelativeToCarTarget.Y = 10;
	//RelativeToCar.Y = 1;
	camera->setPosition(RelativeToCar);
	camera->setTarget(RelativeToCarTarget);
	/*
	if(!fpsActiva){
		camera = Motor3d::instancia().getScene()->addCameraSceneNodeFPS(pj1->getNodo());
		fpsActiva = true;
	}*/
}

void Camara3persona::moveCameraControl(Corredor *pj1) {
	CTeclado *teclado = CTeclado::getInstancia();
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	core::vector3df cameraPos = camera->getAbsolutePosition();

	float change_x = 0.0f;
	float change_y = 0.0f;


	if (teclado->isKeyDown(KEY_UP)) {
		change_y = 1.0f;
	}
	else {
		if (teclado->isKeyDown(KEY_DOWN)) {
			change_y = -1.0f;
		}
	}

	if (teclado->isKeyDown(KEY_LEFT)) {
		change_x = -1.0f;
	}
	else {
		if (teclado->isKeyDown(KEY_RIGHT)) {
			change_x = 1.0f;
		}
	}

	direction += change_x;
	zdirection -= change_y;
	if (zdirection < -90)
		zdirection = -90;
	else if (zdirection > 90)
		zdirection = 90;


	core::vector3df playerPos = pj1->getNodo()->getPosition();

	float xf = playerPos.X - cos(direction * PI / 180.0f) * 64.0f;
	float yf = playerPos.Y - sin(zdirection * PI / 180.0f) * 64.0f;
	float zf = playerPos.Z + sin(direction * PI / 180.0f) * 64.0f;

	camera->setPosition(core::vector3df(xf, yf, zf));
	camera->setTarget(core::vector3df(playerPos.X, playerPos.Y + 25.0f, playerPos.Z));
	//pj1->getNodo()->setRotation( core::vector3df( 0, direction, 0 ) );
	fpsActiva = false;
}

void Camara3persona::moveCameraControlPointer(Corredor *pj1) {
	core::position2d<f32> cursorPos = Motor3d::instancia().getDevice()->getCursorControl()->getRelativePosition();
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	core::vector3df cameraPos = camera->getAbsolutePosition();

	float change_x = (cursorPos.X - 0.5) * 256.0f;
	float change_y = (cursorPos.Y - 0.5) * 256.0f;

	direction += change_x;
	zdirection -= change_y;

	if (zdirection < -90)
		zdirection = -90;
	else if (zdirection > 90)
		zdirection = 90;
	Motor3d::instancia().getDevice()->getCursorControl()->setPosition(0.5f, 0.5f);

	core::vector3df playerPos = pj1->getNodo()->getPosition();

	float xf = playerPos.X - cos(direction * PI / 180.0f) * 64.0f;
	float yf = playerPos.Y - sin(zdirection * PI / 180.0f) * 64.0f;
	float zf = playerPos.Z + sin(direction * PI / 180.0f) * 64.0f;

	camera->setPosition(core::vector3df(xf, yf, zf));
	camera->setTarget(core::vector3df(playerPos.X, playerPos.Y + 25.0f, playerPos.Z));
	//pj1->getNodo()->setRotation( core::vector3df( 0, direction, 0 ) );
	fpsActiva = false;
}