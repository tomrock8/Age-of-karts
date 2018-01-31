#include "Habilidad.hpp"

Habilidad::Habilidad(int tipo, ISceneNode* n) : Item(tipo) {
	//almacenamos tipo de habilidad
	tipoHabilidad = tipo;
	NodoVehiculo = n;
	habilidadActiva = false;//hasta que no se lanze no se activa



	if (tipoHabilidad == 1) {
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/onda.obj");
		HabilidadGladiador();
	}
	else if (tipoHabilidad == 2) {
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/habilidadGladiador.obj");
		HabilidadPirata();
	}
	else if (tipoHabilidad == 3) {
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/habilidadVikingo.obj");
		HabilidadVikingo();
	}
	else if (tipoHabilidad == 4) {
		objetoHabilidad = Motor3d::instancia().getScene()->getMesh("assets/Objetos/habilidadChino.obj");
		HabilidadChino();
	}
}


void Habilidad::HabilidadPirata() {
	tamanyo = 2.0f;
	nodo = Motor3d::instancia().getScene()->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadPirata";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.setX(3);
	escala.setY(3);
	escala.setZ(3);
	nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
	nodo->setPosition(vector3df(posicion.getX(), posicion.getY(), posicion.getZ()));
	id = 1235;//esto habra que tocarlo posible null
	nodo->setID(id);
}
void Habilidad::HabilidadVikingo() {//Se queda en stand by, no se puede asignar con irrlicht la fisica necesaria, todo se hace con openGL
	tamanyo = 5.0f;
	nodo = Motor3d::instancia().getScene()->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadVikingo";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.setX(1);
	escala.setY(1);
	escala.setZ(1);
	nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
	nodo->setParent(NodoVehiculo);
	posicion = btVector3(NodoVehiculo->getPosition().X, NodoVehiculo->getPosition().Y, NodoVehiculo->getPosition().Z);
	id = 1236;//esto habra que tocarlo posible null
	nodo->setID(id);

	scene::ISceneNodeAnimator* anim = Motor3d::instancia().getScene()->createFlyCircleAnimator(core::vector3df(0, 0, 0), 10.0f, 0.01f);
	if (anim)
	{
		nodo->addAnimator(anim);
		anim->drop();
	}
}
void Habilidad::HabilidadGladiador() {//Se queda en stand by, no se puede asignar con irrlicht la fisica necesaria, todo se hace con openGL

	tamanyo = 10.0f;
	nodo = Motor3d::instancia().getScene()->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadGladiador";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.setX(10);
	escala.setY(10);
	escala.setZ(10);
	nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
	nodo->setParent(NodoVehiculo);
	posicion = btVector3(NodoVehiculo->getPosition().X, NodoVehiculo->getPosition().Y, NodoVehiculo->getPosition().Z);
	id = 1234;//esto habra que tocarlo posible null
	nodo->setID(id);


}
void Habilidad::HabilidadChino() {

	tamanyo = 2.0f;
	nodo = Motor3d::instancia().getScene()->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadChino";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.setX(1);
	escala.setY(1);
	escala.setZ(1);
	nodo->setScale(vector3df(escala.getX(), escala.getY(), escala.getZ()));
	nodo->setParent(NodoVehiculo);
	posicion = btVector3(NodoVehiculo->getPosition().X, NodoVehiculo->getPosition().Y, NodoVehiculo->getPosition().Z);
	id = 1237;//esto habra que tocarlo posible null
	nodo->setID(id);
}
void Habilidad::lanzarHabilidad() {
	tiempo = Timer::getInstancia();
	tiempo->getInstancia();
	inicioHabilidad = tiempo->getTimer();
	habilidadActiva = true;
	//btRigidBody *rigidBody = inicializarFisicas(tipoHabilidad);
	inicializarFisicas(tipoHabilidad);
	if (tipoHabilidad == 2) {//pirata
		this->getRigidBody()->setGravity(btVector3(0, 0, 0));
		this->getRigidBody()->setLinearVelocity(btVector3(orientacion.getX() * 100, 5.0f, orientacion.getZ() * 100));
	}
	if (tipoHabilidad == 4) {
		this->getRigidBody()->setGravity(btVector3(0, 0, 0));

	}


}
void Habilidad::eliminarFisicas() {
	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	// Remove the object from the world
	mundo->removeRigidBody(rigidBody);
	bullet->setObjetos(objetos);
}
int Habilidad::getInicioHabilidad() {
	return inicioHabilidad;
}

bool Habilidad::getHabilidadActiva() {
	return habilidadActiva;
}

void Habilidad::setHabilidadActiva(bool s) {
	habilidadActiva = s;
}
void Habilidad::setOrientacion(btVector3 o) {
	orientacion = o;

}
void Habilidad::setPosicion(btVector3 p) {
	posicion = p;

}

void Habilidad::setPadre(ISceneNode* n) {
	NodoVehiculo = n;
}

int Habilidad::getTipo() {
	return tipoHabilidad;
}

void Habilidad::inicializarFisicas(int tipo) {

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();

	// Set the initial position of the object

	btTransform Transform;
	Transform.setIdentity();
	if (tipoHabilidad == 1 || tipoHabilidad == 4) {//solo la del chino y gladiador seguira al coche
		Transform.setOrigin(btVector3(Habilidad::getNodo()->getPosition().X, Habilidad::getNodo()->getPosition().Y, Habilidad::getNodo()->getPosition().Z));
	}
	else {
		Transform.setOrigin(posicion);
	}



	MotionState = new btDefaultMotionState(Transform);


	// Create the shape
	/*if ( tipo == 2) {

		btScalar esfera(16);
		Shape = new btSphereShape(esfera);

	}
	else {*/
	btVector3 HalfExtents(escala.getX() * 0.5f, escala.getY() * 0.5f, escala.getZ() * 0.5f);
	Shape = new btBoxShape(HalfExtents);
	//}

	// Add mass
	btVector3 LocalInertia;

	masa = 10;
	Shape->calculateLocalInertia(masa, LocalInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(masa, MotionState, Shape, LocalInertia);

	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	// Store a pointer to the irrlicht node so we can update it later
	rigidBody->setUserPointer((void *)(this->getNodo()));
	if (masa != 0)
		rigidBody->setActivationState(DISABLE_DEACTIVATION);
	// Add it to the world
	mundo->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
	bullet->setObjetos(objetos);


}
