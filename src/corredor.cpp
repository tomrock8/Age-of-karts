#include "irrlichtlib.hpp"
#include "corredor.hpp"
#include "AxesSceneNode.cpp"


enum
{
    // No colisionable, para evitar cogerlo por error
    ID_NULO = 0,

    // Objetos que pueden colisionar
    ID_COLISION = 1 << 0,

    // Objetos para iluminar
    ID_ILUMINAR = 1 << 1
};
//-------------------------\*
//---CONSTRUCTOR CORREDOR--\*
//-------------------------\*
corredor::corredor(ISceneManager *smgr, stringw rutaObj,s32 id_colision)
{
	//aceleraciones
	aceleracion = 0.1;		   //aceleracion eje Z
	aceleracionInversa = 0.05; //marcha atras
	aceleracionFrenado = 0.15; //aceleracion eje X
	tiempo = 0.5;

	// velocidades
	velocidadIni = 0;
	velocidadX = 0;
	velocidadZ = 0;
	velocidad = 0;
	velocidadMax = 5;

	//posiciones
	posXIni = 0;
	posZIni = 0;
	posYIni = 0;
	posX = 0;
	posZ = 0;
	posY = 0;
	//rotaciones
	Rotacionruedas = 2;
	Rotacioncoche=1;
	rotRuedasY = 0;
	rotCocheY = 0;
	anguloMaxRuedas=50;
	anguloMaxCoche=180;

	// booleanos
	adelante = false;
	atras = false;

	coche = smgr->getMesh(rutaObj);
	cuboNodo = smgr->addMeshSceneNode(coche,0,id_colision);
	cuboNodo->setName("Jugador");
	//cambiar a color rojo del coche
	smgr->getMeshManipulator()->setVertexColors(cuboNodo->getMesh(), SColor(255, 255, 0, 0));
	// Desactivar la iluminacion del cubo
	if (cuboNodo)
	{
		cuboNodo->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminacion
		cuboNodo->setPosition(vector3df(0, 20, 0));
	}

	// una vez hemos establecido la posicion del coche establecemos sus posiciones x,z,y
	posX = cuboNodo->getPosition().X;
	posZ = cuboNodo->getPosition().Z;
	posY = cuboNodo->getPosition().Y;

	// luego declaramos sus ruedas
	ruedasDelanteras = smgr->addCubeSceneNode(0.5f);
	ruedasTraseras = smgr->addCubeSceneNode(0.5f);
	ruedasDelanteras->setParent(cuboNodo);
	ruedasTraseras->setParent(cuboNodo);
	// inicializamos la posicion de las ruedas
	ruedasDelanteras->setPosition(vector3df(-1.2,-0.5,1));
	ruedasTraseras->setPosition(vector3df(-1.2,-0.5,-1));
}
//-----------------------\*
//---CALCULO MOVIMIENTO--\*
//-----------------------\*
float corredor::movimiento(float pos, float vel, float accel, float delta)
{
	pos = pos + vel * delta + (0.5 * accel * (exp2(delta))); //donde el 0 es la velocidad inicial
	return pos;
}
//-----------------------\*
//-------POSICION--------\*
//-----------------------\*

void corredor::updatePosicion()
{
	//posX+=this->getPosicion().X;
	//posY+=this->getPosicion().Y;
	//posZ+=this->getPosicion().Z;
	cuboNodo->setPosition(vector3df(posX,posY,posZ));
	//ruedasDelanteras->setPosition(vector3df(posX - 7, posY - 2 , posZ + 4));
	//ruedasTraseras->setPosition(vector3df(posX - 7, posY - 2 , posZ - 4));
}

void corredor::updateVelocidadInicial()
{
	velocidadIni = velocidad;
}

void corredor::updatePosicionInicial()
{
	posXIni = posX;
	posZIni = posZ;
}

void corredor::update()
{
	updatePosicion();
	updateVelocidadInicial();
	updatePosicionInicial();
}

void corredor::setAxis(ISceneManager *smgr)
{
	AxesSceneNode *axis = new AxesSceneNode(cuboNodo, smgr, -1);
	axis->setAxesScale(20); //  for the length of the axes
	axis->drop();
}

float corredor::getVelocidad()
{
	return velocidad;
}
vector3df corredor::getPosicion()
{
	return cuboNodo->getPosition();
}

vector3df corredor::getRotacion()
{
	return cuboNodo->getRotation();
}

IMeshSceneNode* corredor::getNodo()
{

	return cuboNodo;
}

