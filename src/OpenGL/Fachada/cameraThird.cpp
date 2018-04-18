#include "cameraThird.hpp"
#include "TMotor.hpp"

cameraThird::cameraThird(TNodo *m, TNodo *parent, const char *n, GLuint i) {
	node = m;
	parentNode = parent;
	name = n;
	id = i;
	direction = 0;
	zdirection = 0;
	auxX = 0;
	rigidBody = initializePhysics();
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

}

cameraThird::~cameraThird() {
}

btRigidBody *cameraThird::initializePhysics() {
	btVector3 escala(5, 5, 5);
	btVector3 posicion(-10, 10, 31);
	btScalar masa = btScalar(10);
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	//std::vector<btRigidBody *> objetos = bullet->getObjetos();
	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();

	Transform.setOrigin(posicion);
	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(escala.getX(), escala.getY(), escala.getZ());
	btBoxShape *Shape = new btBoxShape(HalfExtents);

	// Add mass
	btVector3 LocalInertia;


	Shape->calculateLocalInertia(masa, LocalInertia);

	// Create the rigid body object
	btRigidBody *rigidBody = new btRigidBody(masa, MotionState, Shape, LocalInertia);
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	// Store a pointer to the irrlicht node so we can update it later
	rigidBody->setUserPointer((void *)(this->node));

	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	// Add it to the world
	mundo->addRigidBody(rigidBody);
	//objetos.push_back(rigidBody);
	//bullet->setObjetos(objetos);
	rigidBody->setGravity(btVector3(0, 0, 0));
	return rigidBody;
}

