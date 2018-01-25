#include "Habilidad.hpp"

Habilidad::Habilidad(int tipo, ISceneNode* n) : Item(tipo)
{
	// -----------------------------
	//  PREPARAR LA VENTANA
	// -----------------------------
	Motor3d *m = Motor3d::getInstancia();
	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();
	//almacenamos tipo de habilidad
	tipoHabilidad = tipo;
	NodoVehiculo = n;
	habilidadActiva = false;//hasta que no se lanze no se activa
	

	
	if (tipoHabilidad == 1) {
		objetoHabilidad = smgr->getMesh("assets/Objetos/onda.obj");
		HabilidadGladiador();
	}	else if (tipoHabilidad == 2) {
		objetoHabilidad = smgr->getMesh("assets/Objetos/habilidadGladiador.obj");
		HabilidadPirata();
	}else if (tipoHabilidad == 3) {
		objetoHabilidad = smgr->getMesh("assets/Objetos/habilidadVikingo.obj");
		HabilidadVikingo();
	}else if (tipoHabilidad == 4) {
		objetoHabilidad = smgr->getMesh("assets/Objetos/habilidadChino.obj");
		HabilidadChino();
	}
}


void Habilidad::HabilidadPirata () {

	Motor3d *m = Motor3d::getInstancia();
	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();

	tamanyo = 2.0f;
	nodo = smgr->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadPirata";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.X = 3.0f;
	escala.Y = 3.0f;
	escala.Z = 3.0f;
	nodo->setScale(escala);
	nodo->setPosition(posicion);
	id = 1235;//esto habra que tocarlo posible null
	nodo->setID(id);
}
void Habilidad::HabilidadVikingo() {//Se queda en stand by, no se puede asignar con irrlicht la fisica necesaria, todo se hace con openGL

	Motor3d *m = Motor3d::getInstancia();
	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();

	tamanyo = 5.0f;
	nodo = smgr->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadVikingo";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.X = 1.0f;
	escala.Y = 1.0f;
	escala.Z = 1.0f;
	nodo->setScale(escala);
	nodo->setParent(NodoVehiculo);
	posicion = NodoVehiculo->getPosition();
	id = 1236;//esto habra que tocarlo posible null
	nodo->setID(id);

	scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(core::vector3df(0, 0, 0), 10.0f,0.01f);
	if (anim)
	{
	nodo->addAnimator(anim);
	anim->drop();
	}
}
void Habilidad::HabilidadGladiador() {//Se queda en stand by, no se puede asignar con irrlicht la fisica necesaria, todo se hace con openGL

	Motor3d *m = Motor3d::getInstancia();
	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();

	tamanyo = 10.0f;
	nodo = smgr->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadGladiador";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.X = 10.0f;
	escala.Y = 10.0f;
	escala.Z = 10.0f;
	nodo->setScale(escala);
	nodo->setParent(NodoVehiculo);
	posicion = NodoVehiculo->getPosition();
	id = 1234;//esto habra que tocarlo posible null
	nodo->setID(id);


}
void Habilidad::HabilidadChino() {

	Motor3d *m = Motor3d::getInstancia();
	IVideoDriver *driver = m->getDriver();
	ISceneManager *smgr = m->getScene();

	tamanyo = 2.0f;
	nodo = smgr->addMeshSceneNode(objetoHabilidad, 0);
	nombre = "HabilidadChino";
	nodo->setName(nombre);
	nodo->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL); //transparencia

	nodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
	escala.X = 1.0f;
	escala.Y = 1.0f;
	escala.Z = 1.0f;
	nodo->setScale(escala);
	nodo->setParent(NodoVehiculo);
	posicion = NodoVehiculo->getPosition();
	id = 1237;//esto habra que tocarlo posible null
	nodo->setID(id);
}
void Habilidad::lanzarHabilidad() {
	tiempo = Timer::getInstancia();
	tiempo->getInstancia();
	inicioHabilidad = tiempo->getTimer();
	habilidadActiva = true;
	btRigidBody *rigidBody = inicializarFisicas(tipoHabilidad);
	if (tipoHabilidad == 2) {//pirata
		this->getRigidBody()->setGravity(btVector3(0, 0, 0));
		this->getRigidBody()->setLinearVelocity(btVector3(orientacion.X * 100, 5.0f, orientacion.Z * 100));
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
//void Habilidad::setOrientacion(btVector3 o) {
//	orientacion = o;
//
//}
//void Habilidad::setPosicion(btVector3 p) {
//	posicion = p;
//
//}
void Habilidad::setOrientacion(vector3df o) {
	orientacion = o;

}
void Habilidad::setPosicion(vector3df p) {
	posicion = p;

}
void Habilidad::setPadre(ISceneNode* n){
	NodoVehiculo = n;
	}
int Habilidad::getTipo() {
	return tipoHabilidad;
}

btRigidBody *Habilidad::inicializarFisicas(int tipo)
{

	MotorFisicas *bullet = MotorFisicas::getInstancia();
	btDynamicsWorld *mundo = bullet->getMundo();
	core::list<btRigidBody *> objetos = bullet->getObjetos();
	// Set the initial position of the object

	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(posicion.X, posicion.Y, posicion.Z));
	MotionState = new btDefaultMotionState(Transform);


	// Create the shape
	if (tipo == 1 || tipo == 2) {

		btScalar esfera(16);
		Shape = new btSphereShape(esfera);
				
	}
	else {
		btVector3 HalfExtents(escala.X * 0.5f, escala.Y * 0.5f, escala.Z * 0.5f);
		Shape = new btBoxShape(HalfExtents);
	}
	
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

	return rigidBody;
}
