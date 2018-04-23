#include "cameraThird.hpp"
#include "TMotor.hpp"

cameraThird::cameraThird(const char *name, const char *parentName) {
	camara = TMotor::instancia().newCameraNode(name, parentName);
	camara->rotate(glm::vec3(0,1,0),180);
	//this->nodo = nodo;
	//parentNode = parent;

	/*
	name = n;
	id = i;
	direction = 0;
	zdirection = 0;
	auxX = 0;

	rigidBody = initializePhysics();
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

*/
}

cameraThird::~cameraThird() {}

void cameraThird::lookAt(glm::vec3 posicion) {

	/*
	glPushMatrix();
			glLoadIdentity();
			gluLookAt(camara->getPosition().x, camara->getPosition().y, camara->getPosition().z,
					  posicion.x, posicion.y, posicion.z,
					  0,1,0);

			GLfloat MV[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, MV);
		glPopMatrix();

		glm::mat4 T(MV[0], MV[1], MV[2], MV[3],
					MV[4], MV[5], MV[6], MV[7],
					MV[8], MV[9], MV[10], MV[11],
					MV[12], MV[13], MV[14], MV[15]);

		T = glm::inverse(T);
		T[2][0] = -T[2][0];
		T[2][1] = -T[2][1];
		T[2][2] = -T[2][2];


		T[0][0] = -T[0][0];
		T[0][1] = -T[0][1];
		T[0][2] = -T[0][2];

		glPushMatrix();
		glMultMatrixf(glm::value_ptr(T));
		glPopMatrix();
	*/

/*
glm::vec3 delta = posicion-camara->getPosition();
glm::vec3 up;
glm::vec3 direction(glm::normalize(delta));

if(abs(direction.x)< 0.00001 && abs(direction.z) < 0.00001){ 
   if(direction.y > 0)
     up = glm::vec3(0.0, 0.0, -1.0); 
   else
        up = glm::vec3(0.0, 0.0, 1.0); 
   } else {
        up = glm::vec3(0.0, 1.0, 0.0);
   }

up=glm::normalize(up);
glm::vec3 right = glm::normalize(glm::cross(up,direction));
up= glm::normalize(glm::cross(direction, right));
   
glm::mat4 M(right.x, right.y, right.z, 0.0f,     
up.x, up.y, up.z, 0.0f,                      
direction.x, direction.y, direction.z, 0.0f, 
camara->getPosition().x, camara->getPosition().y, camara->getPosition().z, 1.0f);   
*/

 

glm::vec3 delta =(posicion-camara->getPosition());
glm::vec3 desiredUp(0,1,0.00001); 
glm::quat rot1 = RotationBetweenVectors(glm::vec3(0,0,1), delta);
glm::vec3 right = glm::cross(delta, desiredUp);
desiredUp = glm::cross(right, delta);
glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);
glm::quat targetOrientation = rot2 * rot1;
glm::mat4 M=glm::toMat4(targetOrientation);
M[3][0]=camara->getPosition().x;
M[3][1]=camara->getPosition().y;
M[3][2]=camara->getPosition().z;


glm::mat4 lookAtCamara(camara->getNode()->getEntidad()->getModelViewMatrix()* M);
//glMultMatrixf(glm::value_ptr(M));
static_cast<TTransform*>(camara->getNode()->getPadre()->getEntidad())->setMatriz(lookAtCamara);

}

void cameraThird::update(glm::vec3 posicion, glm::vec3 rotacion, btVector3 direccion) {

	float distanciaX = 20;
	camara->setPosition(posicion.x - direccion.getX()*distanciaX, posicion.y + 7, posicion.z - direccion.getZ()*distanciaX);
	//camara->setRotation(0, rotacion.y + 180, 0);
	//gluLookAt(posicion.x - direccion.getX()*distanciaX,posicion.y+ 7,posicion.z - direccion.getZ()*distanciaX , posicion.x,posicion.y,posicion.z,0,1,0);

}
btRigidBody *cameraThird::initializePhysics() {
	/*
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
	*/
	return NULL;
}

void cameraThird::moveCamera(btRigidBody * pj1, btVector3 dir) {
	/*
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
		*/
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
	/*
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
	*/
}


GLuint cameraThird::getID() { return id; }
const char *cameraThird::getName() { return name; }
/*
glm::mat4 cameraThird::getParentPosition() {x
	glm::mat4 aux = (static_cast<TTransform*>(parentNode->getPadre()->getEntidad())->getMatriz());
	return aux;
}

glm::mat4 cameraThird::getParentRotation() {
	glm::mat4 aux = (static_cast<TTransform*>(parentNode->getPadre()->getPadre()->getEntidad())->getMatriz());
	return aux;
}
*/


void cameraThird::setID(GLuint id) {
	this->id = id;
}

void cameraThird::setName(const char *nombre) {
	name = nombre;
	//node->setName(nombre);
}



void cameraThird::setRotation(glm::vec3 axis, GLfloat angle) {

	//rotation.x = angle*axis.x;
	//rotation.y = angle*axis.y;
	//rotation.z = angle*axis.z;
/*
	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->setMatriz(getParentRotation());
	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->rotar(axis.x, axis.y, axis.z, angle);
	rotation =	static_cast<TTransform*>(node->getPadre()->getPadre()->getEntidad())->getMatriz();
*/
}


void cameraThird::setPosition(GLfloat X, GLfloat Y, GLfloat Z) {
	/*
	static_cast<TTransform*>(node->getPadre()->getEntidad())->setMatriz(getParentPosition());
	static_cast<TTransform*>(node->getPadre()->getEntidad())->trasladar(X, Y, Z);
	position = static_cast<TTransform*>(node->getPadre()->getEntidad())->getMatriz();
*/
}

void cameraThird::setParentNode(TNodo* p) {
	//parentNode = p;
}


glm::quat cameraThird::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest){
   start = glm::normalize(start);
   dest = glm::normalize(dest);

   float cosTheta = glm::dot(start, dest);
   glm::vec3 rotationAxis;

   if (cosTheta < -1 + 0.001f){
     rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
     if (glm::length(rotationAxis) < 0.01 ) 
      rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
      rotationAxis = glm::normalize(rotationAxis);
      return glm::angleAxis(180.0f, rotationAxis);
    }

    rotationAxis = glm::cross(start, dest);

    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;

    return glm::quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    ); 
}
