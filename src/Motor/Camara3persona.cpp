#include "Camara3persona.hpp"

Camara3persona::Camara3persona() {
	camera = Motor3d::instancia().getScene()->addCameraSceneNode(0, core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 0.0f, 0.0f), -1);
	direction = 0;
	zdirection = 0;
	XCamera3 = 0;
	YCamera3 = 5;
	ZCamera3 = -10;
	XCamera1 = 0;
	YCamera1 = 10;
	ZCamera1 = 20;
	auxX=0;
	rigidBody=inicializarFisicas();
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
}

Camara3persona::~Camara3persona() {
	cout << "\nENTRO AL DESTRUCTOR DE CAMARA\n";
	//delete camera;
	cout << "\nSALGO DEL DESTRUCTOR DE CAMARA\n";
}

btRigidBody *Camara3persona::inicializarFisicas()
{
	btVector3 escala(5,5,5);
	btVector3 posicion(-10, 10, 31);
	btScalar masa=btScalar(10);
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	//vector<btRigidBody *> objetos = bullet->getObjetos();
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	
	Transform.setOrigin(posicion);
	MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(escala.getX() , escala.getY(), escala.getZ());
	Shape = new btBoxShape(HalfExtents);

	// Add mass
	btVector3 LocalInertia;

	
	Shape->calculateLocalInertia(masa, LocalInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(masa, MotionState, Shape, LocalInertia);
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	// Store a pointer to the irrlicht node so we can update it later
	rigidBody->setUserPointer((void *)(camera));

		rigidBody->setActivationState(DISABLE_DEACTIVATION);
	// Add it to the world
	mundo->addRigidBody(rigidBody);
	//objetos.push_back(rigidBody);
	//bullet->setObjetos(objetos);
	rigidBody->setGravity(btVector3(0, 0, 0));
	return rigidBody;
}

void Camara3persona::moveCamera(Corredor * pj1){
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	vector3df RelativeToCar;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && XCamera3 >= 0) {
		if (XCamera3 < 30) {
			XCamera3++;
			ZCamera3 += 0.5;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && XCamera3 <= 0) {
		if (XCamera3 > -30) {
			XCamera3--;
			ZCamera3 += 0.5;
		}
	}
	else if (XCamera3 < 0) {
		XCamera3++;
		ZCamera3 -= 0.5;
	}
	else if (XCamera3 > 0) {
		XCamera3--;
		ZCamera3 -= 0.5;
	}
	RelativeToCar.X = XCamera3;
	RelativeToCar.Y = 5;
	RelativeToCar.Z = ZCamera3;
	(XCamera3, 5, ZCamera3);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		RelativeToCar.Z=-ZCamera3;

	float incremento = 0.1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		if(auxX < 4)
			auxX += incremento;
	}else  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		if(auxX > -4)
			auxX-= incremento;
	}else{
		if(auxX < -0.25){
			auxX+=incremento;
		}else if(auxX > 0.25){
			auxX-=incremento;
		}else{
			auxX=0;
		}
	}
	RelativeToCar.X += auxX;

	vector3df RelativeToCarTarget(0,1,0);
		
	pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCar);
	pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCarTarget);
	RelativeToCar.Y = 5;
	RelativeToCarTarget.Y = 1;
	if (!pj1->getAceiteActivado()){
 		camera->setPosition(RelativeToCar); 
		camera->setTarget(RelativeToCarTarget);
	}else{
		camera->setPosition(camera->getPosition());
		camera->setTarget(camera->getTarget());
	}
	fpsActiva = false;
}

void Camara3persona::movefpsCamera(Corredor * pj1){
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	vector3df RelativeToCar(0,1,0);
	vector3df RelativeToCarTarget;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && XCamera1 >= 0) {
		if (XCamera1 < 30) {
			XCamera1++;
			ZCamera1 -= 0.5;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && XCamera1 <= 0) {
		if (XCamera1 > -30) {
			XCamera1--;
			ZCamera1 -= 0.5;
		}
	}
	else if (XCamera1 < 0) {
		XCamera1++;
		ZCamera1 += 0.5;
	}
	else if (XCamera1 > 0) {
		XCamera1--;
		ZCamera1 += 0.5;
	}

	RelativeToCarTarget.X = XCamera1;
	RelativeToCarTarget.Y = 5;
	RelativeToCarTarget.Z = ZCamera1;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		RelativeToCarTarget.Z=-ZCamera1;
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
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	core::vector3df cameraPos = camera->getAbsolutePosition();

	float change_x = 0.0f;
	float change_y = 0.0f;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		change_y = 1.0f;
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			change_y = -1.0f;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		change_x = -1.0f;
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
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

	btVector3 btPos(xf, yf, zf);
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