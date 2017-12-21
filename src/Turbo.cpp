#include "Turbo.hpp"
//el turbo se creara con una id y una posicion que se recojera por fichero 
Turbo::Turbo(int id, btVector3 pos, bool estado){
    turboActivo = estado;
    MotorFisicas *bullet = MotorFisicas::getInstancia();
    irr:core::list<btRigidBody *> objetos = bullet->getObjetos();
    Motor3d *m = Motor3d::getInstancia();
    turbo = m->getScene()->addCubeSceneNode(5.0f);
    escala = vector3df(2.5f,0.2f,1.25f);
    turbo->setScale(escala);
	turbo->setMaterialFlag(EMF_LIGHTING, false);
	turbo->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	turbo->setName("Turbo");
    turbo->setID(id);
    m->getScene()->getMeshManipulator()->setVertexColors(turbo->getMesh(), SColor(255, 255, 0, 0));
	//posicion origem 
	transform;
	transform.setIdentity();
	transform.setOrigin(pos);
	

	MotionState = new btDefaultMotionState(transform);

    // Create the shape
    halfExtents = btVector3(escala.X*2,escala.Y+5,escala.Z*2);

	Shape = new btBoxShape(halfExtents);

	// sin masa
	localInertia;
	Shape->calculateLocalInertia(0, localInertia);

	// Create the rigid body object
	rigidBody = new btRigidBody(0, MotionState, Shape,localInertia);

	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	//ACTIVA LA COLISION SIN COLISIONAR CON EL OBJETO
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// Store a pointer to the irrlicht node so we can update it later
	rigidBody->setUserPointer((void *)(turbo));

	// Add it to the world
	bullet->getMundo()->addRigidBody(rigidBody);
	objetos.push_back(rigidBody);
    bullet->setObjetos(objetos);
    
	

}

bool Turbo::getTurboActivo(){
   //cout<<"esta activo ? :"<<turboActivo<<endl;
    return turboActivo;
} 
void Turbo::setTurboActivo(Corredor *c, bool s){
    corredor = c;
    Motor3d *m = Motor3d::getInstancia();
    turboActivo = s;
    if(turboActivo){//si esta activo almacenamos tiempo y aumentamos fuerza
            corredor->SetFuerzaVelocidad(30000);
            tiempo = m->getDevice()->getTimer()->getTime();
    }  
    else corredor->SetFuerzaVelocidad(10000);


}
void Turbo::quitarTurbo(){
    turboActivo = false;
    corredor->SetFuerzaVelocidad(6000);
}
int Turbo::getTiempoTurbo(){
    return tiempo;
}