void cameraThird::moveCamera(btRigidBody * pj1, btVector3 dir) {

	cout << "estoy en moveCamera(64) en cameraThird" << endl;
	glm::vec3 RelativeToCar;

	btTransform transform = pj1->getCenterOfMassTransform();
	btVector3 posicion = transform.getOrigin();
	btQuaternion rotacion = transform.getRotation();
	btVector3 orientacion = dir;
	float altura = 8;
	float distanciaX = 20;
	float distanciaZ = 20;

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_E) == GLFW_PRESS && XCamera3 >= 0) {
		if (XCamera3 < 30) {
			XCamera3++;
			ZCamera3 += 0.5;
		}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_Q) == GLFW_PRESS && XCamera3 <= 0) {
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
	//posicion.setX(XCamera3+posicion.getX());
	//posicion.setY(5+posicion.getY());
	//posicion.setZ(ZCamera3+ posicion.getZ());



	float incremento = 0.1;

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_D) == GLFW_PRESS) {
		if (auxX < 4)
			auxX += incremento;
	}
	else  if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_A) == GLFW_PRESS) {
		if (auxX > -4)
			auxX -= incremento;
	}
	else {
		if (auxX < -0.25) {
			auxX += incremento;
		}
		else if (auxX > 0.25) {
			auxX -= incremento;
		}
		else {
			auxX = 0;
		}
	}

	//	if(orientacion.getZ()>=0 && orientacion.getX()>=0){
	//	distanciaX += auxX;
	//	distanciaZ -= auxX;
	//	}else if(orientacion.getZ()>=0 && orientacion.getX()<=0){
	//	distanciaX -= auxX;
	//	distanciaZ += auxX;
	//	}else if(orientacion.getZ()<=0 && orientacion.getX()>=0){
	//	distanciaX -= auxX;
	//	distanciaZ += auxX;
	//	}else if(orientacion.getZ()<=0 && orientacion.getX()<=0){
	//	distanciaX -= auxX;
	//	distanciaZ += auxX;
	//	}
	//
	//	cout<<"EJEX::::::::"<< orientacion.getX()<<"    " <<"EJEZ:::::::"<< orientacion.getZ()<< endl;


	glm::vec3 RelativeToCarTarget = glm::vec3(0, 1, 0);

	//pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCar);
	//pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCarTarget);
	RelativeToCar.y = 5;
	RelativeToCarTarget.y = 1;
	//	if (!pj1->getAceiteActivado()) {

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_L) == GLFW_PRESS) {
		static_cast<TTransform*>(node->getEntidad())->setPosition(posicion.getX() + orientacion.getX()*distanciaX, posicion.getY() + altura, posicion.getZ() + orientacion.getZ()*distanciaZ);
	}
	else {
		static_cast<TTransform*>(node->getEntidad())->setPosition(posicion.getX() - orientacion.getX()*distanciaX, posicion.getY() + altura, posicion.getZ() - orientacion.getZ()*distanciaZ);
	}
	//static_cast<TTransform*>(node->getEntidad())->setPosition(posicion.getX(), posicion.getY(), posicion.getZ());
	//}
	//else {
	//	static_cast<TTransform*>(node->getEntidad())->setPosition(getPosition().x,getPosition().y,getPosition().z);
	////camera->setTarget(camera->getTarget());
	//}
	fpsActiva = false;

	//}
}
void cameraThird::movefpsCamera(btRigidBody * pj1) {
	/*
	camera = Motor3d::instancia().getDevice()->getSceneManager()->getActiveCamera();
	vector3df RelativeToCar(0,1,0);
	vector3df RelativeToCarTarget;


	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_E) == GLFW_PRESS && XCamera1 >= 0) {
	if (XCamera1 < 30) {
	XCamera1++;
	ZCamera1 -= 0.5;
	}
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_Q) == GLFW_PRESS && XCamera1 <= 0) {
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
	if(glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_L) == GLFW_PRESS)
	RelativeToCarTarget.Z=-ZCamera1;
	//pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCar);
	//pj1->getNodo()->getAbsoluteTransformation().transformVect(RelativeToCarTarget);
	RelativeToCarTarget.Y = 10;
	//RelativeToCar.Y = 1;
	camera->setPosition(RelativeToCar);
	camera->setTarget(RelativeToCarTarget);

	//if(!fpsActiva){
	//	camera = Motor3d::instancia().getScene()->addCameraSceneNodeFPS(pj1->getNodo());
	//	fpsActiva = true;
	//}

	*/
}
void cameraThird::moveCameraControl(btRigidBody *pj1) {

	TNodo *camera = node;
	position = getParentPosition();
	float change_x = 0.0f;
	float change_y = 0.0f;

	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_UP) == GLFW_PRESS) {
		change_y = 1.0f;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_DOWN) == GLFW_PRESS) {
		change_y = -1.0f;
	}


	if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_LEFT) == GLFW_PRESS) {
		change_x = -1.0f;
	}
	else if (glfwGetKey(TMotor::instancia().getVentana(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
		change_x = 1.0f;
	}


	direction = direction + change_x;
	zdirection = direction - change_y;
	if (zdirection < -90)
		zdirection = -90;
	else if (zdirection > 90)
		zdirection = 90;


	glm::vec3 playerPos = glm::vec3(pj1->getCenterOfMassPosition().getX(), pj1->getCenterOfMassPosition().getY(), pj1->getCenterOfMassPosition().getZ());

	float xf = playerPos.x - cos(direction * PI / 180.0f) * 64.0f;
	float yf = playerPos.y - sin(zdirection * PI / 180.0f) * 64.0f;
	float zf = playerPos.z + sin(direction * PI / 180.0f) * 64.0f;

	btVector3 btPos(xf, yf, zf);
	static_cast<TTransform*>(camera->getEntidad())->setPosition(xf, yf + 50, zf);
	//camera->setTarget(core::vector3df(playerPos.X, playerPos.Y + 25.0f, playerPos.Z));
	//pj1->getNodo()->setRotation( core::vector3df( 0, direction, 0 ) );
	fpsActiva = false;

}


GLuint cameraThird::getID() {
	return id;
}

const char *cameraThird::getName() {
	return name;
}

glm::mat4 cameraThird::getParentPosition() {
	glm::mat4 aux = (static_cast<TTransform*>(parentNode->getPadre()->getEntidad())->getMatriz());
	return aux;
}

glm::mat4 cameraThird::getParentRotation() {
	
	glm::mat4 aux = (static_cast<TTransform*>(parentNode->getPadre()->getPadre()->getEntidad())->getMatriz());
	return aux;
}



void cameraThird::setID(GLuint id) {
	this->id = id;
}

void cameraThird::setName(const char *nombre) {
	name = nombre;
	node->setName(nombre);
}



void cameraThird::setRotation(glm::vec3 axis, GLfloat angle) {

	//rotation.x = angle*axis.x;	
	//rotation.y = angle*axis.y;
	//rotation.z = angle*axis.z;

	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->setMatriz(getParentRotation());
	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->rotar(axis.x, axis.y, axis.z, angle);
	rotation =	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->getMatriz();
}


void cameraThird::setPosition(GLfloat X, GLfloat Y, GLfloat Z) {
	static_cast<TTransform*>(node->getPadre()->getEntidad())->setMatriz(getParentPosition());
	static_cast<TTransform*>(node->getPadre()->getEntidad())->trasladar(X, Y, Z);
	position = static_cast<TTransform*>(node->getPadre()->getEntidad())->getMatriz();

}

void cameraThird::setParentNode(TNodo* p) {
	parentNode = p;
}