//-----------------------\*
//---MOVIMIENTO JUGADOR--\*
//-----------------------\*
void corredor::acelerar()
{

	//cout<<"velocidad marcha adelante: "<<v<<"  "<<endl;

	//esto da un ligero retroceso al arrancar de nuevo (sin ser excesivo con v=-2,5)
	if (velocidadIni < 0)
	{
		velocidadIni = 0;
	}

	//variamos la velocidad en funcion de la acceleracion (sin pasar de la velocidad max=5)
	if (velocidad < velocidadMax)
	{
		velocidad = velocidadIni + aceleracion * tiempo;
	}
	//calculamos el desplazamiento en z y x, en funcion del angulo (radianes)
	posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
	posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
	atras = false;
	adelante = true;
}

//-----------------------\*
//---FRENADO JUGADOR---\*
//-----------------------\*
void corredor::frenar()
{
	if (adelante == true)
	{
		posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
		posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
		if (velocidad > -2.5f)
		{ //Controla que no pase de 2.5 de velocidad en marcha atras cuando se frena
			velocidad = velocidadIni - aceleracionFrenado * tiempo;
			//	v = v -0.3;
		}
		//v = vIni -Afrenado*t;
	}
	else
	{
		
		//cout<<"velocidad marcha atras: "<<v<<"  "<<endl;
		atras = true;
		adelante = false;
		if (velocidad > -2.5f && velocidad < 2.5f)
		{
			//v = vIni + Afrenado*t;
			velocidad = velocidadIni - aceleracionInversa * tiempo;
			//	v = v -0.3;
		}

		posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
		posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
	}
}
//-----------------------\*
//------GIRAR JUGADOR----\*
//-----------------------\*
void corredor::girarDerecha()
{	
	if (velocidad>=0.5 || velocidad<=-0.5){
		rotCocheY += Rotacioncoche;

		if(rotCocheY>180){
			rotCocheY=-179;
		}
	}
	/*
	if(rotCocheY > anguloMaxCoche)
	{
		rotCocheY=anguloMaxCoche;
	}
*/
	if (velocidad<0){
		rotRuedasY -= Rotacionruedas;
		if (rotRuedasY < -anguloMaxRuedas)
		{
			rotRuedasY = -anguloMaxRuedas;
		}
	}else{
		rotRuedasY += Rotacionruedas;
		if (rotRuedasY > anguloMaxRuedas)
		{
			rotRuedasY = anguloMaxRuedas;
		}
		
	}
	ruedasDelanteras->setRotation(vector3df(0, rotRuedasY, 0));
	cuboNodo->setRotation(vector3df(0, rotCocheY, 0));
}

void corredor::girarIzquierda()
{
	if (velocidad>=0.5 || velocidad<=-0.5){
		rotCocheY -= Rotacioncoche;
		if(rotCocheY<-180){
			rotCocheY=179;
		}
	}
	/*
	if(rotCocheY < -anguloMaxCoche)
	{
		rotCocheY=-anguloMaxCoche;
	}
*/
	if (velocidad<0){
		rotRuedasY += Rotacionruedas;
		if (rotRuedasY > anguloMaxRuedas)
		{
			rotRuedasY = anguloMaxRuedas;
		}
	}else{
		rotRuedasY -= Rotacionruedas;
		if (rotRuedasY < -anguloMaxRuedas)
		{
			rotRuedasY = -anguloMaxRuedas;
		}
	}
	ruedasDelanteras->setRotation(vector3df(0, rotRuedasY, 0));
	cuboNodo->setRotation(vector3df(0, rotCocheY, 0));
}

void corredor::resetGiro()
{
	if (rotRuedasY>0){
		rotRuedasY-=Rotacionruedas+0.5;
	}
	if (rotRuedasY<0){
		rotRuedasY+=Rotacionruedas+0.5;
	}
	ruedasDelanteras->setRotation(vector3df(0 ,rotRuedasY,0));
}

//-----------------------\*
//---DESACELERAR JUGADOR--\*
//-----------------------\*
void corredor::desacelerar()
{
	//desaceleracion
	//X = Xi + Vi . t - 1/2 . a . t²
	//V = Vi - a . t
	velocidad = velocidadIni - aceleracionInversa * tiempo;
	if (velocidad > 0)
	{
		
		
		if (adelante)
		{ //desacelerar una vez el coche iba hacia adelante
			posZ = posZIni + velocidad * cos((PI / 180) * (rotCocheY));
			posX = posXIni + velocidad * sin((PI / 180) * (rotCocheY));
		}
	}else if (velocidad<0 && velocidad>-2.6){
		velocidad =0;
		if (atras)
		{ //esta parte no se si entra alguna vez XD

			posZ = posZIni - velocidad * cos((PI / 180) * (rotCocheY));
			posX = posXIni - velocidad * sin((PI / 180) * (rotCocheY));
		}
	}
}
void corredor::escalar(float tam)
{
	core::vector3d<f32> factorEscalado(tam, tam, tam);
	cuboNodo->setScale(factorEscalado);
}