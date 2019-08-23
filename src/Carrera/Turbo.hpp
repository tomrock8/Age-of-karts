#ifndef TURBO_H
#define TURBO_H

class btRigidBody;
class btVector3;
class Corredor;
class obj3D;

class Turbo {
public:
	Turbo(btVector3 pos, bool estado);
	~Turbo();

	// METODOS SET
	void setTurboActivo(Corredor* corredor, bool s);
	//void setFrenadaActivo(Corredor *corredor, bool s);
	//void setTurbo(bool valor);
	void setTurboTocado(bool s);

	// METODOS GET
	bool getTurboActivo();
	int getTiempoTurbo();
	int getID();

private:
	obj3D* turbo;
	btRigidBody* rigidBody;
	bool turboActivo;
	bool turboTocado;//para que solo se active la comprobacion 1 vez
	int tiempo;
	int id;

};

#endif /* TURBO */